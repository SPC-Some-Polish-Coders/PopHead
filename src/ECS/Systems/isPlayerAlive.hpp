#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class IsPlayerAlive : public System
	{
	public:
		using System::System;

		void update(float seconds) override;

		bool isAlive();

	private:
		bool mIsAlive;
	};
}
