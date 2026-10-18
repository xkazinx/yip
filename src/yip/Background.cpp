#include "stdafx.h"
#include "Background.h"
#include "Random.h"
#include "Camera.h"

void Background::Render(TargetStatesRef && ts) const
{
	ts().draw(_stars, ts.states);
	ts().draw(_transition_stars, ts.states);
}

Background::Background(const size_t stars_count, const Vector2f & size)
	: Object({ 0.f, 0.f }, ObjectID::Background),
	_stars(stars_count, size),
	_transition_stars(200, Vector2f(kResX, size.y))
{
	_transition_stars.setPosition({ size.x, 0.f });
}

Background::~Background()
{
}

// Two strips of stars leapfrog each other as the camera moves right
void Background::Update(const sf::Time & delta)
{
	if (g_camera->GetLeft() >= _stars.getPosition().x + _stars._size.x)
		_stars.move({ _stars._size.x + _transition_stars._size.x, 0.f });

	if (g_camera->GetRight() >= _transition_stars.getPosition().x + 2 * _transition_stars._size.x)
		_transition_stars.move({ _stars._size.x + _transition_stars._size.x, 0.f });
}

Stars::Stars(const size_t count, const Vector2f & size)
	: Object({ 0.f, 0.f }, ObjectID::Star), _list(sf::PrimitiveType::Points, count), _size(size)
{
	ForEach([&](auto & star) {
		star.color = sf::Color::White;

		star.position = Vector2f{
			g_rand->Generate<float>(0.f, _size.x),
			g_rand->Generate<float>(0.f, _size.y)
		};
	});
}

void Stars::Update(const sf::Time & delta)
{
}

void Stars::Render(TargetStatesRef && ts) const
{
	ts().draw(_list, ts.states);
}
