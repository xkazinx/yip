#pragma once
class FpsCounter
{
public:
	FpsCounter(const string & name);
	~FpsCounter();

	// Return fps count after restarting clock and counter
	u16 Reset();

	// Increase counter and check for time
	void Tick();

	// The last second's count
	u16 _last = 0;

private:
	u16 _count = 0;
	string _name;
	sf::Clock _clock;

	// If time has elapsed for reset
	auto Elapsed() const {
		return _clock.getElapsedTime().asSeconds() >= 1;
	}
};
