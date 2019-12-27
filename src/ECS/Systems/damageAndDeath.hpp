#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class DamageAndDeath : public System
	{
	public:
		using System::System;

		void update(float dt) override;

	private:
		void dealDamage() const;
		void makeDamageJuice(float dt) const; // NOTE: Juice is a game design term
		void makeCharactersDie() const;
		void makeCorpsesFadeOut(float dt) const;
	};
}
