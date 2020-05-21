#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class PlayerCameraMovement : public System
{
public:
	using System::System;	

	void update(float dt) override;

private:
	std::vector<entt::entity> mRoomsThatIntersectPlayer;
	entt::entity mCurrentCamRoom = entt::null;
	float mInterpolation = 0.f; // 0 - free cam  1 - room cam
};

}
