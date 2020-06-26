#include "pch.hpp"
#include "gunAttacks.hpp"
#include "ECS/Components/audioComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Utilities/random.hpp"
#include "Utilities/direction.hpp"
#include "Utilities/joystickMacros.hpp"

namespace ph::system {

void GunAttacks::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause)
		return;

	handlePendingGunAttacks();
	handleLastingBullets();
}

using namespace component;

void GunAttacks::onEvent(sf::Event e)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	auto shoot = [this]()
	{
		if(!shootInputDisabled)
		{
			mRegistry.view<Player, GunAttacker>().each([&]
			(auto, auto& gunAttacker)
			{
				gunAttacker.isTryingToAttack = true;
			});
		}
	};

	auto changeWeapon = [this]()
	{
		if(!changeWeaponInputDisabled)
		{
			mRegistry.view<CurrentGun, GunProperties>().each([&]
			(auto currentGunEntity, auto, const auto& currentGunProperties)
			{
				mRegistry.assign_or_replace<HiddenForRenderer>(currentGunEntity);
				mRegistry.remove<CurrentGun>(currentGunEntity);

				mRegistry.view<GunProperties>(entt::exclude<CurrentGun>).each([&]
				(auto otherGunEntity, const auto& gunProperties)
				{
					if(gunProperties.gunId != currentGunProperties.gunId)
						mRegistry.assign<CurrentGun>(otherGunEntity);
				});
			});

			mRegistry.view<Player, GunAttacker>().each([&]
			(auto, auto& gunAttacker)
			{
				gunAttacker.timeToHide = gunAttacker.timeBeforeHiding;
			});
		}
	};

	if(e.type == sf::Event::KeyPressed)
	{
		if(e.key.code == sf::Keyboard::Enter) 
			shoot();	
		else if(e.key.code == sf::Keyboard::Q)
			changeWeapon();
	}
	else if(e.type == sf::Event::JoystickButtonPressed)
	{
		if(e.joystickButton.button == PH_JOYSTICK_A)
			shoot();
		else if(e.joystickButton.button == PH_JOYSTICK_LB || e.joystickButton.button == PH_JOYSTICK_RB)
			changeWeapon();
	}
}

void GunAttacks::handlePendingGunAttacks() const
{
	mRegistry.view<Player, GunAttacker, Bullets, FaceDirection, BodyRect>().each([&]
	(auto, auto& gunAttacker, auto& playerBullets, auto playerFaceDirection, auto playerBody)
	{
		if(gunAttacker.isTryingToAttack)
		{
			gunAttacker.isTryingToAttack = false;

			mRegistry.view<CurrentGun, GunProperties, BodyRect>().each([&]
			(auto, const auto& gunProperties, auto gunBody)
			{
				if((gunProperties.type == GunProperties::Type::Pistol && playerBullets.numOfPistolBullets <= 0.f) ||
				   (gunProperties.type == GunProperties::Type::Shotgun && playerBullets.numOfShotgunBullets <= 0.f)) 
				{
					return;
				}

				Vec2 startingBulletPosition = gunBody.pos + getBulletStartingPosition(playerFaceDirection);

				bool wasOpponentHit = false;
				tagEnemiesInGunAttackArea(playerFaceDirection, playerBody, gunBody.size,
					gunProperties.range, gunProperties.deflectionAngle, &wasOpponentHit);

				std::vector<Vec2> shotsEndingPositions = performShoot(playerFaceDirection, startingBulletPosition,
					gunProperties.range, gunProperties.deflectionAngle, gunProperties.damage, gunProperties.numberOfBullets);

				createShotImage(startingBulletPosition, shotsEndingPositions, gunProperties.shotSoundFilepath);
				clearInGunAttackAreaTags();

				if(gunProperties.type == GunProperties::Type::Pistol) 
				{
					--playerBullets.numOfPistolBullets;
					wasOpponentHit ? shakeCamera(2.f, false) : shakeCamera(0.15f, true);
				}
				else if(gunProperties.type == GunProperties::Type::Shotgun) 
				{
					--playerBullets.numOfShotgunBullets;
					wasOpponentHit ? shakeCamera(3.f, false) : shakeCamera(0.2f, true);
				}
			});
		}
	});
}

Vec2 GunAttacks::getBulletStartingPosition(Vec2 playerFaceDirection) const
{
	if(playerFaceDirection == PH_WEST || playerFaceDirection == PH_EAST)
		return Vec2(25.f * playerFaceDirection.x, 25.f);
	if(playerFaceDirection == PH_NORTH || playerFaceDirection == PH_SOUTH)
		return Vec2(25.f, 25.f * -playerFaceDirection.y);
	else if(playerFaceDirection == PH_NORTH_EAST || playerFaceDirection == PH_SOUTH_EAST)
		return Vec2(25.f, 25.f);
	else if(playerFaceDirection == PH_NORTH_WEST || playerFaceDirection == PH_SOUTH_WEST)
		return Vec2(-25.f, 25.f);
	else
		return Vec2(0.f, 0.f);
}

void GunAttacks::tagEnemiesInGunAttackArea(Vec2 playerFaceDirection, FloatRect playerBody, Vec2 gunSize,
                                           float range, float deflectionAngle, bool* wasOpponentHitOut) const
{
	FloatRect attackArea(playerBody.center(), Vec2(0.f, 0.f));
	if(playerFaceDirection == PH_WEST)
		attackArea.x += range * playerFaceDirection.x;
	else if(playerFaceDirection == PH_NORTH)
		attackArea.y += range * playerFaceDirection.y;

	if(playerFaceDirection == PH_WEST || playerFaceDirection == PH_EAST)
	{
		attackArea.w = range;
		attackArea.h = range * std::tan(degreesToRadians(deflectionAngle)) * 2.f;

		attackArea.x += (playerBody.w / 2.f + gunSize.x / 2.f) * playerFaceDirection.x;;
		attackArea.y -= attackArea.h / 2.f;
	}
	else if(playerFaceDirection == PH_SOUTH || playerFaceDirection == PH_NORTH)
	{
		attackArea.w = range * std::tan(degreesToRadians(deflectionAngle)) * 2.f;
		attackArea.h = range;

		attackArea.x -= attackArea.w / 2.f;
		attackArea.y += (playerBody.h / 2.f + gunSize.y / 2.f) * playerFaceDirection.y;
	}
	else if(PH_IS_DIAGONAL(playerFaceDirection))
	{
		attackArea.size = Vec2(range, range);
		if(PH_IS_X_AXIS_POSITIVE(playerFaceDirection))
			attackArea.x += playerBody.w / 2.f;
		else if(PH_IS_X_AXIS_NEGATIVE(playerFaceDirection))
			attackArea.x -= range + playerBody.w / 2.f;
		if(PH_IS_Y_AXIS_POSITIVE(playerFaceDirection))
			attackArea.y += playerBody.h / 2.f;
		else if(PH_IS_Y_AXIS_NEGATIVE(playerFaceDirection))
			attackArea.y -= range + playerBody.h / 2.f;
	}

	mRegistry.view<Killable, InsideSimRegion, BodyRect>(entt::exclude<Player>).each([&]
	(auto enemyEntity, auto, auto, auto enemyBody)
	{
		if(intersect(attackArea, enemyBody))
		{
			mRegistry.assign<InPlayerGunAttackArea>(enemyEntity);
			*wasOpponentHitOut = true;
		}
	});
}

std::vector<Vec2> GunAttacks::performShoot(Vec2 playerFaceDirection, Vec2 startingBulletPos,
                                           float range, float deflectionAngle, i32 damage, i32 numberOfBullets) const
{
	// TODO: This algorithm is very very very slow!
	auto enemiesWithDamageTag = mRegistry.view<DamageTag, InPlayerGunAttackArea, Killable, BodyRect>();
	auto enemies = mRegistry.view<InPlayerGunAttackArea, Killable, BodyRect>();
	std::vector<Vec2> shotsEndingPositions;
	const i32 bulletShiftValue = 5;

	for(i32 i = 0; i < numberOfBullets; ++i)
	{
		Vec2 direction = getBulletDirection(playerFaceDirection, deflectionAngle);
		Vec2 currentBulletPos = startingBulletPos;
		i32 bulletTravelledDist = 1;

		while(bulletTravelledDist < range)
		{
			bool didBulletHitEnemy = false;
			for(const auto enemy : enemies)
			{
				const auto& bodyRect = enemies.get<BodyRect>(enemy);
				if(bodyRect.contains(currentBulletPos))
				{
					if(enemiesWithDamageTag.contains(enemy))
					{
						auto& damageTagValue = enemiesWithDamageTag.get<DamageTag>(enemy);
						damageTagValue.amountOfDamage += damage;
					}
					else
					{
						mRegistry.assign<DamageTag>(enemy, damage);
					}
					didBulletHitEnemy = true;
					break;
				}
			}

			if(didBulletHitEnemy) break;

			bulletTravelledDist += bulletShiftValue;
			currentBulletPos = getCurrentPosition(direction, startingBulletPos, bulletTravelledDist);
		}
		shotsEndingPositions.emplace_back(currentBulletPos);
	}

	return shotsEndingPositions;
}

Vec2 GunAttacks::getBulletDirection(Vec2 playerFaceDirection, float deflection) const
{
	deflection = Random::generateNumber(-deflection, deflection);
	const float deflectionFactor = deflection / -90.f;
	Vec2 deflectedBulletDirection = playerFaceDirection;

	if(playerFaceDirection == PH_NORTH)
	{
		if(deflectionFactor < 0.f)
		{
			deflectedBulletDirection.x += deflectionFactor;
			deflectedBulletDirection.y += -deflectionFactor;
		}
		else
		{
			deflectedBulletDirection.x += deflectionFactor;
			deflectedBulletDirection.y += deflectionFactor;
		}
	}
	else if(playerFaceDirection == PH_EAST)
	{
		if(deflectionFactor < 0.f)
		{
			deflectedBulletDirection.x += deflectionFactor;
			deflectedBulletDirection.y += deflectionFactor;
		}
		else
		{
			deflectedBulletDirection.x += -deflectionFactor;
			deflectedBulletDirection.y += deflectionFactor;
		}
	}
	else if(playerFaceDirection == PH_SOUTH)
		if(deflectionFactor < 0.f)
		{
			deflectedBulletDirection.x += -deflectionFactor;
			deflectedBulletDirection.y += deflectionFactor;
		}
		else
		{
			deflectedBulletDirection.x += -deflectionFactor;
			deflectedBulletDirection.y += -deflectionFactor;
		}
	else if(playerFaceDirection == PH_WEST)
		if(deflectionFactor < 0.f)
		{
			deflectedBulletDirection.x += -deflectionFactor;
			deflectedBulletDirection.y += -deflectionFactor;
		}
		else
		{
			deflectedBulletDirection.x += deflectionFactor;
			deflectedBulletDirection.y += -deflectionFactor;
		}

	else if(playerFaceDirection == PH_NORTH_WEST)
	{
		if(deflectionFactor < 0.f)
			deflectedBulletDirection.y += -deflectionFactor;
		else
			deflectedBulletDirection.x += deflectionFactor;
	}
	else if(playerFaceDirection == PH_NORTH_EAST)
	{
		if(deflectionFactor < 0.f)
			deflectedBulletDirection.x += deflectionFactor;
		else
			deflectedBulletDirection.y += deflectionFactor;
	}
	else if(playerFaceDirection == PH_SOUTH_WEST)
	{
		if(deflectionFactor < 0.f)
			deflectedBulletDirection.x += -deflectionFactor;
		else
			deflectedBulletDirection.y += -deflectionFactor;
	}
	else if(playerFaceDirection == PH_SOUTH_EAST)
	{
		if(deflectionFactor < 0.f)
			deflectedBulletDirection.y += deflectionFactor;
		else
			deflectedBulletDirection.x += -deflectionFactor;
	}

	return deflectedBulletDirection;
}

Vec2 GunAttacks::getCurrentPosition(Vec2 bulletDirection, Vec2 startingPos, i32 bulletDistance) const
{
	Vec2 newPosition;
	newPosition.x = startingPos.x + bulletDirection.x * bulletDistance;
	newPosition.y = startingPos.y + bulletDirection.y * bulletDistance;
	return newPosition;
}

void GunAttacks::clearInGunAttackAreaTags() const
{
	mRegistry.view<InPlayerGunAttackArea>().each([&]
	(auto enemyEntity, auto)
	{
		mRegistry.remove<InPlayerGunAttackArea>(enemyEntity);
	});
}

void GunAttacks::createShotImage(Vec2 shotsStartingPosition, const std::vector<Vec2>& shotsEndingPosition,
                                 const std::string& soundFilename) const
{
	for(auto shot : shotsEndingPosition)
	{
		auto entity = mRegistry.create();
		mRegistry.assign<LastingShot>(entity, shotsStartingPosition, shot);
		mRegistry.assign<Lifetime>(entity, .05f);
	}

	auto soundEntity = mRegistry.create();
	mRegistry.assign<AmbientSound>(soundEntity, soundFilename);
	mRegistry.assign<Lifetime>(soundEntity, .05f);
}

void GunAttacks::handleLastingBullets() const
{
	mRegistry.view<LastingShot>().each([]
	(auto lastingShot)
	{
		Renderer::submitLine(sf::Color(230, 160, 0), sf::Color(250, 250, 200),
			lastingShot.startingShotPos, lastingShot.endingShotPos, 1.f);
	});
}

void GunAttacks::shakeCamera(float magnitude, bool smooth) const
{
	mRegistry.view<Player>().each([&]
	(auto playerEntity, auto)
	{
		CameraShake shake;
		shake.duration = 0.35f;
		shake.magnitude = magnitude;
		shake.smooth = smooth;
		mRegistry.assign_or_replace<CameraShake>(playerEntity, shake);
	});
}

}
