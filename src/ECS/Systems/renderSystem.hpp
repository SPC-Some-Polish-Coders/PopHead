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

#ifndef PH_DISTRIBUTION
private:
	// debug imgui stuff
	struct DebugLayer
	{
		char name[50] = {};
		bool turnOn = true;
	};
	std::vector<DebugLayer> mDebugLayers;
	bool mDebugLayersInitialized = false;
#endif
};

}
