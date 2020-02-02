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

		float mTimeFromLastUp    = 1.f;
		float mTimeFromLastDown  = 1.f;
		float mTimeFromLastLeft  = 1.f;
		float mTimeFromLastRight = 1.f;

		float mTimeFromDashBegining= 1.f;

		bool mUp    = false;
		bool mDown  = false;
		bool mLeft  = false;
		bool mRight = false;
	};
}

}
