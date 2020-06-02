#pragma once

#include "ECS/system.hpp"

namespace ph {
	class AIManager;
}

namespace ph::system {

class DamageAndDeath : public System
{
public:
	DamageAndDeath(entt::registry&, AIManager&);

	void update(float dt) override;

private:
	void dealDamage() const;
	void makeDamageJuice(float dt) const; // NOTE: Juice is a game design term
	void makeCharactersDie();
	void updateDeadCharacters(float dt);

private:
	AIManager& mAIManager;
	u8 mLastDeadBodyZ = 170;
};

}
