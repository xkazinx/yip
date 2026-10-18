#pragma once
#include "FpsCounter.h"

class App
{
public:
	App();
	~App();

	// Returns pointer to destroy instance after tick ends
	App * Tick();

	// Returns false if the processed event has closed the window
	bool HandleEvents();

	// One event, real or Testerino's
	void ProcessEvent(const sf::Event & event);

	// Returns int as main() expects
	int Destroy();

	// To track the elapsed time between frames
	sf::Clock _delta_clock;

	// Main window we render to
	RenderWindow _window;

private:
	void _CreateWindow();
};
