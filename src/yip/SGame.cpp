#include "stdafx.h"
#include "SGame.h"
#include "TextMgr.h"
#include "Background.h"
#include "Camera.h"
#include "Player.h"
#include "Random.h"
#include "Enemy.h"
#include "SceneMgr.h"
#include "Input.h"

SGame::SGame() : Scene(ID::Game)
{

}

SGame::~SGame()
{
}

void SGame::Reset()
{
	_objects.clear();
	_projectiles.clear();
	_objects += mk<Background>(1024, Vector2f(800.f, kResY));
	_objects += mk<Player>(Vector2f{ 50.f, kResY / 2.f });
	g_player = (Player *)_objects[1].get();

	_projectiles_hit = _projectiles_missed = _enemies_killed = _enemies_missed = 0;
}

void SGame::SpawnEnemy()
{
	_objects += mk<Enemy>(Vector2f{
		g_camera->GetRight() + 100.f,
		g_rand->Generate<float>(0.f, kResY)
	});
}

void SGame::Update(const sf::Time & delta)
{
	Vector2f velocity(0.05f, 0.f);
	g_camera->move(DeltaVelocity(velocity, delta));

	// The list grows while it runs (Enemy::Update fires), so by index
	for (szt i = 0; i < _objects.size(); i++)
		_objects[i]->Update(delta);

	for (auto & p : _projectiles)
		p->Update(delta);

	for (auto & obj : _objects)
	{
		for (auto & p : _projectiles)
		{
			if (!obj->HasExpired() && !p->HasExpired() && obj->CollidesWith(p.get())) {
				obj->HandleCollition(p.get());
				p->HandleCollition(obj.get());

				if (obj->Is(ObjectID::Player) && p->Is(ObjectID::PDot))
					g_player->_hits_taken++;
			}
		}
	}

	std::erase_if(_projectiles, [&](const unique_ptr<Object> & p) {
		if (!p->HasExpired())
			return false;

		if (p->Is(ObjectID::PNormal))
			(p->_hit ? _projectiles_hit : _projectiles_missed)++;

		return true;
	});

	std::erase_if(_objects, [&](const unique_ptr<Object> & obj) {
		if (!obj->HasExpired())
			return false;

		if (obj->Is(ObjectID::Enemy))
			(obj->_hit ? _enemies_killed : _enemies_missed)++;

		return true;
	});
}

void SGame::Render(TargetStatesRef && ts) const
{
	for (auto & obj : _objects) {
		ts().draw(*obj.get(), ts.states);
	}

	for (auto & obj : _projectiles) {
		ts().draw(*obj.get(), ts.states);
	}

	g_app->_window.setView(g_app->_window.getDefaultView());
	g_text->DrawText(std::format("Enemies killed: {}/{}", _enemies_killed, _enemies_killed + _enemies_missed), { 10.f, 10.f });
	g_text->DrawText(std::format("Projectiles hit: {}/{}", _projectiles_hit, _projectiles_hit + _projectiles_missed), { 10.f, 40.f });
	g_text->DrawText(std::format("Hits taken: {}", g_player ? g_player->_hits_taken : 0), { 10.f, 70.f });
}

void SGame::HandleEvent(const sf::Event & event)
{
	const auto pressed = event.getIf<sf::Event::KeyPressed>();
	if (!pressed)
		return;

	switch (pressed->code)
	{
	// Ctrl+E: an enemy
	case Key::E:
		if (pressed->control)
			SpawnEnemy();
		break;

	case Key::Escape:
		input::ReleaseAll();
		g_scene->Set(ID::Main);
		break;

	case Key::Num1:
		g_player->SetWeapon(Weapon::Normal);
		break;

	case Key::Num2:
		g_player->SetWeapon(Weapon::Laser);
		break;

	case Key::Num3:
		g_player->SetWeapon(Weapon::Triple);
		break;

	default:
		break;
	}
}
