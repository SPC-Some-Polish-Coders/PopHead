#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"

namespace ph {
	class Texture;
}

namespace ph::system {

class RenderSystem : public System
{
public:
	RenderSystem(entt::registry& registry, Texture& tileset);

	void update(float dt) override;
};

}
