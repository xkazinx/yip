#include "stdafx.h"
#include "global.h"
#include "SceneMgr.h"
#include "Camera.h"
#include "Random.h"
#include "TextMgr.h"

namespace global
{
	unique_ptr<App> g_app;
	unique_ptr<SceneMgr> g_scene;
	unique_ptr<Camera> g_camera;
	unique_ptr<TextMgr> g_text;
	unique_ptr<Random> g_rand;

	SGame * g_game = nullptr;
	Player * g_player = nullptr;

	RenderWindow & GetWindow() {
		return g_app->_window;
	}

	Vector2f DeltaVelocity(const Vector2f & velocity, const sf::Time & delta)
	{
		const float ms = (float)delta.asMilliseconds();
		return { velocity.x * ms, velocity.y * ms };
	}

	namespace app
	{
		void Initialize() {
			// unordered
			g_rand = mk<Random>();
			g_text = mk<TextMgr>();

			// ordered
			g_camera = mk<Camera>();
			g_scene = mk<SceneMgr>();
		}

		void Deinitialize() {
			g_scene.reset();
			g_camera.reset();
			g_text.reset();
			g_rand.reset();
		}
	}
}

namespace resolution {
	sf::Vector2f GetCenter(const Vector2f & base)
	{
		return { kResX / 2.f - base.x / 2.f, kResY / 2.f - base.y / 2.f };
	}
}
