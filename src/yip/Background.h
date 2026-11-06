#pragma once
#include "Object.h"

struct Stars : Object {
	template<typename Fnc>
	void ForEach(Fnc fnc) {
		for (size_t i = 0; i < _list.getVertexCount(); i++)
		{
			fnc(_list[i]);
		}
	}

	Stars(const size_t count, const Vector2f & size);

	void Update(const sf::Time & delta) override;
	void Render(TargetStatesRef && ts) const override;

	sf::VertexArray _list;
	sf::Vector2f _size;
};

struct Background : Object
{
	Background(const size_t stars_count, const Vector2f & size = { 1000.f, (float)kResY });
	~Background();

	void Update(const sf::Time & delta) override;
	void Render(TargetStatesRef && ts) const override;

	Stars _stars, _transition_stars;
};
