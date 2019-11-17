#pragma once

#include "pendingMeleeAttacks.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/math.hpp"

namespace ph::system {

	void PendingMeleeAttacks::update(float seconds)
	{
		const auto meleeAttackerView = mRegistry.view<component::MeleeAttacker, component::BodyRect, component::FaceDirection, component::Player>();
		for (const auto& meleeAttacker : meleeAttackerView)
		{
			const auto& playerFaceDirection = meleeAttackerView.get<component::FaceDirection>(meleeAttacker);
			auto& playerMeleeAttack = meleeAttackerView.get<component::MeleeAttacker>(meleeAttacker);

			bool cooldown = hasCooldown(playerMeleeAttack.cooldownSinceLastHit);
			if (cooldown)
				playerMeleeAttack.cooldownSinceLastHit -= seconds;

			if (playerMeleeAttack.isTryingToAttack)
			{
				playerMeleeAttack.isTryingToAttack = false;

				if (cooldown)
					return;	

				const auto& playerBody = meleeAttackerView.get<component::BodyRect>(meleeAttacker);
				performHit(playerBody.rect.getCenter(), getStartAttackRotation(playerFaceDirection.direction));

				playerMeleeAttack.cooldownSinceLastHit = playerMeleeAttack.minSecondsInterval;
			}
		}
	}

	bool PendingMeleeAttacks::hasCooldown(float cooldownSinceLastHit) const
	{
		return cooldownSinceLastHit > 0.f;
	}

	void PendingMeleeAttacks::performHit(const sf::Vector2f playerPosition, float weaponInitialRotation)
	{
		auto enemies = mRegistry.view<component::BodyRect, component::Killable>(entt::exclude<component::Player>);
		for(auto enemy : enemies)
		{
			const auto& enemyBody = enemies.get<component::BodyRect>(enemy);
			auto nearestPoint = nearestPointOfCharacter(enemyBody.rect, playerPosition);
			auto distance = Math::distanceBetweenPoints(playerPosition, nearestPoint);

			const float range = 27.f;
			if (distance > range)
				continue;

			float characterAngle = angleOfPointToStart(nearestPoint, playerPosition);
			if (isAngleInAttackRange(characterAngle, weaponInitialRotation))
				mRegistry.assign_or_replace<component::DamageTag>(enemy, 50);
		}	
	}

	sf::Vector2f PendingMeleeAttacks::nearestPointOfCharacter(const FloatRect& rect, const sf::Vector2f playerPosition) const
	{
		auto right = rect.left + rect.width;
		auto bottom = rect.top + rect.height;

		bool onLeft = right < playerPosition.x;
		bool onRight = rect.left > playerPosition.x;
		bool above = bottom < playerPosition.y;
		bool under = rect.top > playerPosition.y;

		bool sameXAxis = !onLeft && !onRight;
		bool sameYAxis = !above && !under;

		if (sameXAxis && !sameYAxis)
		{
			if (under)
				return sf::Vector2f(playerPosition.x, rect.top);
			else
				return sf::Vector2f(playerPosition.x, bottom);
		}
		if (!sameXAxis && sameYAxis)
		{
			if (onRight)
				return sf::Vector2f(rect.left, playerPosition.y);
			else
				return sf::Vector2f(right, playerPosition.y);
		}
		if (sameXAxis && sameYAxis)
		{
			return playerPosition;
		}

		if (onLeft && above)
			return sf::Vector2f(right, bottom);
		if (onLeft && under)
			return sf::Vector2f(right, rect.top);
		if (onRight && above)
			return sf::Vector2f(rect.left, bottom);
		if (onRight && under)
			return sf::Vector2f(rect.left, rect.top);

		return {};
	}

	float PendingMeleeAttacks::angleOfPointToStart(sf::Vector2f point, const sf::Vector2f& playerPosition) const
	{
		point -= playerPosition;

		float angle = std::atan2f(point.y, point.x);
		angle = Math::radiansToDegrees(angle);

		if (angle < 0.f)
			angle += 360.f;
		return angle;
	}

	bool PendingMeleeAttacks::isAngleInAttackRange(float angle, float attackRotation) const
	{
		const float mRotationRange = 100.f;
		float halfOfRotationRange = mRotationRange / 2.f;
		auto attackRange = std::make_pair(getFixedAngle(attackRotation - halfOfRotationRange), getFixedAngle(attackRotation + halfOfRotationRange));

		if (attackRange.first < attackRange.second)
			return angle >= attackRange.first && angle <= attackRange.second;
		return angle >= attackRange.second || angle <= attackRange.first;
	}

	float PendingMeleeAttacks::getFixedAngle(float angle) const
	{
		angle -= (static_cast<unsigned>(angle) / 360) * 360.f;
		if (angle < 0.f)
			angle += 360.f;
		return angle;
	}

	float PendingMeleeAttacks::getStartAttackRotation(const sf::Vector2f& playerFaceDirection) const
	{
		if (playerFaceDirection == sf::Vector2f(1, 0))
			return 0.f;
		else if (playerFaceDirection == sf::Vector2f(-1, 0))
			return 180.f;
		else if (playerFaceDirection == sf::Vector2f(0, 1))
			return 90.f;
		else if (playerFaceDirection == sf::Vector2f(0, -1))
			return -90.f;
		else if (playerFaceDirection == sf::Vector2f(0.7f, -0.7f))
			return -45.f;
		else if (playerFaceDirection == sf::Vector2f(-0.7f, -0.7f))
			return -135.f;
		else if (playerFaceDirection == sf::Vector2f(0.7f, 0.7f))
			return 45.f;
		else if (playerFaceDirection == sf::Vector2f(-0.7f, 0.7f))
			return 135.f;

		return 0.f;
	}

}
