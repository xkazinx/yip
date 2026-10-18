#include "stdafx.h"
#include "Testerino.h"
#include "App.h"
#include "Camera.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "SGame.h"
#include "Player.h"
#include "Input.h"
#include <initflags.h>
#include <testerino/tcore.h>

namespace testerino
{
	namespace
	{
		using tcore::JArr;
		using tcore::JObj;

		tcore::Endpoint g_ep;
		tcore::Runner g_run(g_ep);
		bool g_active = false;
		string g_scene_seen;

		struct Shot
		{
			string _path;
			tcore::Job * _job = nullptr;
		};
		vector<Shot> g_shots;

		// Held keys let go at a time (hold <key> <ms>)
		struct Timed
		{
			Key _key;
			sf::Clock _clock;
			int _ms = 0;
		};
		vector<Timed> g_timed;

		string SceneName()
		{
			const Scene * const s = g_scene ? g_scene->Current() : nullptr;
			return s ? string(enum_name(s->_id)) : "";
		}

		bool InGame()
		{
			return g_scene && g_scene->Current() == (Scene *)g_game;
		}

		long long Count(const ObjectID id, const vector<unique_ptr<Object>> & list)
		{
			return std::ranges::count_if(list, [id](const unique_ptr<Object> & o) { return o->Is(id); });
		}

		JObj State()
		{
			JObj o;
			o.Str("scene", SceneName()).Num("camera_x", g_camera->getCenter().x);

			if (InGame() && g_player)
			{
				o.Num("x", g_player->getPosition().x).Num("y", g_player->getPosition().y);
				o.Num("screen_x", g_player->getPosition().x - g_camera->GetLeft()).Num("screen_y", g_player->getPosition().y - g_camera->GetTop());
				o.Str("weapon", string(enum_name(g_player->_weapon))).Int("hits_taken", g_player->_hits_taken);
				o.Int("enemies", Count(ObjectID::Enemy, g_game->_objects)).Int("shots", Count(ObjectID::PNormal, g_game->_projectiles));
				o.Int("enemy_shots", Count(ObjectID::PDot, g_game->_projectiles));
				o.Int("enemies_killed", g_game->_enemies_killed).Int("enemies_missed", g_game->_enemies_missed);
				o.Int("projectiles_hit", g_game->_projectiles_hit).Int("projectiles_missed", g_game->_projectiles_missed);
			}

			JArr held;

			for (const Key k : input::HeldByTest())
				held.Str(string(enum_name(k)));

			o.Raw("held", held.Done());
			return o;
		}

		// A key by SFML's name, any case: A, Left, Num2, Enter, Escape...
		bool KeyCode(const string & name, Key & code)
		{
			const auto k = magic_enum::enum_cast<Key>(name, magic_enum::case_insensitive);
			if (!k || *k == Key::Unknown)
				return false;
			code = *k;
			return true;
		}

		void KeyEvent(const Key code, const bool down, const bool control)
		{
			if (down)
			{
				sf::Event::KeyPressed k{};
				k.code = code;
				k.scancode = sf::Keyboard::delocalize(code);
				k.control = control;
				g_app->ProcessEvent(k);
			}
			else
			{
				sf::Event::KeyReleased k{};
				k.code = code;
				k.scancode = sf::Keyboard::delocalize(code);
				k.control = control;
				g_app->ProcessEvent(k);
			}
		}

		void Mouse(const sf::Vector2i at, const int what)
		{
			if (what == 0)
				g_app->ProcessEvent(sf::Event::MouseMoved{ at });
			else if (what == 1)
				g_app->ProcessEvent(sf::Event::MouseButtonPressed{ sf::Mouse::Button::Left, at });
			else
				g_app->ProcessEvent(sf::Event::MouseButtonReleased{ sf::Mouse::Button::Left, at });
		}

		void AddVerbs()
		{
			g_run.Add("state", "state", "the scene (Main | Game), the camera, and in game the player (x y, on screen, weapon, hits taken), enemies, shots and the score; held: the keys the test holds", [](const tcore::Cmd &, tcore::Job & j) {
				j.Ok(State());
			});

			g_run.Add("key", "key <name> [ctrl] [times]", "a key pressed and let go as events (SFML names: Enter, Escape, E, Num1..Num3...), ctrl with Control down", [](const tcore::Cmd & c, tcore::Job & j) {
				Key code{};
				if (!KeyCode(c.Arg(0), code))
					return j.Fail("unknown key " + c.Arg(0));
				const bool control = tcore::Lower(c.Arg(1)) == "ctrl";
				int times = 1;
				c.ArgInt(control ? 2 : 1, times);

				for (int i = 0; i < std::max(1, times); i++)
				{
					j.Add([code, control] { KeyEvent(code, true, control); return true; });
					j.Add([code, control] { KeyEvent(code, false, control); return true; });
				}

				j.Add([&j] { j.Ok(State()); return true; });
			});

			g_run.Add("hold", "hold <name> [ms]", "holds a key the game reads every tick (A fires, the arrows move), until release or for ms", [](const tcore::Cmd & c, tcore::Job & j) {
				Key code{};
				if (!KeyCode(c.Arg(0), code))
					return j.Fail("unknown key " + c.Arg(0));
				input::Hold(code, true);
				int ms = 0;
				if (c.ArgInt(1, ms) && ms > 0)
					g_timed += Timed{ code, {}, ms };
				j.Ok(State());
			});

			g_run.Add("release", "release [name]", "lets go of a held key, or of every one", [](const tcore::Cmd & c, tcore::Job & j) {
				if (c.Arg(0).empty())
				{
					input::ReleaseAll();
					g_timed.clear();
				}
				else
				{
					Key code{};
					if (!KeyCode(c.Arg(0), code))
						return j.Fail("unknown key " + c.Arg(0));
					input::Hold(code, false);
					std::erase_if(g_timed, [code](const Timed & t) { return t._key == code; });
				}
				j.Ok(State());
			});

			g_run.Add("spawn", "spawn [n] [y]", "n enemies off the right of the screen (as Ctrl+E), at screen height y (random without it)", [](const tcore::Cmd & c, tcore::Job & j) {
				if (!InGame())
					return j.Fail("not in game", State());
				int n = 1, y = -1;
				c.ArgInt(0, n);
				const bool at_y = c.ArgInt(1, y);

				for (int i = 0; i < std::max(1, n); i++)
				{
					g_game->SpawnEnemy();
					if (at_y)
						g_game->_objects.back()->setPosition({ g_game->_objects.back()->getPosition().x, g_camera->GetTop() + y });
				}

				j.Ok(State());
			});

			g_run.Add("click", "click <x> <y>", "a left click at that window spot (move, press, release on three ticks)", [](const tcore::Cmd & c, tcore::Job & j) {
				int x = 0, y = 0;
				if (!c.ArgInt(0, x) || !c.ArgInt(1, y))
					return j.Fail("click <x> <y>");
				const sf::Vector2i at(x, y);
				j.Add([at] { Mouse(at, 0); return true; });
				j.Add([at] { Mouse(at, 1); return true; });
				j.Add([at] { Mouse(at, 2); return true; });
				j.Add([&j] { j.Ok(State()); return true; });
			});

			g_run.Add("shot", "shot <name>", "a screenshot of the next frame (shots\\<name>.png)", [](const tcore::Cmd & c, tcore::Job & j) {
				if (c.Arg(0).empty())
					return j.Fail("shot <name>");
				g_shots += Shot{ g_ep.ShotPath(c.Arg(0)).string(), &j };
				j.Add([&j] { return j.answered; }); // AfterDraw answers it
				j.OnAbort([&j] { std::erase_if(g_shots, [&j](const Shot & s) { return s._job == &j; }); });
			});

			g_run.Add("quit", "quit", "ends the program", [](const tcore::Cmd &, tcore::Job & j) {
				j.Ok();
				g_app->_window.close();
			});

			g_run.AddCond("scene", "scene=<Main|Game>", [](const string & v, char, tcore::Cond & out, string &) {
				out = [v](string & d) {
					d = SceneName();
					return tcore::Lower(d) == tcore::Lower(v);
				};
				return true;
			});

			// A number from the state: <key>=<n> exactly, <key>~<n> at least n (killed~1, enemies=0...)
			auto number_cond = [](const char * key, function<long long()> get) {
				g_run.AddCond(key, string(key) + "=<n> exactly / " + key + "~<n> at least", [get](const string & v, const char op, tcore::Cond & out, string &) {
					long long n = 0;
					try { n = std::stoll(v); }
					catch (...) { return false; }
					out = [n, op, get](string & d) {
						const long long now = get();
						d = to_string(now);
						return op == '~' ? now >= n : now == n;
					};
					return true;
				});
			};
			number_cond("killed", [] { return InGame() ? (long long)g_game->_enemies_killed : 0; });
			number_cond("enemies", [] { return InGame() ? Count(ObjectID::Enemy, g_game->_objects) : 0; });
			number_cond("hits_taken", [] { return InGame() && g_player ? (long long)g_player->_hits_taken : 0; });

			g_run.ping_fields = [](JObj & o) { o.Str("scene", SceneName()); };
		}
	}

	void Start()
	{
		const int slot = (int)lib::init::Number("testerino", 0);
		if (slot <= 0)
			return;

		std::error_code ec;
		if (!tcore::fs::exists(tcore::CheckoutFromExe() / "src" / "yip.sln", ec))
		{
			cout << "(!) testerino is only on in a source checkout (src\\yip.sln)\n";
			return;
		}

		AddVerbs();
		if (!g_ep.Open(tcore::CheckoutFromExe() / "testerino", "client_" + to_string(slot), JObj().Int("slot", slot)))
		{
			cout << "(!) testerino: cannot open its folder\n";
			return;
		}

		g_active = true;
		cout << "Testerino: listening on " << g_ep.Dir().string() << "\n";
	}

	void Pump()
	{
		if (!g_active)
			return;

		std::erase_if(g_timed, [](const Timed & t) {
			if (t._clock.getElapsedTime().asMilliseconds() < t._ms)
				return false;
			input::Hold(t._key, false);
			return true;
		});

		g_run.Pump();

		const string scene = SceneName();
		if (scene != g_scene_seen)
		{
			g_scene_seen = scene;
			g_ep.Event("scene", JObj().Str("scene", scene));
		}
	}

	void AfterDraw(sf::RenderWindow & wnd)
	{
		if (g_shots.empty())
			return;

		static sf::Texture texture;
		const auto sz = wnd.getSize();
		if (texture.getSize() != sz && !texture.resize(sz))
			return;
		texture.update(wnd);
		const sf::Image image = texture.copyToImage();

		tcore::Rgba img;
		img.w = (int)image.getSize().x;
		img.h = (int)image.getSize().y;
		img.px.assign(image.getPixelsPtr(), image.getPixelsPtr() + (size_t)img.w * img.h * 4);

		for (auto & s : g_shots)
		{
			if (tcore::WritePng(s._path, img))
				s._job->Ok(JObj().Str("path", s._path).Int("w", img.w).Int("h", img.h));
			else
				s._job->Fail("could not write " + s._path);
		}

		g_shots.clear();
	}

	void Stop()
	{
		if (g_active)
			g_ep.Close();
		g_active = false;
	}

	bool Active()
	{
		return g_active;
	}
}
