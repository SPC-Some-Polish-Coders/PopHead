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

		void update(float dt) override;
		void onEvent(Event) override;

	private:
		bool isPlayerWithoutControl();
		sf::Vector2f getPlayerDirection() const;
		void updateInputFlags();
		void updateAnimationData();
		void setPlayerFaceDirection(const sf::Vector2f faceDirection) const;
		void setFlashLightDirection(const sf::Vector2f faceDirection) const;

	private:
		AIManager& mAIManager;
		Scene* mScene;
		bool mUp    = false;
		bool mDown  = false;
		bool mLeft  = false;
		bool mRight = false;
	};
}

}
