#include "stdafx.h"
#include "Input.h"

namespace input
{
	namespace
	{
		set<Key> g_held;
	}

	bool Held(const Key key)
	{
		if (g_held.contains(key))
			return true;

		return GetWindow().hasFocus() && sf::Keyboard::isKeyPressed(key);
	}

	void Hold(const Key key, const bool down)
	{
		if (down)
			g_held.insert(key);
		else
			g_held.erase(key);
	}

	void ReleaseAll()
	{
		g_held.clear();
	}

	const set<Key> & HeldByTest()
	{
		return g_held;
	}
}
