#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class PuzzleBoulders : public System
	{
	public:
		using System::System;

		void update(float dt) override;

	private:
		float mTimeSincePlayerIsPushingBoulder = 1.f;
	};
}
