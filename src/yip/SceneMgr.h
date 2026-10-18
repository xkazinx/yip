#pragma once
#include <scene/manager.h>

class Scene;

// yip's scenes, one per scene::ID, switched by kazilib's scene manager
class SceneMgr : public sf::Drawable
{
public:
	using ID = ::scene::ID;

	SceneMgr();
	~SceneMgr();

	array<unique_ptr<Scene>, enum_count<ID>()> _list;

	auto & At(const ID id)		 { return _list[Under(id)]; }
	auto & At(const ID id) const { return _list[Under(id)]; }

	void Set(const ID id);
	Scene * Current() const;

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	void Update(const sf::Time & delta);
	void HandleEvent(const sf::Event & event);

private:
	lib::scene::Manager _mgr;
};
