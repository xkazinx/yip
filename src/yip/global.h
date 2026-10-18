#pragma once
#include "App.h"

class App;
class SceneMgr;
class Camera;
class TextMgr;
class Random;
class Background;
class Button;
class Player;
class Object;
class SGame;

#define DEBUG_MODE

namespace global
{
	constexpr char kAppName[] = "Yip";

	extern unique_ptr<App> g_app;
	extern unique_ptr<SceneMgr> g_scene;
	extern unique_ptr<Camera> g_camera;
	extern unique_ptr<TextMgr> g_text;
	extern unique_ptr<Random> g_rand;

	extern SGame * g_game;
	extern Player * g_player;

	extern RenderWindow & GetWindow();

	namespace app
	{
		template<typename Class, typename Ptr>
		static auto & Create(Ptr && ptr) {
			ptr = mk<Class>();
			return ptr;
		}

		extern void Initialize();
		extern void Deinitialize();
	}

	extern Vector2f DeltaVelocity(const Vector2f & velocity, const sf::Time & delta);
}

namespace scene {
	enum class ID : u8 {
		Main,
		Game,
	};
}

namespace font {
	enum class ID : u8 {
		Arial,
	};

	// Next to the exe (deploy\)
	constexpr array<string_view, enum_count<ID>()> kNames = {
		"arial.ttf",
	};
}

template<class T>
class TargetStatesBase {
public:
	sf::RenderTarget & target;
	T states;

	auto & operator ()() {
		return target;
	}
};

using TargetStates = TargetStatesBase<sf::RenderStates>;
using TargetStatesRef = TargetStatesBase<sf::RenderStates&>;

enum class ObjectID : u8 {
	None,
	Player,
	PNormal,
	PDot,
	Enemy,
	Star,
	Background,
};

enum class Weapon : u8
{
	Normal,
	Triple,
	Laser,
	Dot,
};

namespace resolution
{
	extern sf::Vector2f GetCenter(const Vector2f & base);
}
