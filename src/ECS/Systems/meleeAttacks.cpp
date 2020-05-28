#include "pch.hpp"
#include "meleeAttacks.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Utilities/direction.hpp"

namespace ph::system {

void MeleeAttacks::onEvent(sf::Event e)
{
	if(sPause || inputDisabled)
		return;

	if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Backslash && !mShouldWeaponBeRendered)
		mIsAttackButtonPressed = true;
	if(e.type == sf::Event::JoystickButtonPressed && e.joystickButton.button == 1 && !mShouldWeaponBeRendered)
		mIsAttackButtonPressed = true;
}

void MeleeAttacks::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause)
		return;

	auto players = mRegistry.view<component::FaceDirection, component::BodyRect, component::Player>();
	for(auto player : players)
	{
		const auto& [faceDirection, playerBody] = players.get<component::FaceDirection, component::BodyRect>(player);
		const Vec2 playerBodyCenter = playerBody.center();

		auto currentMeleeWeaponView = mRegistry.view<component::CurrentMeleeWeapon, component::MeleeProperties, component::RenderQuad, component::BodyRect>();

		for(auto& meleeWeapon : currentMeleeWeaponView)
		{
			const auto& meleeProperties = currentMeleeWeaponView.get<component::MeleeProperties>(meleeWeapon);
			auto& [renderQuad, weaponBody] = currentMeleeWeaponView.get<component::RenderQuad, component::BodyRect>(meleeWeapon);

			if(mIsAttackButtonPressed)
			{
				mIsAttackButtonPressed = false;
				mStartWeaponRotation = getStartAttackRotation(faceDirection);

				// set melee weapon ahead or behind player 
				renderQuad.z = faceDirection.y >= 0.f ? 93 : 96;

				// deal damage, push enemy and shake camera
				FloatRect attackArea(
					playerBodyCenter - sf::Vector2(meleeProperties.range, meleeProperties.range),
					Vec2(meleeProperties.range * 2, meleeProperties.range * 2)
				);
				bool wasEnemyHit = false;
				auto enemies = mRegistry.view<component::Killable, component::BodyRect, component::Kinematics>(entt::exclude<component::Player>);
				for(auto enemy : enemies)
				{
					const auto& [enemyBody, kinematics] = enemies.get<component::BodyRect, component::Kinematics>(enemy);
					const Vec2 enemyBodyCenter = enemyBody.center();
					if(intersect(attackArea, enemyBody))
					{
						float enemyAngle = std::atan2f(enemyBodyCenter.y - playerBodyCenter.y, enemyBodyCenter.x - playerBodyCenter.x);
						enemyAngle = Math::radiansToDegrees(enemyAngle);
						if(enemyAngle >= mStartWeaponRotation - meleeProperties.rotationRange - 10.f && enemyAngle <= mStartWeaponRotation + 10.f) 
						{
							mRegistry.assign_or_replace<component::DamageTag>(enemy, meleeProperties.damage);
							kinematics.vel = Vec2(faceDirection.x, faceDirection.y) * 300.f;
							kinematics.friction = 0.005f;
							kinematics.frictionLerpSpeed = 0.033f;
							wasEnemyHit = true;
						}
					}
				};

				// shake camera
				component::CameraShake shake;
				shake.duration = 0.35f;
				shake.magnitude = wasEnemyHit ? 2.f : 0.15f;
				shake.smooth = !wasEnemyHit;
				mRegistry.assign_or_replace<component::CameraShake>(player, shake);

				// initialize weapon rendering
				mShouldWeaponBeRendered = true;
				renderQuad.rotation = mStartWeaponRotation;
				PH_ASSERT_UNEXPECTED_SITUATION(mRegistry.has<component::HiddenForRenderer>(meleeWeapon),
					"Melee weapon doesn't have HiddenForRenderer component!");
				mRegistry.remove<component::HiddenForRenderer>(meleeWeapon);
			}

			if(mShouldWeaponBeRendered)
			{
				// set weapon position
				weaponBody.pos = playerBody.center() - Vec2(12, 12);

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

float MeleeAttacks::getStartAttackRotation(Vec2 faceDirection) const
{
	if(faceDirection == PH_EAST)
		return 90.f;
	else if(faceDirection == PH_WEST)
		return -90.f;
	else if(faceDirection == PH_SOUTH)
		return 180.f;
	else if(faceDirection == PH_NORTH)
		return 0.f;
	else if(faceDirection == PH_NORTH_EAST)
		return 45.f;
	else if(faceDirection == PH_NORTH_WEST)
		return -45.f;
	else if(faceDirection == PH_SOUTH_EAST)
		return 135.f;
	else if(faceDirection == PH_SOUTH_WEST)
		return -135.f;
	return 0.f;
}

}

