#include "pch.hpp"
#include "meleeAttacks.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/entityUtil.hpp"
#include "Utilities/direction.hpp"

namespace ph::system {

void MeleeAttacks::onEvent(sf::Event e)
{
	if(sPause || inputDisabled) return;

	if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Backslash && !mShouldWeaponBeRendered)
		mIsAttackButtonPressed = true;
	if(e.type == sf::Event::JoystickButtonPressed && e.joystickButton.button == 1 && !mShouldWeaponBeRendered)
		mIsAttackButtonPressed = true;
}

static float getStartAttackRotation(Vec2 playerFaceDir)
{
	if(playerFaceDir == PH_EAST)
		return 90.f;
	else if(playerFaceDir == PH_WEST)
		return -90.f;
	else if(playerFaceDir == PH_SOUTH)
		return 180.f;
	else if(playerFaceDir == PH_NORTH)
		return 0.f;
	else if(playerFaceDir == PH_NORTH_EAST)
		return 45.f;
	else if(playerFaceDir == PH_NORTH_WEST)
		return -45.f;
	else if(playerFaceDir == PH_SOUTH_EAST)
		return 135.f;
	else if(playerFaceDir == PH_SOUTH_WEST)
		return -135.f;
	return 0.f;
}

using namespace component;

void MeleeAttacks::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	if(!isPlayerAlive()) return;

	auto playerEntity = getPlayerEntity();
	Vec2 playerFaceDir = getPlayerFaceDirection();
	Vec2 playerPos = getPlayerCenterPos(); 

	mRegistry.view<CurrentMeleeWeapon, MeleeProperties, RenderQuad, BodyRect>().each([&]
	(auto meleeWeaponEntity, auto, const auto& meleeProperties, auto& renderQuad, auto& weaponBody)
	{
		if(mIsAttackButtonPressed)
		{
			mIsAttackButtonPressed = false;
			mStartWeaponRotation = getStartAttackRotation(playerFaceDir);

			// set melee weapon ahead or behind player 
			renderQuad.z = playerFaceDir.y >= 0.f ? 93 : 96;

			// deal damage and push enemy
			FloatRect attackArea(
				playerPos - sf::Vector2(meleeProperties.range, meleeProperties.range),
				Vec2(meleeProperties.range * 2, meleeProperties.range * 2)
			);
			bool wasEnemyHit = false;

			mRegistry.view<Killable, BodyRect, Kinematics>(entt::exclude<Player>).each([&]
			(auto enemyEntity, auto, auto enemyBody, auto& kinematics)
			{
				Vec2 enemyBodyCenter = enemyBody.center();
				if(intersect(attackArea, enemyBody))
				{
					float enemyAngle = std::atan2f(enemyBodyCenter.y - playerPos.y, enemyBodyCenter.x - playerPos.x);
					enemyAngle = radiansToDegrees(enemyAngle);
					if(enemyAngle >= mStartWeaponRotation - meleeProperties.rotationRange - 10.f &&
					   enemyAngle <= mStartWeaponRotation + 10.f) 
					{
						mRegistry.assign_or_replace<DamageTag>(enemyEntity, meleeProperties.damage);
						kinematics.vel = Vec2(playerFaceDir.x, playerFaceDir.y) * 300.f;
						kinematics.friction = 0.005f;
						kinematics.frictionLerpSpeed = 0.033f;
						wasEnemyHit = true;
					}
				}
			});

			// shake camera
			CameraShake shake;
			shake.duration = 0.35f;
			shake.magnitude = wasEnemyHit ? 2.f : 0.15f;
			shake.smooth = !wasEnemyHit;
			mRegistry.assign_or_replace<CameraShake>(playerEntity, shake);

			// initialize weapon rendering
			mShouldWeaponBeRendered = true;
			renderQuad.rotation = mStartWeaponRotation;
			mRegistry.remove<HiddenForRenderer>(meleeWeaponEntity);
		}

		if(mShouldWeaponBeRendered)
		{
			// set weapon position
			weaponBody.pos = playerPos - Vec2(12.f);

			// rotate weapon
			constexpr float anglesPerSecond = 240.f;
			renderQuad.rotation -= dt * anglesPerSecond;

			if(renderQuad.rotation <= mStartWeaponRotation - meleeProperties.rotationRange) 
			{
				// stop rendering weapon
				mShouldWeaponBeRendered = false;
				mRegistry.assign_or_replace<HiddenForRenderer>(meleeWeaponEntity);
			}
		}
	});
}

}

