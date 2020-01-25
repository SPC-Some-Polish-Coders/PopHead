#include "playerMovementInput.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Events/actionEventManager.hpp"
#include "AI/aiManager.hpp"
#include "Scenes/scene.hpp"
#include "GUI/gui.hpp"
#include "Utilities/direction.hpp"
#include "Utilities/profiling.hpp"
#include <cmath>

namespace ph::system {

	PlayerMovementInput::PlayerMovementInput(entt::registry& registry, AIManager& aiManager, Scene* scene)
		:System(registry)
		,mScene(scene)
		,mAIManager(aiManager)
	{
	}

	void PlayerMovementInput::update(float dt)
	{
		PH_PROFILE_FUNCTION(0);

		if(isPlayerWithoutControl())
			return;

		updateInputFlags();
		updateAnimationData();
		const auto playerDirection = getPlayerDirection();
		setPlayerFaceDirection(playerDirection);
		setFlashLightDirection(playerDirection);

		auto movementView = mRegistry.view<component::Player, component::Velocity, component::CharacterSpeed, component::BodyRect>();
		movementView.each([this, playerDirection]
		(const component::Player, component::Velocity& velocity, const component::CharacterSpeed& speed, const component::BodyRect& body) 
		{
			mAIManager.setPlayerPosition(body.rect.getTopLeft());

			const auto vel = playerDirection * speed.speed;
			velocity.dx = vel.x;
			velocity.dy = vel.y;
		});
	}

	void PlayerMovementInput::onEvent(const ActionEvent& event)
	{
		if(event.mType == ActionEvent::Type::Pressed)
		{
			// TODO_states: Pause screen could be handled by states
			if(event.mAction == "pauseScreen") 
			{
				auto players = mRegistry.view<component::Player, component::Health>();
				players.each([this](component::Player, component::Health) {
					const bool pause = mScene->getPause();
					pause ? GUI::hideInterface("pauseScreen") : GUI::showInterface("pauseScreen");
					mScene->setPause(!pause);
				});
			}
		}
	}

	bool PlayerMovementInput::isPlayerWithoutControl()
	{
		auto view = mRegistry.view<component::Player>();
		for(auto entity : view)
			return mRegistry.has<component::DeadCharacter>(entity);
		return false;
	}

	void PlayerMovementInput::updateInputFlags()
	{
		mUp    = ActionEventManager::isActionPressed("movingUp");
		mDown  = ActionEventManager::isActionPressed("movingDown");
		mLeft  = ActionEventManager::isActionPressed("movingLeft");
		mRight = ActionEventManager::isActionPressed("movingRight");
	}

	void PlayerMovementInput::updateAnimationData()
	{
		auto view = mRegistry.view<component::Player, component::AnimationData>();
		for(auto& entity : view)
		{
			auto& animationData = view.get<component::AnimationData>(entity);

			if(mUp && mLeft) {
				animationData.currentStateName = "leftUp";
				animationData.isPlaying = true;
			}
			else if(mUp && mRight) {
				animationData.currentStateName = "rightUp";
				animationData.isPlaying = true;
			}
			else if(mLeft) {
				animationData.currentStateName = "left";
				animationData.isPlaying = true;
			}
			else if(mRight) {
				animationData.currentStateName = "right";
				animationData.isPlaying = true;
			}
			else if(mUp) {
				animationData.currentStateName = "up";
				animationData.isPlaying = true;
			}
			else if(mDown) {
				animationData.currentStateName = "down";
				animationData.isPlaying = true;
			}
			else {
				animationData.isPlaying = false;
			}
		}
	}

	sf::Vector2f PlayerMovementInput::getPlayerDirection() const
	{
		constexpr float diagonal = 0.7f;

		if (mUp   && mLeft)  return PH_NORTH_WEST;
		if (mUp   && mRight) return PH_NORTH_EAST;
		if (mDown && mLeft)  return PH_SOUTH_WEST;
		if (mDown && mRight) return PH_SOUTH_EAST;

		if (mUp)    return PH_NORTH;
		if (mDown)  return PH_SOUTH;
		if (mLeft)  return PH_WEST;
		if (mRight) return PH_EAST;

		return sf::Vector2f(0.f, 0.f);
	}

	void PlayerMovementInput::setPlayerFaceDirection(const sf::Vector2f faceDirection) const
	{
		auto playerView = mRegistry.view<component::Player, component::FaceDirection>();
		for (auto player : playerView)
		{
			if (faceDirection != sf::Vector2f(0.f, 0.f))
			{
				auto& prevFaceDirection = playerView.get<component::FaceDirection>(player);
				prevFaceDirection.direction = faceDirection;
			}
		}
	}

	void PlayerMovementInput::setFlashLightDirection(const sf::Vector2f faceDirection) const
	{
		auto view = mRegistry.view<component::Player, component::FaceDirection, component::LightSource>();
		view.each([this](const component::Player, const component::FaceDirection face, component::LightSource& lightSource) 
		{
			// TODO: Try to do that with std::atan2f() function instead of if statements

			float middleAngle;
			if(face.direction == PH_EAST)            middleAngle = 0.f;
			else if(face.direction == PH_WEST)       middleAngle = 180.f;
			else if(face.direction == PH_SOUTH)      middleAngle = 90.f;
			else if(face.direction == PH_NORTH)      middleAngle = -90.f;
			else if(face.direction == PH_SOUTH_EAST) middleAngle = 45.f;
			else if(face.direction == PH_NORTH_EAST) middleAngle = -45.f;
			else if(face.direction == PH_SOUTH_WEST) middleAngle = 135.f;
			else if(face.direction == PH_NORTH_WEST) middleAngle = -135.f;
			else middleAngle = 0.f;

			lightSource.startAngle = middleAngle - 35.f;
			lightSource.endAngle = middleAngle + 35.f;
		});
	}
}

