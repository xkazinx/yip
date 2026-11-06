#pragma once
#include "Object.h"

struct Player : Object
{
	Player(const Vector2f & pos = { 0.f, 0.f });

	~Player();

	void Render(TargetStatesRef && ts) const override;

	sf::CircleShape _triangle, _circles[2];
	sf::Vector2f _velocity;

	void Update(const sf::Time & delta) override;
	void SetWeapon(const Weapon weapon);

	Collider GetCollider() const override;

	Weapon _weapon;

	// Enemy shots that reached it
	u16 _hits_taken = 0;

	sf::Clock _fire_timer;
};
