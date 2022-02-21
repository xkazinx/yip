#include "stdafx.h"
#include "PDot.h"
#include "Camera.h"
#include <math.h>

PDot::PDot(const Vector2f& pos, const Vector2f& end_pos) :
	Object(pos, ObjectID::PDot),
	_initial_pos(pos), _end_pos(end_pos)
{
	_shape.setRadius(3.f);
	_shape.setFillColor(sf::Color::Red);

	cout << _end_pos.x << "/" << _end_pos.y << "  " << pos.x << "/" << pos.y << endl;
	auto vec = _end_pos - pos;
	_angle = atan2(vec.y, vec.x);
}

void PDot::Render(TargetStatesRef&& ts) const
{
	ts().draw(_shape, ts.states);
}

void PDot::Update(const sf::Time& delta)
{
	constexpr auto x_vel = .30f + .5f; //.5f = camera movement
	constexpr auto y_vel = .30f + .5f;
	
	move(cos(_angle) * x_vel, sin(_angle) * y_vel);
}

Object::Collider PDot::GetCollider() const
{
	auto & pos = getPosition();
	auto rect = _shape.getGlobalBounds();
	return sf::FloatRect(rect.left + pos.x, rect.top + pos.y, rect.width, rect.height);
}

void PDot::HandleCollition(const Object * object) {
	if (object->Is(ObjectID::Player))
	{
		Expire();
	}
}

bool PDot::_HasExpired() const
{
	return getPosition().x <= g_camera->GetLeft() || getPosition().x >= g_camera->GetRight();
}