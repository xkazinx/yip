#pragma once

struct Object : sf::Drawable, sf::Transformable
{
	bool _has_expired = false;
	virtual bool _HasExpired() const { return false; }

	void _DebugRender(TargetStatesRef && ts) const;

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override final
	{
		states.transform *= getTransform();
		Render({ target, states });

#ifdef DEBUG_MODE
		states.transform *= getInverseTransform();
		_DebugRender({ target, states });
#endif
	}

	void Expire() { _has_expired = true; }

	Object(const Vector2f & pos = { 0.f, 0.f }, const ObjectID id = ObjectID::None)
		: _id(id)
	{
		setPosition(pos);
	}

	virtual ~Object();

	virtual void Render(TargetStatesRef && ts) const = 0;

	virtual void Update(const sf::Time & delta) = 0;

	virtual bool HasExpired() const { return _HasExpired() || _has_expired; }

	virtual bool CollidesWith(const Object * object) const {
		const Collider mine = GetCollider();
		const Collider theirs = object->GetCollider();

		if (!mine || !theirs)
		{
			return false;
		}

		return mine->findIntersection(*theirs).has_value();
	}

	virtual void HandleCollition(const Object * object) {}

	using Collider = optional<sf::FloatRect>;
	virtual Collider GetCollider() const {
		return std::nullopt;
	}

	const ObjectID _id;
	auto Is(const ObjectID id) const { return _id == id; }

	// Hit something it counts against (a shot an enemy, an enemy a shot): the score
	bool _hit = false;

	template<typename T>
		requires is_base_of_v<Object, T>
	T * Get()
	{
		return dynamic_cast<T *>(this);
	}
};
