#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class IsInPlayerAttackArea : public System
	{
	public:
		using System::System;

		void update(float dt) override;

	private:
		void handleGun() const;
		void handleMelee() const;
	};
}
