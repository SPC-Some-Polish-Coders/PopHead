#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class CollisionDebug : public System
{
public:
	using System::System;

	void update(float dt) override;

	static void setIsCollisionDebugActive(bool isActive) { sIsCollisionDebugActive = isActive; }

private:
	inline static bool sIsCollisionDebugActive = false;
};

}
