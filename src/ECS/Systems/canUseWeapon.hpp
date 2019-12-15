#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class CanUseWeapon : public System
	{
	public:
		using System::System;

		void update(float seconds) override;

	private:
		void updateMeleeWeapon(float dt);
		void updateGun(float dt);
	};

}
