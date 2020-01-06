#pragma once

#include "ECS/system.hpp"
#include <SFML/System/Vector2.hpp>

namespace ph {

class AIManager;
class GUI;
class Scene;

namespace system {

	class PlayerMovementInput : public System
	{
	public:
		PlayerMovementInput(entt::registry&, AIManager&, GUI& gui, Scene*);

		void update(float dt) override;
		void onEvent(const ActionEvent& event) override;

	private:
		bool isPlayerWithoutControl();
		sf::Vector2f getPlayerDirection() const;
		void updateInputFlags();
		void updateAnimationData();
		void setPlayerFaceDirection(const sf::Vector2f faceDirection) const;
		void setFlashLightDirection(const sf::Vector2f faceDirection) const;

	private:
		AIManager& mAIManager;
		GUI& mGui;
		Scene* mScene;
		bool mUp    = false;
		bool mDown  = false;
		bool mLeft  = false;
		bool mRight = false;
	};
}

}
