#include "ECS/system.hpp"

namespace ph::system {

class DebugCamera : public System
{
public:
	using System::System;

	void update(float dt) override;

private:
	float mZoom = 1.f;
	float mMovementSpeed = 1.f;
	bool mDebugCameraEnabled = false;
};

}

