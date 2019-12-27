#pragma once

#include "meleeAttacks.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Utilities/math.hpp"
#include "Utilities/direction.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

void MeleeAttacks::onEvent(const ActionEvent& e)
{
	if(e.mType == ActionEvent::Type::Pressed && e.mAction == "meleeAttack")
		mIsAttackButtonPressed = true;
}

void MeleeAttacks::update(float dt)
{
	PH_PROFILE_FUNCTION();

	auto players = mRegistry.view<component::FaceDirection, component::BodyRect, component::Player>();
	for(auto player : players)
	{
		const auto& [faceDirection, playerBody] = players.get<component::FaceDirection, component::BodyRect>(player);

		auto currentMeleeWeaponView = mRegistry.view<component::CurrentMeleeWeapon, component::MeleeProperties, component::RenderQuad, component::BodyRect>();

		for(auto& meleeWeapon : currentMeleeWeaponView)
		{
			const auto& meleeProperties = currentMeleeWeaponView.get<component::MeleeProperties>(meleeWeapon);
			auto& [renderQuad, weaponBody] = currentMeleeWeaponView.get<component::RenderQuad, component::BodyRect>(meleeWeapon);

			if(mIsAttackButtonPressed && !mShouldWeaponBeRendered)
			{
				mIsAttackButtonPressed = false;

				// deal damage
				FloatRect attackArea(
					playerBody.rect.getCenter() - sf::Vector2(meleeProperties.range, meleeProperties.range),
					sf::Vector2f(meleeProperties.range * 2, meleeProperties.range * 2)
				);
				auto enemies = mRegistry.view<component::Killable, component::BodyRect>(entt::exclude<component::Player>);
				for(auto enemy : enemies)
				{
					const auto& enemyBody = enemies.get<component::BodyRect>(enemy);
					if(attackArea.doPositiveRectsIntersect(enemyBody.rect))
					{
						auto nearestPoint = nearestPointOfCharacter(enemyBody.rect, playerBody.rect.getTopLeft());
						auto distance = Math::distanceBetweenPoints(playerBody.rect.getCenter(), nearestPoint);
						if(distance < meleeProperties.range)
						{
							float characterAngle = angleOfPointToStart(nearestPoint, playerBody.rect.getCenter());
							if(isAngleInAttackRange(characterAngle, getStartAttackRotation(faceDirection.direction), meleeProperties.rotationRange))
								mRegistry.assign_or_replace<component::DamageTag>(enemy, meleeProperties.damage);
						}
					}
				};

				// initialize weapon rendering
				mShouldWeaponBeRendered = true;
				mWeaponRotation = 0.f;
				PH_ASSERT_UNEXPECTED_SITUATION(mRegistry.has<component::HiddenForRenderer>(meleeWeapon), "Melee weapon doesn't have HiddenForRenderer component!");
				mRegistry.remove<component::HiddenForRenderer>(meleeWeapon);
			}

			if(mShouldWeaponBeRendered)
			{
				// set weapon position
				const sf::Vector2f playerCenter = playerBody.rect.getCenter();
				if(faceDirection.direction == PH_EAST)
					weaponBody.rect.setPosition(playerCenter + sf::Vector2f(0.f, -3.f));
				else if(faceDirection.direction == PH_WEST)
					weaponBody.rect.setPosition(playerCenter + sf::Vector2f(3.f, -3.f));
				else if(faceDirection.direction == PH_NORTH)
					weaponBody.rect.setPosition(playerCenter + sf::Vector2f(-3.f, 1.f));
				else if(faceDirection.direction == PH_SOUTH)
					weaponBody.rect.setPosition(playerCenter + sf::Vector2f(-3.f, 9.f));
				else if(faceDirection.direction == PH_NORTH_WEST)
					weaponBody.rect.setPosition(playerCenter + sf::Vector2f(0.f, -8.f));
				else if(faceDirection.direction == PH_NORTH_EAST)
					weaponBody.rect.setPosition(playerCenter + sf::Vector2f(0.f, -8.f));
				else if(faceDirection.direction == PH_SOUTH_WEST)
					weaponBody.rect.setPosition(playerCenter + sf::Vector2f(3.f, -3.f));
				else if(faceDirection.direction == PH_SOUTH_EAST)
					weaponBody.rect.setPosition(playerCenter + sf::Vector2f(-3.f, -1.f));
				else
					weaponBody.rect.setPosition(playerCenter);

				// rotate weapon
				constexpr float anglesPerSecond = 240.f;
				float thisFrameRotation = dt * anglesPerSecond;
				renderQuad.rotation -= thisFrameRotation;
				mWeaponRotation += thisFrameRotation;

				if(mWeaponRotation > meleeProperties.rotationRange) {
					// stop rendering weapon 
					mShouldWeaponBeRendered = false;
					mRegistry.assign_or_replace<component::HiddenForRenderer>(meleeWeapon);
				}
			}
		};
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

	if(sameXAxis && !sameYAxis)
	{
		if(under)
			return sf::Vector2f(playerPosition.x, rect.top);
		else
			return sf::Vector2f(playerPosition.x, bottom);
	}
	if(!sameXAxis && sameYAxis)
	{
		if(onRight)
			return sf::Vector2f(rect.left, playerPosition.y);
		else
			return sf::Vector2f(right, playerPosition.y);
	}
	if(sameXAxis && sameYAxis)
	{
		return playerPosition;
	}

	if(onLeft && above)
		return sf::Vector2f(right, bottom);
	if(onLeft && under)
		return sf::Vector2f(right, rect.top);
	if(onRight && above)
		return sf::Vector2f(rect.left, bottom);
	if(onRight && under)
		return sf::Vector2f(rect.left, rect.top);

	return {};
}

float MeleeAttacks::angleOfPointToStart(sf::Vector2f point, const sf::Vector2f& playerPosition) const
{
	point -= playerPosition;

	float angle = std::atan2f(point.y, point.x);
	angle = Math::radiansToDegrees(angle);

	if(angle < 0.f)
		angle += 360.f;
	return angle;
}

bool MeleeAttacks::isAngleInAttackRange(float angle, float attackRotation, float rotationRange) const
{
	float halfOfRotationRange = rotationRange / 2.f;
	auto attackRange = std::make_pair(getFixedAngle(attackRotation - halfOfRotationRange), getFixedAngle(attackRotation + halfOfRotationRange));

	if(attackRange.first < attackRange.second)
		return angle >= attackRange.first && angle <= attackRange.second;
	return angle >= attackRange.second || angle <= attackRange.first;
}

float MeleeAttacks::getFixedAngle(float angle) const
{
	angle -= (static_cast<unsigned>(angle) / 360) * 360.f;
	if(angle < 0.f)
		angle += 360.f;
	return angle;
}

float MeleeAttacks::getStartAttackRotation(const sf::Vector2f& faceDirection) const
{
	if(faceDirection == PH_EAST)
		return 0.f;
	else if(faceDirection == PH_WEST)
		return 180.f;
	else if(faceDirection == PH_SOUTH)
		return 90.f;
	else if(faceDirection == PH_NORTH)
		return -90.f;
	else if(faceDirection == PH_NORTH_EAST)
		return -45.f;
	else if(faceDirection == PH_NORTH_WEST)
		return -135.f;
	else if(faceDirection == PH_SOUTH_EAST)
		return 45.f;
	else if(faceDirection == PH_SOUTH_WEST)
		return 135.f;

	return 0.f;
}

}
