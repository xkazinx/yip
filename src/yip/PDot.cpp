#include "stdafx.h"
#include "PDot.h"
#include "Camera.h"

PDot::PDot(const Vector2f & pos, const Vector2f & end_pos) :
	Object(pos, ObjectID::PDot),
	_initial_pos(pos), _end_pos(end_pos)
{
	_shape.setRadius(3.f);
	_shape.setOrigin({ 3.f, 3.f });
	_shape.setFillColor(sf::Color::Red);

	const auto vec = _end_pos - pos;
	_angle = std::atan2(vec.y, vec.x);
}

void PDot::Render(TargetStatesRef && ts) const
{
	ts().draw(_shape, ts.states);
}

void PDot::Update(const sf::Time & delta)
{
	// Its own speed along the aim, plus the camera's scroll (0.05 px/ms, SGame) on x:
	// the aim was taken on screen, where the player stays while the world scrolls
	constexpr auto speed = .30f;
	constexpr auto camera_velocity = .05f;

	move(DeltaVelocity({ std::cos(_angle) * speed + camera_velocity, std::sin(_angle) * speed }, delta));
}

Object::Collider PDot::GetCollider() const
{
	const auto rect = _shape.getGlobalBounds();
	return sf::FloatRect(rect.position + getPosition(), rect.size);
}

void PDot::HandleCollition(const Object * object) {
	if (object->Is(ObjectID::Player))
	{
		_hit = true;
		Expire();
	}
}

bool PDot::_HasExpired() const
{
	const auto & pos = getPosition();
	return pos.x <= g_camera->GetLeft() || pos.x >= g_camera->GetRight()
		|| pos.y <= g_camera->GetTop() || pos.y >= g_camera->GetBottom();
}
