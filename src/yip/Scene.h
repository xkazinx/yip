#pragma once
#include "Object.h"
#include <scene/manager.h>

struct UIComponent;

// A scene on kazilib's scene manager (scene/manager.h): entering it (Initialize)
// puts the camera back at the start and resets the scene
struct Scene : lib::scene::Base, sf::Drawable
{
	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	using ID = ::scene::ID;

	Scene(const ID id);
	~Scene();

	void Initialize() override;
	void HandleEvent(const void * event) override;

	virtual void Render(TargetStatesRef && ts) const = 0;

	virtual void Update(const sf::Time & delta) = 0;

	virtual void Reset() = 0;

	void HandleEvents(const sf::Event & event);

	virtual void HandleEvent(const sf::Event & event) = 0;

	ID _id;

	vector<UIComponent *> _ui_components;

	template<typename T>
	unique_ptr<T> AddComponent(unique_ptr<T> && component)
	{
		_ui_components += component.get();
		return move(component);
	}
};
