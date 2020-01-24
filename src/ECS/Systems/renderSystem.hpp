#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"

namespace ph {
	class Camera;
	class Texture;
}

namespace ph::system {

class RenderSystem : public System
{
public:
	RenderSystem(entt::registry& registry, Texture& tileset);

	void updateGraphics() override;

private:
	Texture& mTilesetTexture;
};

}
