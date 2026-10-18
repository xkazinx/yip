#pragma once
#include "Scene.h"

class SGame : public Scene
{
public:
	SGame();
	~SGame();

	void Reset() override;
	void Update(const sf::Time & delta) override;
	void Render(TargetStatesRef && ts) const override;
	void HandleEvent(const sf::Event & event) override;

	// An enemy off the right of the screen at a random height
	void SpawnEnemy();

	vector<unique_ptr<Object>> _objects;
	vector<unique_ptr<Object>> _projectiles;

	u16 _projectiles_hit = 0, _projectiles_missed = 0,
		_enemies_killed = 0, _enemies_missed = 0;
};
