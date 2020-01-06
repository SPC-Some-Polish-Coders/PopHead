#pragma once

#include "ECS/system.hpp"

namespace ph {
	class GUI;
	class AIManager;
}

namespace ph::system {

	class DamageAndDeath : public System
	{
	public:
		DamageAndDeath(entt::registry&, GUI&, AIManager&);

		void update(float dt) override;

	private:
		void dealDamage() const;
		void makeDamageJuice(float dt) const; // NOTE: Juice is a game design term
		void makeCharactersDie();
		void updateDeadCharacters(float dt);

	private:
		GUI& mGui;
		AIManager& mAIManager;
		unsigned char mLastDeadBodyZ = 170;
	};
}
