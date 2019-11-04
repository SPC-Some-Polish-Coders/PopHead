#pragma once

#include "ECS/system.hpp"
#include "Renderer/camera.hpp"

namespace sf {
	class Window;	
}

namespace ph::system {

class RenderSystem : public System
{
public:
	RenderSystem(entt::registry& registry, sf::Window& window);

	void update(float seconds) override;
private:
	Camera mCamera;
	sf::Window& mWindow;
};

}
