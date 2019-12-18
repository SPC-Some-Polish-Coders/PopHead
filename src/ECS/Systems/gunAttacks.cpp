#pragma once

#include "gunAttacks.hpp"
#include "ECS/Components/audioComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Utilities/random.hpp"

namespace ph::system {

void GunAttacks::update(float dt)
{
	handlePendingGunAttacks();
	handleLastingBullets();
}

void GunAttacks::handlePendingGunAttacks()
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

				sf::Vector2f shift = gunBody.rect.getCenter();
				sf::Vector2f startingBulletPosition = playerBody.rect.getTopLeft() + getGunPosition(playerFaceDirection.direction);
				shift -= startingBulletPosition;
				startingBulletPosition += getCorrectedBulletStartingPosition(playerFaceDirection.direction) + shift;

				std::vector<sf::Vector2f> shotsEndingPositions = performShoot(playerFaceDirection.direction, startingBulletPosition, gunProperties.range, gunProperties.deflectionAngle, gunProperties.damage, gunProperties.numberOfBullets);
				createShotImage(startingBulletPosition, shotsEndingPositions, gunProperties.shotSoundFilepath);

				playerGunAttack.bullets -= gunProperties.numberOfBullets;
			}
		}
	}
}

sf::Vector2f GunAttacks::getGunPosition(const sf::Vector2f& playerFaceDirection) const
{
	if (playerFaceDirection == sf::Vector2f(1, 0))
		return { 16, 7 };
	else if (playerFaceDirection == sf::Vector2f(-1, 0))
		return { -2, 7 };
	else if (playerFaceDirection == sf::Vector2f(0, 1))
		return { 10, 18 };
	else if (playerFaceDirection == sf::Vector2f(0, -1))
		return { 9, -12 };
	else if (playerFaceDirection == sf::Vector2f(0.7f, -0.7f))
		return { 15, -5 };
	else if (playerFaceDirection == sf::Vector2f(-0.7f, -0.7f))
		return { -1, -1 };
	else if (playerFaceDirection == sf::Vector2f(0.7f, 0.7f))
		return { 17, 17 };
	else if (playerFaceDirection == sf::Vector2f(-0.7f, 0.7f))
		return { -3, 17 };
	else
		return { 0, 0 };
}

sf::Vector2f GunAttacks::getCorrectedBulletStartingPosition(const sf::Vector2f& playerFaceDirection) const
{
	if (playerFaceDirection == sf::Vector2f(1, 0))
		return sf::Vector2f(5, -1);
	else if (playerFaceDirection == sf::Vector2f(-1, 0))
		return sf::Vector2f(-5, -1);
	else if (playerFaceDirection == sf::Vector2f(0, 1) || playerFaceDirection == sf::Vector2f(0, -1))
		return sf::Vector2f(-4.5, 0);
	else
		return sf::Vector2f(0.f, 0.f);
}

std::vector<sf::Vector2f> GunAttacks::performShoot(const sf::Vector2f& playerFaceDirection, const sf::Vector2f& startingBulletPos, float range, float deflectionAngle, int damage, int numberOfBullets)
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

void GunAttacks::createShotImage(const sf::Vector2f shotsStartingPosition, const std::vector<sf::Vector2f>& shotsEngingPosition, const std::string& soundFilename)
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

void GunAttacks::handleLastingBullets()
{
	const auto lastingShotsView = mRegistry.view<component::LastingShot>();

	lastingShotsView.each([](const component::LastingShot& lastingShotDetails) {
		Renderer::submitLine(sf::Color(230, 160, 0), sf::Color(250, 250, 200), lastingShotDetails.startingShotPos, lastingShotDetails.endingShotPos, 1.f);
		});
}

}
