#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class PlayerCameraMovement : public System
{
public:
	PlayerCameraMovement(entt::registry& registry);

	void update(float dt) override;
};

}
