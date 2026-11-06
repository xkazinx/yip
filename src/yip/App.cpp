// common
#include "stdafx.h"
#include "App.h"
// extra
#include "Ticker.h"
#include "Camera.h"
#include "SceneMgr.h"
#include "Testerino.h"
#include <initflags.h>

App::App()
{
	global::app::Initialize();

	_CreateWindow();
}

App::~App()
{
	global::app::Deinitialize();
}

void App::_CreateWindow()
{
	ContextSettings settings;
#ifndef __EMSCRIPTEN__
	// (a browser's WebGL canvas has neither: asking only printed a warning there)
	{
		settings.depthBits = 24;
		settings.stencilBits = 8;
		settings.antiAliasingLevel = 0;
	}
#endif

	// title=<text> names the window (several test runs side by side)
	const string title = lib::init::Value("title", kAppName);
	_window.create(VideoMode({ kResX, kResY }), title, sf::Style::Default, sf::State::Windowed, settings);
	_window.setVerticalSyncEnabled(true);
}

App * App::Tick()
{
	testerino::Start();

	Ticker ticker;
	while (_window.isOpen())
	{
		if (!HandleEvents())
			break;

#ifdef __EMSCRIPTEN__
		// A browser: one logic tick and one frame per pass; display() waits for the
		// browser's next animation frame (the page's refresh), so nothing to wait for
		// (sleep_for would spin there). The logic moves by the real time passed.
		g_scene->Update(_delta_clock.restart());
		_window.clear(sf::Color::Black);
		_window.setView(*g_camera);
		_window.draw(*g_camera);
		_window.setView(_window.getDefaultView());
		_window.display();
		continue;
#endif

		ticker.Tick([&]()
		{
			testerino::Pump();
			sf::Time elapsed = _delta_clock.restart();
			g_scene->Update(elapsed);
		}, [&]()
		{
			_window.setView(*g_camera);
			_window.draw(*g_camera);
			_window.setView(_window.getDefaultView());
			testerino::AfterDraw(_window);
		});

		this_thread::sleep_for(chrono::milliseconds(1));
	}

	testerino::Stop();
	return this;
}

int App::Destroy()
{
	g_app.reset();
	return 0;
}

bool App::HandleEvents()
{
	while (const optional<sf::Event> event = _window.pollEvent())
	{
		ProcessEvent(*event);
		if (!_window.isOpen())
			return false;
	}

	return true;
}

void App::ProcessEvent(const sf::Event & event)
{
	if (event.is<sf::Event::Closed>())
	{
		_window.close();
		return;
	}

	// The view keeps the game's 800x600 when the window is resized (SFML 3 scales the
	// default view with the window)
	if (event.is<sf::Event::Resized>())
		return;

	g_scene->HandleEvent(event);
}
