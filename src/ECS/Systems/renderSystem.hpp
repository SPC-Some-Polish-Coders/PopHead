#pragma once

#include "ECS/system.hpp"

namespace ph {
	class Camera;
}

namespace ph::system {

class RenderSystem : public System
{
public:
	RenderSystem(entt::registry& registry);

	void update(float dt) override;

private:
	Camera& getCameraWithTheBiggestPriority();
	void submitLights() const;
	void submitRenderQuads() const;
	void submitRenderQuadsWithTextureRect() const;
};

}
