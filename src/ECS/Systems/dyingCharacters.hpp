#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class DyingCharacters : public System
	{
	public:
		using System::System;

		void update(float seconds) override;

	private:
		void makeCharactersDie() const;
		void playDyingAnimation(float seconds) const;
	};
}