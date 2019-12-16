#pragma once

#include "pendingGunAttacks.hpp"
#include "ECS/Components/audioComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Utilities/random.hpp"

#include <iostream>

namespace ph::system {

void PendingGunAttacks::update(float dt)
{
	handleLastingBullets();
	handlePendingGunAttacks();
}

void PendingGunAttacks::handlePendingGunAttacks()
{
	auto gunAttackerView = mRegistry.view<component::GunAttacker, component::BodyRect, component::Player, component::FaceDirection>();
	for (const auto& gunAttacker : gunAttackerView)
	{
		auto& playerGunAttack = gunAttackerView.get<component::GunAttacker>(gunAttacker);
		auto& playerFaceDirection = gunAttackerView.get<component::FaceDirection>(gunAttacker);

		if (playerGunAttack.isTryingToAttack)
		{
			playerGunAttack.isTryingToAttack = false;
			if (!playerGunAttack.canAttack)
				return;

			auto gunView = mRegistry.view<component::CurrentGun, component::BodyRect, component::GunProperties>();
			for (const auto& gun : gunView)
			{
				const auto& [gunBody, gunProperties] = gunView.get<component::BodyRect, component::GunProperties>(gun);
				const auto& playerBody = gunAttackerView.get<component::BodyRect>(gunAttacker);

				sf::Vector2f shift = gunBody.rect.getCenter();
				sf::Vector2f startingBulletPos = playerBody.rect.getTopLeft() + getGunPosition(playerFaceDirection.direction);
				shift -= startingBulletPos;
				startingBulletPos += getCorrectedBulletStartingPosition(playerFaceDirection.direction) + shift;

				sf::Vector2f endingBulletPos = performShoot(playerFaceDirection.direction, startingBulletPos, gunProperties.range, gunProperties.deflectionAngle, gunProperties.damage);
				createShotImage(startingBulletPos, endingBulletPos);

				playerGunAttack.bullets -= gunProperties.numberOfBullets;
			}
		}
	}
}

sf::Vector2f PendingGunAttacks::getCorrectedBulletStartingPosition(const sf::Vector2f& playerFaceDirection) const
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

sf::Vector2f PendingGunAttacks::performShoot(const sf::Vector2f& playerFaceDirection, const sf::Vector2f& startingBulletPos, float range, float deflectionAngle, int damage)
{
	auto enemies = mRegistry.view<component::BodyRect, component::Killable>(entt::exclude<component::Player>);
	sf::Vector2f currentBulletPos = startingBulletPos;
	sf::Vector2f direction = getBulletDirection(playerFaceDirection, deflectionAngle);
	int bulletTravelledDist = 1;

	while (bulletTravelledDist < range)
	{
		for (const auto enemy : enemies)
		{
			const auto& bodyRect = enemies.get<component::BodyRect>(enemy);
			if (bodyRect.rect.contains(currentBulletPos))
			{
				mRegistry.assign<component::DamageTag>(enemy, damage);
				return currentBulletPos;
			}
		}
		bulletTravelledDist += 5;
		currentBulletPos = getCurrentPosition(direction, startingBulletPos, bulletTravelledDist);
	}
	return currentBulletPos;
}

sf::Vector2f PendingGunAttacks::getCurrentPosition(const sf::Vector2f& bulletDirection, const sf::Vector2f& startingPos, const int bulletDistance) const
{
	sf::Vector2f newPosition;
	newPosition.x = startingPos.x + bulletDirection.x * bulletDistance;
	newPosition.y = startingPos.y + bulletDirection.y * bulletDistance;
	return newPosition;
}

sf::Vector2f PendingGunAttacks::getBulletDirection(const sf::Vector2f& playerFaceDirection, float deflection) const
{
	deflection = Random::generateNumber(-deflection, deflection);
	const float deflectionFactor= deflection / 90.f;
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
		deflectedBulletDirection += sf::Vector2f(deflectionFactor, -deflectionFactor);
	else if (playerFaceDirection == sf::Vector2f(0.7f, -0.7f)) // up-right
		deflectedBulletDirection += sf::Vector2f(deflectionFactor, deflectionFactor);
	else if (playerFaceDirection == sf::Vector2f(-0.7f, 0.7f)) //down-left
		deflectedBulletDirection += sf::Vector2f(-deflectionFactor, -deflectionFactor);
	else if (playerFaceDirection == sf::Vector2f(0.7f, 0.7f)) // down-right
		deflectedBulletDirection += sf::Vector2f(deflectionFactor, -deflectionFactor);

	return deflectedBulletDirection;
}

void PendingGunAttacks::createShotImage(const sf::Vector2f& startingPosition, const sf::Vector2f& endingPosition)
{
	auto entity = mRegistry.create();
	mRegistry.assign<component::LastingShot>(entity, startingPosition, endingPosition);
	mRegistry.assign<component::AmbientSound>(entity, "sounds/pistolShot.ogg");
	mRegistry.assign<component::Lifetime>(entity, .05f);
}

sf::Vector2f PendingGunAttacks::getGunPosition(const sf::Vector2f& playerFaceDirection) const
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

void PendingGunAttacks::handleLastingBullets()
{
	const auto lastingShotsView = mRegistry.view<component::LastingShot>();

	lastingShotsView.each([](const component::LastingShot& lastingShotDetails) {
		Renderer::submitLine(sf::Color(230, 160, 0), sf::Color(250, 250, 200), lastingShotDetails.startingShotPos, lastingShotDetails.endingShotPos, 1.f);
		});
}

}
