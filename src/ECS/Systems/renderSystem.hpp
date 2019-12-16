#pragma once

#include "ECS/system.hpp"

namespace sf {
	class Window;	
}
namespace ph {
	class Camera;
}

namespace ph::system {

class RenderSystem : public System
{
public:
	RenderSystem(entt::registry& registry, sf::Window& window);

	void update(float dt) override;

private:
	Camera& getCameraWithTheBiggestPriority();
	void submitLights() const;
	void submitRenderQuads() const;
	void submitRenderQuadsWithTextureRect() const;

private:
	sf::Window& mWindow;
};

}
