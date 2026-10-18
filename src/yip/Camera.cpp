#include "stdafx.h"
#include "Camera.h"
#include "Scene.h"

void Camera::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (_scene)
		target.draw(*_scene);
}

Camera::~Camera()
{
}
