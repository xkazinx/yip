#pragma once

// Keys held down, read by the game every tick: the keyboard while the window has
// focus, plus the keys Testerino holds (its `hold` verb), so a test plays the game
// from the background
namespace input
{
	bool Held(const Key key);

	void Hold(const Key key, const bool down);
	void ReleaseAll();
	const set<Key> & HeldByTest();
}
