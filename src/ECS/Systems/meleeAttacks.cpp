#pragma once

#include "meleeAttacks.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "Utilities/math.hpp"
#include "Utilities/direction.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

void MeleeAttacks::update(float dt)
{
	PH_PROFILE_FUNCTION();

	auto meleeAttackerView = mRegistry.view<component::Player, component::MeleeAttacker, component::FaceDirection, component::BodyRect>();
	for (auto meleeAttacker : meleeAttackerView)
	{
		const auto& playerFaceDirection = meleeAttackerView.get<component::FaceDirection>(meleeAttacker);
		auto& meleeAttackerDetails = meleeAttackerView.get<component::MeleeAttacker>(meleeAttacker);

		if (meleeAttackerDetails.isTryingToAttack)
		{
			meleeAttackerDetails.isTryingToAttack = false;
			if (!meleeAttackerDetails.canAttack)
				return;	

			auto meleeView = mRegistry.view<component::CurrentMeleeWeapon, component::MeleeProperties>();
			for (auto melee : meleeView)
			{
				const auto& meleeProperties = meleeView.get<component::MeleeProperties>(melee);
				const auto& playerBody = meleeAttackerView.get<component::BodyRect>(meleeAttacker);
				tagEnemiesInMeleeAttackArea(playerFaceDirection.direction, playerBody.rect, meleeProperties.range);
				performHit(playerBody.rect.getCenter(), getStartAttackRotation(playerFaceDirection.direction), meleeProperties.damage, meleeProperties.range, meleeProperties.rotationRange);
				clearInMeleeAttackAreaTags();

				meleeAttackerDetails.isAttacking = true;
				meleeAttackerDetails.cooldownSinceLastHit = meleeProperties.minHitInterval;
			}
		}
	}
}

void MeleeAttacks::tagEnemiesInMeleeAttackArea(sf::Vector2f playerFaceDirection, const FloatRect& playerBody, float range) const
{
	FloatRect attackArea(playerBody.getCenter(), sf::Vector2f(0.f, 0.f));

	attackArea.width, attackArea.height = range * 2.f;
	attackArea.left -= attackArea.width / 2.f;
	attackArea.top -= attackArea.height / 2.f;

	auto enemiesView = mRegistry.view<component::Killable, component::BodyRect>(entt::exclude<component::Player>);
	for (auto enemy : enemiesView)
	{
		const auto& enemyBody = enemiesView.get<component::BodyRect>(enemy);
		if (attackArea.doPositiveRectsIntersect(enemyBody.rect))
			mRegistry.assign_or_replace<component::InPlayerAttackArea>(enemy);
	}
}

void MeleeAttacks::performHit(const sf::Vector2f playerPosition, float weaponInitialRotation, int damage, float range, float rotationRange)
{
	auto enemies = mRegistry.view<component::BodyRect, component::Killable>(entt::exclude<component::Player>);
	for(auto enemy : enemies)
	{
		const auto& enemyBody = enemies.get<component::BodyRect>(enemy);
		auto nearestPoint = nearestPointOfCharacter(enemyBody.rect, playerPosition);
		auto distance = Math::distanceBetweenPoints(playerPosition, nearestPoint);

		if (distance > range)
			continue;

		float characterAngle = angleOfPointToStart(nearestPoint, playerPosition);
		if (isAngleInAttackRange(characterAngle, weaponInitialRotation, rotationRange))
			mRegistry.assign_or_replace<component::DamageTag>(enemy, damage);
	}	
}

sf::Vector2f MeleeAttacks::nearestPointOfCharacter(const FloatRect& rect, const sf::Vector2f playerPosition) const
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

float MeleeAttacks::angleOfPointToStart(sf::Vector2f point, const sf::Vector2f& playerPosition) const
{
	point -= playerPosition;

	float angle = std::atan2f(point.y, point.x);
	angle = Math::radiansToDegrees(angle);

	if (angle < 0.f)
		angle += 360.f;
	return angle;
}

bool MeleeAttacks::isAngleInAttackRange(float angle, float attackRotation, float rotationRange) const
{
	float halfOfRotationRange = rotationRange / 2.f;
	auto attackRange = std::make_pair(getFixedAngle(attackRotation - halfOfRotationRange), getFixedAngle(attackRotation + halfOfRotationRange));

	if (attackRange.first < attackRange.second)
		return angle >= attackRange.first && angle <= attackRange.second;
	return angle >= attackRange.second || angle <= attackRange.first;
}

float MeleeAttacks::getFixedAngle(float angle) const
{
	angle -= (static_cast<unsigned>(angle) / 360) * 360.f;
	if (angle < 0.f)
		angle += 360.f;
	return angle;
}

float MeleeAttacks::getStartAttackRotation(const sf::Vector2f& playerFaceDirection) const
{
	if (playerFaceDirection == PH_EAST)
		return 0.f;
	else if (playerFaceDirection == PH_WEST) 
		return 180.f;
	else if (playerFaceDirection == PH_SOUTH)
		return 90.f;
	else if (playerFaceDirection == PH_NORTH)
		return -90.f;
	else if (playerFaceDirection == PH_NORTH_EAST)
		return -45.f;
	else if (playerFaceDirection == PH_NORTH_WEST)
		return -135.f;
	else if (playerFaceDirection == PH_SOUTH_EAST)
		return 45.f;
	else if (playerFaceDirection == PH_SOUTH_WEST)
		return 135.f;

	return 0.f;
}

void MeleeAttacks::clearInMeleeAttackAreaTags() const
{
	auto enemiesInAreaAttackView = mRegistry.view<component::InPlayerAttackArea>();
	for (auto enemy : enemiesInAreaAttackView)
		mRegistry.remove<component::InPlayerAttackArea>(enemy);
}

}
