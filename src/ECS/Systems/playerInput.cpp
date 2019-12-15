#include "playerInput.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Events/actionEventManager.hpp"
#include <cmath>

namespace ph::system {

	void PlayerMovementInput::update(float seconds)
	{
		if(isPlayerWithoutControl())
			return;

		updateInputFlags();
		updateAnimationData();
		const auto playerDirection = getPlayerDirection();
		setPlayerFaceDirection(playerDirection);
		setFlashLightDirection(playerDirection);
		updateGunAttackInput();
		updateMeleeAttackInput();

		auto movementView = mRegistry.view<component::Velocity, component::CharacterSpeed, component::Player>();
		movementView.each([playerDirection](component::Velocity& velocity, const component::CharacterSpeed& speed, const component::Player) {
			const auto vel = playerDirection * speed.speed;
			velocity.dx = vel.x;
			velocity.dy = vel.y;
		});
	}

	bool PlayerMovementInput::isPlayerWithoutControl()
	{
		auto view = mRegistry.view<component::Player>();
		for(auto entity : view)
			return mRegistry.has<component::TimeToFadeOut>(entity);
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

		if (mUp   && mLeft)  return sf::Vector2f(-diagonal, -diagonal);
		if (mUp   && mRight) return sf::Vector2f(+diagonal, -diagonal);
		if (mDown && mLeft)  return sf::Vector2f(-diagonal, +diagonal);
		if (mDown && mRight) return sf::Vector2f(+diagonal, +diagonal);

		if (mUp)    return sf::Vector2f(0.f, -1.f);
		if (mDown)  return sf::Vector2f(0.f, 1.f);
		if (mLeft)  return sf::Vector2f(-1.f, 0.f);
		if (mRight) return sf::Vector2f(1.f, 0.f);

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
			constexpr float diagonal = 0.7f;

			// TODO: Try to do that with std::atan2f() function instead of if statements

			float middleAngle;
			if(face.direction == sf::Vector2f(1.f, 0.f))                  middleAngle = 0.f;
			else if(face.direction == sf::Vector2f(-1.f, 0.f))            middleAngle = 180.f;
			else if(face.direction == sf::Vector2f(0.f, 1.f))             middleAngle = 90.f;
			else if(face.direction == sf::Vector2f(0.f, -1.f))            middleAngle = -90.f;
			else if(face.direction == sf::Vector2f(diagonal, diagonal))   middleAngle = 45.f;
			else if(face.direction == sf::Vector2f(diagonal, -diagonal))  middleAngle = -45.f;
			else if(face.direction == sf::Vector2f(-diagonal, diagonal))  middleAngle = 135.f;
			else if(face.direction == sf::Vector2f(-diagonal, -diagonal)) middleAngle = -135.f;
			else middleAngle = 0.f;

			lightSource.startAngle = middleAngle - 45.f;
			lightSource.endAngle = middleAngle + 45.f;
		});
	}

	void PlayerMovementInput::updateGunAttackInput()
	{
		auto playerGunView = mRegistry.view<component::Player, component::GunAttacker>();
		if (ActionEventManager::isActionPressed("gunAttack"))
		{
			for (auto player : playerGunView)
			{
				auto& playerGunAttack = playerGunView.get<component::GunAttacker>(player);
				playerGunAttack.isTryingToAttack = true;
			}
		}
	}

	void PlayerMovementInput::updateMeleeAttackInput()
	{
		auto playerMeleeView = mRegistry.view<component::Player, component::MeleeAttacker>();
		if(ActionEventManager::isActionPressed("meleeAtack"))
		{
			for(auto player : playerMeleeView)
			{
				auto& playerMeleeAttack = playerMeleeView.get<component::MeleeAttacker>(player);
				playerMeleeAttack.isTryingToAttack = true;
			}
		}
	}
}
