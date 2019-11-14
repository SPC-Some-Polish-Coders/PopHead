#pragma once

#include "pendingMeleeAttacks.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

#include "Events/actionEventManager.hpp"

namespace ph::system {

	void PendingMeleeAttacks::update(float seconds)
	{
		const auto meleeAttackerView = mRegistry.view<component::MeleeAttacker, component::BodyRect, component::Player>();
		for (const auto& meleeAttacker : meleeAttackerView)
		{
			auto& playerMeleeAttack = meleeAttackerView.get<component::MeleeAttacker>(meleeAttacker);

			bool cooldown = hasCooldown(playerMeleeAttack.cooldownSinceLastHit);
			if (cooldown)
				playerMeleeAttack.cooldownSinceLastHit -= seconds;

			if (playerMeleeAttack.isTryingToAttack)
			{
				playerMeleeAttack.isTryingToAttack = false;

				setPlayerFacePosition();
				const auto& playerBody = meleeAttackerView.get<component::BodyRect>(meleeAttacker);

				//performHit(playerBody);

				playerMeleeAttack.cooldownSinceLastHit = playerMeleeAttack.minSecondsInterval;
			}
		}
	}

	void PendingMeleeAttacks::setPlayerFacePosition()
	{
		const auto playerView = mRegistry.view<component::FaceDirection, component::Player>();
		for (auto player : playerView)
		{
			const auto& playerFaceDirection = playerView.get<component::FaceDirection>(player);
			mPlayerFaceDirection = playerFaceDirection.direction;
		}
	}

	bool PendingMeleeAttacks::hasCooldown(float cooldownSinceLastHit) const
	{
		return cooldownSinceLastHit > 0.f;
	}

	void PendingMeleeAttacks::performHit(const sf::Vector2f& startingWeaponPosition)
	{
		auto enemies = mRegistry.view<component::BodyRect, component::Killable>(entt::exclude<component::Player>);
		
	}

}
