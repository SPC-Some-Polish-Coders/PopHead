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

		void onEvent(Event) override;
		void update(float dt) override;

	private:
		AIManager& mAIManager;
		Scene* mScene;

		float mTimeFromDashPressed; 

		bool mUp    = false;
		bool mDown  = false;
		bool mLeft  = false;
		bool mRight = false;
	};
}

}
