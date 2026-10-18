#include "stdafx.h"
#include "Button.h"
#include "TextMgr.h"

Button::Button(EventFunction && click_func, const sf::Vector2f & pos, const string & str)
	: _text(g_text->GetFont(font::ID::Arial), str), _click_func(move(click_func))
{
	_text.setPosition(pos);
	_text.setFillColor(sf::Color::White);

	_rect = sf::FloatRect(
		pos,
		{ _text.getLocalBounds().size.x, (float)_text.getCharacterSize() + _text.getLocalBounds().position.y }
	);
}

Button::~Button()
{

}

bool Button::HandleEvent(const sf::Event & event)
{
	const auto released = event.getIf<sf::Event::MouseButtonReleased>();
	if (released && _rect.contains(Vector2f(released->position))) {
		_click_func(event);
		return false;
	}
	return true;
}
