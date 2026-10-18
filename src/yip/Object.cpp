#include "stdafx.h"
#include "Object.h"

Object::~Object()
{
}

#ifdef DEBUG_MODE
void Object::_DebugRender(TargetStatesRef && ts) const
{
	static unique_ptr<sf::CircleShape> _origin;
	if (!_origin) {
		_origin = mk<sf::CircleShape>(1.f);
		_origin->setFillColor(sf::Color::Red);
	}

	_origin->setPosition(getPosition() + getOrigin());
	ts.target.draw(*_origin.get());

	const Collider collider = GetCollider();

	static unique_ptr<sf::RectangleShape> _collider;
	if (!_collider && collider) {
		_collider = mk<sf::RectangleShape>();
		_collider->setFillColor(sf::Color::Transparent);
		_collider->setOutlineColor(sf::Color::Red);
		_collider->setOutlineThickness(2.f);
	}

	if (collider) {
		_collider->setPosition(collider->position);
		_collider->setSize(collider->size);

		ts.target.draw(*_collider.get(), ts.states);
	}
}
#endif
