#pragma once

class Ticker {
public:
	class Unit {
	public:
		Unit(const string & name, const int fps)
			: _ups(sf::seconds(1.f / fps)), _fps(name) {

		}

		sf::Time _ups;
		sf::Time _accumulator = sf::Time::Zero;
		sf::Clock _clock;
		FpsCounter _fps;

		auto IsTick() const {
			return _accumulator > _ups;
		}

		template<typename Func>
		void Tick(Func func) {
			if (IsTick()) {
				_accumulator -= _ups;

				func();

				_fps.Tick();
			}

			_accumulator += _clock.restart();
		}
	};

	Ticker() {

	}

	template<typename LogicFunc, typename RenderFunc>
	void Tick(LogicFunc logic_func, RenderFunc render_func) {
		_units[0].Tick(logic_func);

		_units[1].Tick([&]()
		{
			GetWindow().clear(sf::Color::Black);
			render_func();
			GetWindow().display();
		});
	}

	vector<Unit> _units = {
		{ "logic", 60 },
		{ "render", 60 }
	};
};
