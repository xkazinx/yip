#include "stdafx.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Camera.h"

#include "SMain.h"
#include "SGame.h"

SceneMgr::SceneMgr()
{
	// #scene create
	At(ID::Main) = mk<SMain>();
	At(ID::Game) = mk<SGame>();

	g_game = (SGame *)At(ID::Game).get();

	for (auto & s : _list)
		_mgr.Add(*s);

	// "Scene: Game" on the console (the browser test waits for it, src\tests\web\)
	_mgr._on_set = [](lib::scene::Base * from, lib::scene::Base * to) {
		g_camera->BindScene(static_cast<Scene *>(to));
		if (from != to)
			cout << "Scene: " << enum_name(static_cast<Scene *>(to)->_id) << endl;
	};

	Set(ID::Main);
}

SceneMgr::~SceneMgr()
{
	g_camera->BindScene(nullptr);
}

// Entering another scene resets it (Scene::Initialize); the current one again does nothing
void SceneMgr::Set(const ID id) {
	_mgr.Set(*At(id));
}

Scene * SceneMgr::Current() const
{
	return static_cast<Scene *>(_mgr._current);
}

void SceneMgr::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (Current())
		target.draw(*Current(), states);
}

void SceneMgr::Update(const sf::Time & delta) {
	if (Current())
		Current()->Update(delta);
}

void SceneMgr::HandleEvent(const sf::Event & event) {
	_mgr.HandleEvent(&event);
}
