#include "stdafx.h"
#include "Scene.h"
#include "UIComponent.h"
#include "Button.h"
#include "Camera.h"

void Scene::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.setView(target.getDefaultView());
	for (auto & component : _ui_components) {
		target.draw(*component, states);
	}

	target.setView(*g_camera.get());

	Render({ target, states });
}

Scene::Scene(const ID id) : _id(id)
{
}

Scene::~Scene()
{
}

void Scene::Initialize()
{
	g_camera->setCenter({ kResX / 2.f, kResY / 2.f });
	Reset();
}

void Scene::HandleEvent(const void * event)
{
	if (event)
		HandleEvents(*static_cast<const sf::Event *>(event));
}

void Scene::HandleEvents(const sf::Event & event)
{
	for (auto & component : _ui_components) {
		if (!component->HandleEvent(event))
			return;
	}

	HandleEvent(event);
}
