#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class EntitiesDebugger : public System
{
public:
	using System::System;

	void update(float dt) override;

private:
	entt::entity mSelected = entt::null;
};

}
