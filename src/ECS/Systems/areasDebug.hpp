#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class AreasDebug : public System
{
public:
	using System::System;

	void update(float dt) override;

	static void setIsCollisionDebugActive(bool isActive) { sIsCollisionDebugActive = isActive; }
	static void setIsVelocityChangingAreaDebugActive(bool isActive) { sIsVelocityChangingAreaDebugActive = isActive; }
	static void setIsPushingAreaDebugActive(bool isActive) { sIsPushingAreaDebugActive = isActive; }
	static void setIsLightWallsAreaDebugActive(bool isActive) { sIsLightWallsAreaDebugActive = isActive; }

private:
	inline static bool sIsCollisionDebugActive = false;
	inline static bool sIsVelocityChangingAreaDebugActive = false;
	inline static bool sIsPushingAreaDebugActive = false;
	inline static bool sIsLightWallsAreaDebugActive = false;
};

}
