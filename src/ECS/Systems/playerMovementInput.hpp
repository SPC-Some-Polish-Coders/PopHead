#pragma once

#include "ECS/system.hpp"
#include <SFML/System/Vector2.hpp>

namespace ph {

class AIManager;
class Scene;

namespace system {

	class PlayerMovementInput : public System
	{
	public:
		PlayerMovementInput(entt::registry&, AIManager&, Scene*);

		void onEvent(sf::Event) override;
		void update(float dt) override;

	private:
		AIManager& mAIManager;
		Scene* mScene;

		float mTimeFromDodgePressed = 1.f; 

	public:
		inline static bool dodgeInputDisabled = false;
	};
}

}
