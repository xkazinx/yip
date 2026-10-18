#include "stdafx.h"
#include "SMain.h"
#include "SceneMgr.h"
#include "TextMgr.h"
#include "Button.h"
#include "Background.h"
#include "Camera.h"

SMain::SMain() : Scene(ID::Main)
{
	_button = AddComponent<Button>(
		mk<Button>(
			[](const auto & event) {
				g_scene->Set(ID::Game);
			},
			resolution::GetCenter({ 100.f, 50.f }),
			"Start Game"
		)
	);

	_background = mk<Background>(512, Vector2f(1000.f, kResY));
}

SMain::~SMain()
{
}

void SMain::Reset()
{

}

void SMain::Update(const sf::Time & delta)
{
	g_camera->move({ 0.05f * delta.asMilliseconds(), 0.f });
	_background->Update(delta);
}

void SMain::Render(TargetStatesRef && ts) const
{
	ts().draw(*_background.get(), ts.states);
}

void SMain::HandleEvent(const sf::Event & event)
{
	const auto released = event.getIf<sf::Event::KeyReleased>();
	if (released && released->code == Key::Enter)
		g_scene->Set(ID::Game);
}
