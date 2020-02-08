#pragma once

#include "gunAttacks.hpp"
#include "ECS/Components/audioComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "Events/actionEventManager.hpp"
#include "Renderer/renderer.hpp"

#include "Utilities/random.hpp"
#include "Utilities/direction.hpp"
#include "Utilities/profiling.hpp"
#include "Utilities/math.hpp"

namespace ph::system {

void GunAttacks::update(float dt)
{
	PH_PROFILE_FUNCTION(0);

	if(sPause)
		return;

	handlePendingGunAttacks();
	handleLastingBullets();
}

void GunAttacks::onEvent(Event event)
{
	if(sPause)
		return;

	if(auto* e = std::get_if<ActionEvent>(&event))
	{
		if (e->mType == ActionEvent::Type::Pressed)
		{
			if(e->mAction == "gunAttack") 
			{
				auto playerGunView = mRegistry.view<component::Player, component::GunAttacker>();
				for (auto player : playerGunView) {
					auto& playerGunAttack = playerGunView.get<component::GunAttacker>(player);
					playerGunAttack.isTryingToAttack = true;
				}
			}
			else if(e->mAction == "changeWeapon")
			{
				auto currentGunView = mRegistry.view<component::CurrentGun, component::GunProperties>();
				auto otherGunsView = mRegistry.view<component::GunProperties>(entt::exclude<component::CurrentGun>);

				for (auto currentGun : currentGunView)
				{
					const auto& currentGunProperties = currentGunView.get<component::GunProperties>(currentGun);
					mRegistry.assign_or_replace<component::HiddenForRenderer>(currentGun);
					mRegistry.remove<component::CurrentGun>(currentGun);

					for (auto otherGun : otherGunsView) {
						const auto& gunProperties = otherGunsView.get<component::GunProperties>(otherGun);
						if (gunProperties.gunId != currentGunProperties.gunId)
							mRegistry.assign<component::CurrentGun>(otherGun);
					}
				}

				auto gunAttackerView = mRegistry.view<component::Player, component::GunAttacker>();
				gunAttackerView.each([](component::Player, component::GunAttacker& gunAttacker) {
					gunAttacker.timeToHide = gunAttacker.timeBeforeHiding;
				});
			}
		}
	}
}

void GunAttacks::handlePendingGunAttacks() const
{
	auto gunAttackerView = mRegistry.view<component::Player, component::GunAttacker, component::Bullets, component::FaceDirection, component::BodyRect>();
	gunAttackerView.each([this]
	(const component::Player, component::GunAttacker& playerGunAttack, component::Bullets& playerBullets,
	 const component::FaceDirection playerFaceDirection, const component::BodyRect& playerBody)
	{
		if (playerGunAttack.isTryingToAttack)
		{
			playerGunAttack.isTryingToAttack = false;

			auto gunView = mRegistry.view<component::CurrentGun, component::GunProperties, component::BodyRect>();
			for (const auto& gun : gunView)
			{
				const auto& [gunBody, gunProperties] = gunView.get<component::BodyRect, component::GunProperties>(gun);

				if((gunProperties.type == component::GunProperties::Type::Pistol && playerBullets.numOfPistolBullets <= 0.f) ||
				   (gunProperties.type == component::GunProperties::Type::Shotgun && playerBullets.numOfShotgunBullets <= 0.f)) {
					return;
				}

				sf::Vector2f startingBulletPosition = gunBody.rect.getTopLeft() + getBulletStartingPosition(playerFaceDirection.direction);

				tagEnemiesInGunAttackArea(playerFaceDirection.direction, playerBody.rect, gunBody.rect.getSize(),
					gunProperties.range, gunProperties.deflectionAngle);

				std::vector<sf::Vector2f> shotsEndingPositions = performShoot(playerFaceDirection.direction, startingBulletPosition,
					gunProperties.range, gunProperties.deflectionAngle, gunProperties.damage, gunProperties.numberOfBullets);

				createShotImage(startingBulletPosition, shotsEndingPositions, gunProperties.shotSoundFilepath);
				clearInGunAttackAreaTags();

				if(gunProperties.type == component::GunProperties::Type::Pistol) {
					--playerBullets.numOfPistolBullets;
					shakeCamera(0.3f);
				}
				else if(gunProperties.type == component::GunProperties::Type::Shotgun) {
					--playerBullets.numOfShotgunBullets;
					shakeCamera(0.4f);
				}
			}
		}
	});
}

sf::Vector2f GunAttacks::getBulletStartingPosition(sf::Vector2f playerFaceDirection) const
{
	if (playerFaceDirection == PH_WEST || playerFaceDirection == PH_EAST)
		return sf::Vector2f(25.f * playerFaceDirection.x, 25.f);
	if (playerFaceDirection == PH_NORTH || playerFaceDirection == PH_SOUTH)
		return sf::Vector2f(25.f, 25.f * -playerFaceDirection.y);
	else if (playerFaceDirection == PH_NORTH_EAST || playerFaceDirection == PH_SOUTH_EAST)
		return sf::Vector2f(25.f, 25.f);
	else if (playerFaceDirection == PH_NORTH_WEST || playerFaceDirection == PH_SOUTH_WEST)
		return sf::Vector2f(-25.f, 25.f);
	else
		return sf::Vector2f(0.f, 0.f);
}

void GunAttacks::tagEnemiesInGunAttackArea(sf::Vector2f playerFaceDirection, FloatRect playerBody, sf::Vector2f gunSize,
                                           float range, float deflectionAngle) const
{
	FloatRect attackArea(playerBody.getCenter(), sf::Vector2f(0.f, 0.f));
	if (playerFaceDirection == PH_WEST)
		attackArea.left += range * playerFaceDirection.x;
	else if (playerFaceDirection == PH_NORTH)
		attackArea.top += range * playerFaceDirection.y;

	if (playerFaceDirection == PH_WEST || playerFaceDirection == PH_EAST)
	{
		attackArea.width = range;
		attackArea.height = range * std::tan(Math::degreesToRadians(deflectionAngle)) * 2.f;

		attackArea.left += (playerBody.width / 2.f + gunSize.x / 2.f) * playerFaceDirection.x;;
		attackArea.top -= attackArea.height / 2.f;
	}
	else if (playerFaceDirection == PH_SOUTH || playerFaceDirection == PH_NORTH)
	{
		attackArea.width = range * std::tan(Math::degreesToRadians(deflectionAngle)) * 2.f;
		attackArea.height = range;

		attackArea.left -= attackArea.width / 2.f;
		attackArea.top += (playerBody.height / 2.f + gunSize.y / 2.f) * playerFaceDirection.y;
	}
	else if (PH_IS_DIAGONAL(playerFaceDirection))
	{
		attackArea.setSize(sf::Vector2f(range, range));
		if (PH_IS_X_AXIS_POSITIVE(playerFaceDirection))
			attackArea.left += playerBody.width / 2.f;
		else if (PH_IS_X_AXIS_NEGATIVE(playerFaceDirection))
			attackArea.left -= range + playerBody.width / 2.f;
		if (PH_IS_Y_AXIS_POSITIVE(playerFaceDirection))
			attackArea.top += playerBody.height / 2.f;
		else if (PH_IS_Y_AXIS_NEGATIVE(playerFaceDirection))
			attackArea.top -= range + playerBody.height / 2.f;
	}

	auto enemiesView = mRegistry.view<component::Killable, component::BodyRect>(entt::exclude<component::Player>);
	for (auto enemy : enemiesView)
	{
		const auto& enemyBody = enemiesView.get<component::BodyRect>(enemy);
		if (attackArea.doPositiveRectsIntersect(enemyBody.rect))
			mRegistry.assign<component::InPlayerGunAttackArea>(enemy);
	}
}

std::vector<sf::Vector2f> GunAttacks::performShoot(sf::Vector2f playerFaceDirection, sf::Vector2f startingBulletPos,
                                                   float range, float deflectionAngle, int damage, int numberOfBullets) const
{
	auto enemiesWithDamageTag = mRegistry.view<component::DamageTag, component::InPlayerGunAttackArea, component::Killable, component::BodyRect>();
	auto enemies = mRegistry.view<component::InPlayerGunAttackArea, component::Killable, component::BodyRect>();
	std::vector<sf::Vector2f> shotsEndingPositions;
	const int bulletShiftValue = 5;

	for (int i = 0; i < numberOfBullets; ++i)
	{
		sf::Vector2f direction = getBulletDirection(playerFaceDirection, deflectionAngle);
		sf::Vector2f currentBulletPos = startingBulletPos;
		int bulletTravelledDist = 1;

		while (bulletTravelledDist < range)
		{
			bool didBulletHitEnemy = false;
			for (const auto enemy : enemies)
			{
				const auto& bodyRect = enemies.get<component::BodyRect>(enemy);
				if (bodyRect.rect.contains(currentBulletPos))
				{
					if (enemiesWithDamageTag.contains(enemy))
					{
						auto& damageTagValue = enemiesWithDamageTag.get<component::DamageTag>(enemy);
						damageTagValue.amountOfDamage += damage;
					}
					else
						mRegistry.assign<component::DamageTag>(enemy, damage);
					didBulletHitEnemy = true;
					break;
				}
			}

			if (didBulletHitEnemy)
				break;

			bulletTravelledDist += bulletShiftValue;
			currentBulletPos = getCurrentPosition(direction, startingBulletPos, bulletTravelledDist);
		}
		shotsEndingPositions.emplace_back(currentBulletPos);
	}

	return shotsEndingPositions;
}

sf::Vector2f GunAttacks::getBulletDirection(sf::Vector2f playerFaceDirection, float deflection) const
{
	deflection = Random::generateNumber(-deflection, deflection);
	const float deflectionFactor = deflection / -90.f;
	sf::Vector2f deflectedBulletDirection = playerFaceDirection;

	if (playerFaceDirection == PH_NORTH)
	{
		if (deflectionFactor < 0.f)
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
	else if (playerFaceDirection == PH_EAST)
	{
		if (deflectionFactor < 0.f)
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
	else if (playerFaceDirection == PH_SOUTH)
		if (deflectionFactor < 0.f)
		{
			deflectedBulletDirection.x += -deflectionFactor;
			deflectedBulletDirection.y += deflectionFactor;
		}
		else
		{
			deflectedBulletDirection.x += -deflectionFactor;
			deflectedBulletDirection.y += -deflectionFactor;
		}
	else if (playerFaceDirection == PH_WEST)
		if (deflectionFactor < 0.f)
		{
			deflectedBulletDirection.x += -deflectionFactor;
			deflectedBulletDirection.y += -deflectionFactor;
		}
		else
		{
			deflectedBulletDirection.x += deflectionFactor;
			deflectedBulletDirection.y += -deflectionFactor;
		}

	else if (playerFaceDirection == PH_NORTH_WEST)
	{
		if (deflectionFactor < 0.f)
			deflectedBulletDirection.y += -deflectionFactor;
		else
			deflectedBulletDirection.x += deflectionFactor;
	}
	else if (playerFaceDirection == PH_NORTH_EAST)
	{
		if (deflectionFactor < 0.f)
			deflectedBulletDirection.x += deflectionFactor;
		else
			deflectedBulletDirection.y += deflectionFactor;
	}
	else if (playerFaceDirection == PH_SOUTH_WEST)
	{
		if (deflectionFactor < 0.f)
			deflectedBulletDirection.x += -deflectionFactor;
		else
			deflectedBulletDirection.y += -deflectionFactor;
	}
	else if (playerFaceDirection == PH_SOUTH_EAST)
	{
		if (deflectionFactor < 0.f)
			deflectedBulletDirection.y += deflectionFactor;
		else
			deflectedBulletDirection.x += -deflectionFactor;
	}

	return deflectedBulletDirection;
}

sf::Vector2f GunAttacks::getCurrentPosition(sf::Vector2f bulletDirection, sf::Vector2f startingPos, int bulletDistance) const
{
	sf::Vector2f newPosition;
	newPosition.x = startingPos.x + bulletDirection.x * bulletDistance;
	newPosition.y = startingPos.y + bulletDirection.y * bulletDistance;
	return newPosition;
}

void GunAttacks::clearInGunAttackAreaTags() const
{
	auto enemiesInAreaAttackView = mRegistry.view<component::InPlayerGunAttackArea>();
	for (auto enemy : enemiesInAreaAttackView)
		mRegistry.remove<component::InPlayerGunAttackArea>(enemy);
}

void GunAttacks::createShotImage(sf::Vector2f shotsStartingPosition, const std::vector<sf::Vector2f>& shotsEndingPosition,
                                 const std::string& soundFilename) const
{
	for (auto shot : shotsEndingPosition)
	{
		auto entity = mRegistry.create();
		mRegistry.assign<component::LastingShot>(entity, shotsStartingPosition, shot);
		mRegistry.assign<component::Lifetime>(entity, .05f);
	}

	auto soundEntity = mRegistry.create();
	mRegistry.assign<component::AmbientSound>(soundEntity, soundFilename.c_str());
	mRegistry.assign<component::Lifetime>(soundEntity, .05f);
}

void GunAttacks::handleLastingBullets() const
{
	const auto lastingShotsView = mRegistry.view<component::LastingShot>();

	lastingShotsView.each([](const component::LastingShot& lastingShotDetails) 
	{
		Renderer::submitLine(sf::Color(230, 160, 0), sf::Color(250, 250, 200),
			lastingShotDetails.startingShotPos, lastingShotDetails.endingShotPos, 1.f);
	});
}

void GunAttacks::shakeCamera(float power) const
{
	auto playerCameraView = mRegistry.view<component::Player>();
	for(auto player : playerCameraView)
	{
		component::CameraShake shake;
		shake.duration = 0.5f;
		shake.magnitude = power;
		shake.smooth = false;
		mRegistry.assign_or_replace<component::CameraShake>(player, shake);
	}
}

}
