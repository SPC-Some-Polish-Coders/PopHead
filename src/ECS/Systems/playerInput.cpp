#include "playerInput.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

#include "Events/actionEventManager.hpp"

#include <cmath>

namespace ph::system {

	void PlayerMovementInput::update(float seconds)
	{
		const auto playerDirection = getPlayerDirection();

		auto movementView = mRegistry.view<component::Velocity, component::CharacterSpeed, component::Player>();
		movementView.each([playerDirection](component::Velocity& velocity, const component::CharacterSpeed& speed, component::Player) {
			
			const auto vel = playerDirection * speed.speed;
			velocity.dx = vel.x;
			velocity.dy = vel.y;
			});
	}

	sf::Vector2f PlayerMovementInput::getPlayerDirection() const
	{
		bool up    = ActionEventManager::isActionPressed("movingUp");
		bool down  = ActionEventManager::isActionPressed("movingDown");
		bool left  = ActionEventManager::isActionPressed("movingLeft");
		bool right = ActionEventManager::isActionPressed("movingRight");

		const float diagonal = std::sqrt(2.f) / 2.f;

		if (up   && left)  return sf::Vector2f(-diagonal, -diagonal);
		if (up   && right) return sf::Vector2f(+diagonal, -diagonal);
		if (down && left)  return sf::Vector2f(-diagonal, +diagonal);
		if (down && right) return sf::Vector2f(+diagonal, +diagonal);

		if (up)    return sf::Vector2f(0.f, -1.f);
		if (down)  return sf::Vector2f(0.f, 1.f);
		if (left)  return sf::Vector2f(-1.f, 0.f);
		if (right) return sf::Vector2f(1.f, 0.f);

		return sf::Vector2f(0.f, 0.f);
	}


	void PlayerAttackType::update(float seconds)
	{
		auto playerMeleeView = mRegistry.view<component::Player, component::MeleeAttacker>();
		auto playerGunView= mRegistry.view<component::Player, component::GunAttacker>();

		if (ActionEventManager::isActionPressed("meleeAtack"))
		{
			for (auto player : playerMeleeView)
			{
				auto& playerMeleeAttack = playerMeleeView.get<component::MeleeAttacker>(player);
				playerMeleeAttack.isTryingToAttack = true;
			}
		}

		if (ActionEventManager::isActionPressed("gunAttack"))
		{
			for (auto player : playerGunView)
			{
				auto& playerGunAttack = playerGunView.get<component::GunAttacker>(player);
				playerGunAttack.isTryingToAttack = true;
			}
		}
	}

}
