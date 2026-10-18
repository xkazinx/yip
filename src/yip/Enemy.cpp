#include "stdafx.h"
#include "Enemy.h"
#include "Camera.h"
#include "SGame.h"
#include "Player.h"
#include "PDot.h"

Enemy::Enemy(const Vector2f & pos)
	: Object(pos, ObjectID::Enemy), _shape({ 30.f, 30.f })
{
	_shape.setFillColor(sf::Color::Yellow);
	_shape.setOrigin(_shape.getSize() / 2.f);
}

Enemy::~Enemy()
{

}

void Enemy::Update(const sf::Time & delta)
{
	// A shot at the player every second
	if (_fire_timer.getElapsedTime().asMilliseconds() >= 1000)
	{
		_fire_timer.restart();
		g_game->_projectiles += mk<PDot>(getPosition(), g_player->getPosition());
	}

	// Still in the world: the camera's scroll brings it towards the player
	constexpr auto velocity = 0.05f;
	move({ 0.05f - velocity * delta.asMilliseconds(), 0.f });
}

Object::Collider Enemy::GetCollider() const
{
	const auto rect = _shape.getGlobalBounds();
	return sf::FloatRect(rect.position + getPosition(), rect.size);
}

bool Enemy::_HasExpired() const {
	return getPosition().x < g_camera->GetLeft();
}

void Enemy::HandleCollition(const Object * object)
{
	if (object->Is(ObjectID::PNormal)) {
		_hit = true;
		Expire();
	}
}
