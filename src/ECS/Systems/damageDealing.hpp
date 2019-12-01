#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class DamageDealing : public System
	{
	public:
		using System::System;

		void update(float seconds) override;

	private:
		void dealDamage() const;
		void playDamageAnimation(float seconds) const;
	};
}
