#pragma once

#include "ECS/system.hpp"

namespace ph {

class GUI;

namespace system {

	class HintAreas : public System
	{
	public:
		HintAreas(entt::registry&, GUI&);
		void update(float dt) override;
	private:
		GUI& mGui;
	};

}}
