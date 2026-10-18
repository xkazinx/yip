#include "stdafx.h"
#include "Player.h"
#include "Camera.h"
#include "SGame.h"
#include "SceneMgr.h"
#include "PNormal.h"
#include "Input.h"

Player::Player(const Vector2f & pos)
	: Object(pos, ObjectID::Player),
	_triangle(20.f, 3),
	_weapon(Weapon::Normal)
{
	_triangle.setOrigin({ _triangle.getRadius(), _triangle.getRadius() });
	_triangle.rotate(sf::degrees(90.f));

	_circles[0].setFillColor(sf::Color::Red);
	_circles[0].setRadius(10.f);
	_circles[0].setPosition({ -18.f, -25.f });

	_circles[1].setFillColor(sf::Color::Red);
	_circles[1].setRadius(10.f);
	_circles[1].setPosition({ -18.f, 5.f });
}

Player::~Player()
{
}

void Player::Render(TargetStatesRef && ts) const
{
	if (_weapon != Weapon::Normal)
	{
		ts().draw(_circles[0], ts.states);
		ts().draw(_circles[1], ts.states);
	}

	ts().draw(_triangle, ts.states);
}

Object::Collider Player::GetCollider() const
{
	const auto rect = _triangle.getGlobalBounds();
	return sf::FloatRect(rect.position + getPosition(), rect.size);
}

void Player::Update(const sf::Time & delta)
{
	// A: fire, every 100 ms while held
	if (input::Held(Key::A))
	{
		if (_fire_timer.getElapsedTime().asMilliseconds() >= 100)
		{
			_fire_timer.restart();

			const auto pos = getPosition();
			switch (_weapon)
			{
			case Weapon::Triple:
				for (u8 i = 0; i < 3; i++)
				{
					g_game->_projectiles += mk<PNormal>(sf::Vector2f{ pos.x, pos.y - 15.f + (i * 15.f) });
				}
				break;

			// Laser and Dot have no shot of their own yet: they fire the normal one
			default:
				g_game->_projectiles += mk<PNormal>(pos);
				break;
			}
		}
	}

	constexpr auto camera_velocity = .05f;
	Vector2f velocity(camera_velocity, 0.f);

	// Idle velocity
	velocity.x *= -1.f / 3.f;

	// velocity.x
	if (input::Held(Key::Right) ||
		input::Held(Key::Left))
	{
		auto direction = input::Held(Key::Left) ? -1 : 1;
		velocity.x += 5 * camera_velocity * direction;
	}

	// velocity.y
	if (input::Held(Key::Up) ||
		input::Held(Key::Down))
	{
		auto direction = input::Held(Key::Up) ? -1 : 1;
		velocity.y += 0.3f * direction;
	}

	// Player bounds
	const sf::FloatRect bounds(
		{ 50.f + g_camera->GetLeft(), g_camera->GetTop() },
		{ g_camera->getSize().x - 0.2f * g_camera->getSize().x, g_camera->getSize().y }
	);

	// A copy: getPosition() is a reference that move() changes
	const auto current_pos = getPosition();
	move(DeltaVelocity(velocity, delta));
	if (!bounds.contains(getPosition()))
	{
		auto pos = getPosition();

		if (pos.x < bounds.position.x)
			pos.x = bounds.position.x + 1.f;

		if (pos.x > bounds.position.x + bounds.size.x)
			pos.x = current_pos.x;

		if (pos.y < bounds.position.y)
			pos.y = bounds.position.y + 1.f;
		else if (pos.y > bounds.position.y + bounds.size.y)
			pos.y = bounds.position.y + bounds.size.y - 1.f;

		setPosition(pos);
	}
}

void Player::SetWeapon(const Weapon weapon)
{
	_weapon = weapon;
	switch (weapon)
	{
	case Weapon::Normal:
		break;

	case Weapon::Laser:
		for (auto & circle : _circles)
		{
			circle.setFillColor(sf::Color::Green);
		}
		break;

	case Weapon::Triple:
		for (auto & circle : _circles)
		{
			circle.setFillColor(sf::Color::Blue);
		}
		break;

	default:
		break;
	}
}
