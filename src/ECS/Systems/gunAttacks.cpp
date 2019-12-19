#pragma once

#include "gunAttacks.hpp"
#include "ECS/Components/audioComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Events/actionEventManager.hpp"
#include "Renderer/renderer.hpp"
#include "Utilities/random.hpp"

namespace ph::system {

void GunAttacks::update(float dt)
{
	handlePendingGunAttacks();
	handleLastingBullets();
	handleChangingOfWeapon();
}

void GunAttacks::handleChangingOfWeapon() const
{
	auto currentGunView = mRegistry.view<component::CurrentGun>();
	auto otherGunsView = mRegistry.view<component::GunProperties>(entt::exclude<component::CurrentGun>);
	static int choiceNumber = 0;

	if (ActionEventManager::isActionPressed("changeWeapon"))
	{
		++choiceNumber;
		if (choiceNumber > otherGunsView.size() - 1)
			choiceNumber = 0;

		for (auto currentGun : currentGunView)
		{
			mRegistry.assign_or_replace<component::HiddenForRenderer>(currentGun);
			mRegistry.remove<component::CurrentGun>(currentGun);
		}

		int counter = 0;
		for (auto otherGun : otherGunsView)
		{
			if (counter == choiceNumber)
			{
				mRegistry.assign<component::CurrentGun>(otherGun);
				return;
			}
			++counter;
		}
	}

}

void GunAttacks::handlePendingGunAttacks() const
{
	auto gunAttackerView = mRegistry.view<component::Player, component::GunAttacker, component::FaceDirection, component::BodyRect>();
	for (const auto& gunAttacker : gunAttackerView)
	{
		auto& playerGunAttack = gunAttackerView.get<component::GunAttacker>(gunAttacker);
		auto& playerFaceDirection = gunAttackerView.get<component::FaceDirection>(gunAttacker);

		if (playerGunAttack.isTryingToAttack)
		{
			playerGunAttack.isTryingToAttack = false;
			if (!playerGunAttack.canAttack)
				return;

			auto gunView = mRegistry.view<component::CurrentGun, component::GunProperties, component::BodyRect>();
			for (const auto& gun : gunView)
			{
				const auto& [gunBody, gunProperties] = gunView.get<component::BodyRect, component::GunProperties>(gun);
				const auto& playerBody = gunAttackerView.get<component::BodyRect>(gunAttacker);

				sf::Vector2f startingBulletPosition = gunBody.rect.getCenter() + getCorrectedBulletStartingPosition(playerFaceDirection.direction, gunBody.rect.getSize());

				std::vector<sf::Vector2f> shotsEndingPositions = performShoot(playerFaceDirection.direction, startingBulletPosition, gunProperties.range, gunProperties.deflectionAngle, gunProperties.damage, gunProperties.numberOfBullets);
				createShotImage(startingBulletPosition, shotsEndingPositions, gunProperties.shotSoundFilepath);

				playerGunAttack.bullets -= gunProperties.numberOfBullets;
			}
		}
	}
}

sf::Vector2f GunAttacks::getCorrectedBulletStartingPosition(const sf::Vector2f& playerFaceDirection, sf::Vector2f gunSize) const
{
	if (playerFaceDirection == sf::Vector2f(1, 0) || playerFaceDirection == sf::Vector2f(-1, 0))
		return sf::Vector2f(gunSize.x/2, -2.f);
	else if (playerFaceDirection == sf::Vector2f(0, 1) || playerFaceDirection == sf::Vector2f(0, -1))
		return sf::Vector2f(-4.7f, -gunSize.y/2);
	else if(playerFaceDirection.y > 0.f)
		return sf::Vector2f(gunSize.x/2, gunSize.y/2);
	else
		return sf::Vector2f(gunSize.x/2, -gunSize.y/2);
}

std::vector<sf::Vector2f> GunAttacks::performShoot(const sf::Vector2f& playerFaceDirection, const sf::Vector2f& startingBulletPos, float range, float deflectionAngle, int damage, int numberOfBullets) const
{
	auto enemiesWithDamageTag = mRegistry.view<component::DamageTag, component::Killable, component::BodyRect>(entt::exclude < component::Player>);
	auto enemies = mRegistry.view< component::Killable, component::BodyRect>(entt::exclude<component::Player>);
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


sf::Vector2f GunAttacks::getBulletDirection(const sf::Vector2f& playerFaceDirection, float deflection) const
{
	deflection = Random::generateNumber(-deflection, deflection);
	const float deflectionFactor = deflection / -90.f;
	sf::Vector2f deflectedBulletDirection = playerFaceDirection;

	if (playerFaceDirection == sf::Vector2f(0.f, -1.f)) //up 
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
	else if (playerFaceDirection == sf::Vector2f(1.f, 0.f)) // right
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
	else if (playerFaceDirection == sf::Vector2f(0.f, 1.f)) //down
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
	else if (playerFaceDirection == sf::Vector2f(-1.f, 0.f)) //left
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

	else if (playerFaceDirection == sf::Vector2f(-0.7f, -0.7f)) // up-left
	{
		if (deflectionFactor < 0.f)
			deflectedBulletDirection.y += -deflectionFactor;
		else
			deflectedBulletDirection.x += deflectionFactor;
	}
	else if (playerFaceDirection == sf::Vector2f(0.7f, -0.7f)) // up-right
	{
		if (deflectionFactor < 0.f)
			deflectedBulletDirection.x += deflectionFactor;
		else
			deflectedBulletDirection.y += deflectionFactor;
	}
	else if (playerFaceDirection == sf::Vector2f(-0.7f, 0.7f)) //down-left
	{
		if (deflectionFactor < 0.f)
			deflectedBulletDirection.x += -deflectionFactor;
		else
			deflectedBulletDirection.y += -deflectionFactor;
	}
	else if (playerFaceDirection == sf::Vector2f(0.7f, 0.7f)) // down-right
	{
		if (deflectionFactor < 0.f)
			deflectedBulletDirection.y += deflectionFactor;
		else
			deflectedBulletDirection.x += -deflectionFactor;
	}

	return deflectedBulletDirection;
}

sf::Vector2f GunAttacks::getCurrentPosition(const sf::Vector2f& bulletDirection, const sf::Vector2f& startingPos, const int bulletDistance) const
{
	sf::Vector2f newPosition;
	newPosition.x = startingPos.x + bulletDirection.x * bulletDistance;
	newPosition.y = startingPos.y + bulletDirection.y * bulletDistance;
	return newPosition;
}

void GunAttacks::createShotImage(const sf::Vector2f shotsStartingPosition, const std::vector<sf::Vector2f>& shotsEngingPosition, const std::string& soundFilename) const
{
	for (auto shot : shotsEngingPosition)
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

	lastingShotsView.each([](const component::LastingShot& lastingShotDetails) {
		Renderer::submitLine(sf::Color(230, 160, 0), sf::Color(250, 250, 200), lastingShotDetails.startingShotPos, lastingShotDetails.endingShotPos, 1.f);
		});
}

}
