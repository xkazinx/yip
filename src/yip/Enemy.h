#pragma once
#include "Object.h"

struct Enemy : Object
{
	Enemy(const Vector2f & pos = { 0.f, 0.f });
	~Enemy();

	void Render(TargetStatesRef && ts) const override {
		ts().draw(_shape, ts.states);
	}

	sf::RectangleShape _shape;
	sf::Vector2f _velocity;

	void Update(const sf::Time & delta) override;

	Collider GetCollider() const override;

	void HandleCollition(const Object * object) override;

	sf::Clock _fire_timer;
	bool _HasExpired() const override;
};
