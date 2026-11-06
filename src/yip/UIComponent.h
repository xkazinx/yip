#pragma once

struct UIComponent : sf::Drawable
{
	UIComponent();
	~UIComponent();

	virtual bool HandleEvent(const sf::Event & event) = 0;
};

