#pragma once

#include "ECS/system.hpp"

namespace ph {
	class Camera;
	class Texture;
}

namespace ph::system {

class RenderSystem : public System
{
public:
	RenderSystem(entt::registry& registry, Texture& tileset);

	void update(float dt) override;

private:
	Camera& getCameraWithTheBiggestPriority();
	void submitLights() const;
	void submitMapChunks() const;
	void submitRenderQuads() const;
	void submitRenderQuadsWithTextureRect() const;

private:
	Texture& mTilesetTexture;
};

}
