#pragma once

class Scene;

class Camera : public sf::View, public sf::Drawable
{
private:
	const Scene * _scene = nullptr;

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

public:
	Camera() : sf::View({ kResX / 2.f, kResY / 2.f }, { (float)kResX, (float)kResY }) {
	}
	~Camera();

	void BindScene(const Scene * scene) {
		_scene = scene;
	}

	auto GetLeft() const {
		return getCenter().x - getSize().x / 2.f;
	}

	auto GetRight() const {
		return getCenter().x + getSize().x / 2.f;
	}

	auto GetTop() const {
		return getCenter().y - getSize().y / 2.f;
	}

	auto GetBottom() const {
		return getCenter().y + getSize().y / 2.f;
	}
};
