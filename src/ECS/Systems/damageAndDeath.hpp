#pragma once

#include "ECS/system.hpp"

namespace ph {
	class GUI;
}

namespace ph::system {

	class DamageAndDeath : public System
	{
	public:
		DamageAndDeath(entt::registry&, GUI&);

		void update(float dt) override;

	private:
		void dealDamage() const;
		void makeDamageJuice(float dt) const; // NOTE: Juice is a game design term
		void makeCharactersDie() const;
		void updateDeadCharacters(float dt) const;

	private:
		GUI& mGui;
	};
}
