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
	if(e.mType == ActionEvent::Type::Pressed && e.mAction == "meleeAttack" && !mShouldWeaponBeRendered)
		mIsAttackButtonPressed = true;
}

void MeleeAttacks::update(float dt)
{
	PH_PROFILE_FUNCTION();

	auto players = mRegistry.view<component::FaceDirection, component::BodyRect, component::Player>();
	for(auto player : players)
	{
		const auto& [faceDirection, playerBody] = players.get<component::FaceDirection, component::BodyRect>(player);
		const sf::Vector2f playerBodyCenter = playerBody.rect.getCenter();

		auto currentMeleeWeaponView = mRegistry.view<component::CurrentMeleeWeapon, component::MeleeProperties, component::RenderQuad, component::BodyRect>();

		for(auto& meleeWeapon : currentMeleeWeaponView)
		{
			const auto& meleeProperties = currentMeleeWeaponView.get<component::MeleeProperties>(meleeWeapon);
			auto& [renderQuad, weaponBody] = currentMeleeWeaponView.get<component::RenderQuad, component::BodyRect>(meleeWeapon);

			if(mIsAttackButtonPressed)
			{
				mIsAttackButtonPressed = false;
				mStartWeaponRotation = getStartAttackRotation(faceDirection.direction);

				// deal damage
				FloatRect attackArea(
					playerBodyCenter - sf::Vector2(meleeProperties.range, meleeProperties.range),
					sf::Vector2f(meleeProperties.range * 2, meleeProperties.range * 2)
				);
				auto enemies = mRegistry.view<component::Killable, component::BodyRect>(entt::exclude<component::Player>);
				for(auto enemy : enemies)
				{
					const auto& enemyBody = enemies.get<component::BodyRect>(enemy);
					const sf::Vector2f enemyBodyCenter = enemyBody.rect.getCenter();
					if(attackArea.doPositiveRectsIntersect(enemyBody.rect))
					{
						float enemyAngle = std::atan2f(enemyBodyCenter.y - playerBodyCenter.y, enemyBodyCenter.x - playerBodyCenter.x);
						enemyAngle = Math::radiansToDegrees(enemyAngle);
						if(enemyAngle >= mStartWeaponRotation - meleeProperties.rotationRange - 10 && enemyAngle <= mStartWeaponRotation + 10)
							mRegistry.assign_or_replace<component::DamageTag>(enemy, meleeProperties.damage);
					}
				};

				// initialize weapon rendering
				mShouldWeaponBeRendered = true;
				renderQuad.rotation = mStartWeaponRotation;
				PH_ASSERT_UNEXPECTED_SITUATION(mRegistry.has<component::HiddenForRenderer>(meleeWeapon), "Melee weapon doesn't have HiddenForRenderer component!");
				mRegistry.remove<component::HiddenForRenderer>(meleeWeapon);
			}

			if(mShouldWeaponBeRendered)
			{
				// set weapon position
				weaponBody.rect.setPosition(playerBody.rect.getCenter() - sf::Vector2f(12, 12));

				// rotate weapon
				constexpr float anglesPerSecond = 240.f;
				renderQuad.rotation -= dt * anglesPerSecond;

				if(renderQuad.rotation <= mStartWeaponRotation - meleeProperties.rotationRange) {
					// stop rendering weapon
					mShouldWeaponBeRendered = false;
					mRegistry.assign_or_replace<component::HiddenForRenderer>(meleeWeapon);
				}
			}
		};
	}
}

float MeleeAttacks::getStartAttackRotation(const sf::Vector2f& faceDirection) const
{
	if(faceDirection == PH_EAST)
		return 90.f;
	else if(faceDirection == PH_WEST)
		return 270.f;
	else if(faceDirection == PH_SOUTH)
		return 180.f;
	else if(faceDirection == PH_NORTH)
		return 0.f;
	else if(faceDirection == PH_NORTH_EAST)
		return 45.f;
	else if(faceDirection == PH_NORTH_WEST)
		return 315.f;
	else if(faceDirection == PH_SOUTH_EAST)
		return 135.f;
	else if(faceDirection == PH_SOUTH_WEST)
		return 215.f;
	return 0.f;
}

}

