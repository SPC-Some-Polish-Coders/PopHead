#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class MeleeAttacks : public System
{
public:
	using System::System;

	void onEvent(sf::Event)override;
	void update(float dt) override;

private:
	float mStartWeaponRotation;
	bool mIsAttackButtonPressed = false;
	bool mShouldWeaponBeRendered = false;

public:
	inline static bool inputDisabled = false;
};

}

