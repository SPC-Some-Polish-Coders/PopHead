#pragma once

#include "pendingGunAttacks.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

void PendingGunAttacks::update(float seconds)
{
	const auto gunAttackerView = mRegistry.view<component::GunAttacker, component::BodyRect, component::Player, component::FaceDirection>();
	for (const auto& gunAttacker : gunAttackerView)
	{
		auto& playerGunAttack = gunAttackerView.get<component::GunAttacker>(gunAttacker);
		auto& playerFaceDirection = gunAttackerView.get<component::FaceDirection>(gunAttacker);

		if (playerGunAttack.isTryingToAttack)
		{
			playerGunAttack.isTryingToAttack = false;
			if (!playerGunAttack.canAttack)
				return;

			auto& gunView = mRegistry.view<component::PlayerGun, component::BodyRect>();
			for (const auto& gun : gunView)
			{
				auto gunBody = gunView.get<component::BodyRect>(gun);
				const auto& playerBody = gunAttackerView.get<component::BodyRect>(gunAttacker);


				sf::Vector2f shift = gunBody.rect.getCenter();
				sf::Vector2f startingBulletPos = playerBody.rect.getTopLeft() + getGunPosition(playerFaceDirection.direction);
				shift -= startingBulletPos;
				startingBulletPos += shift;
				///////////////////////////////////////////////////////
				//temporary until I find better bullet positioning
				if (playerFaceDirection.direction == sf::Vector2f(1, 0))
					startingBulletPos += sf::Vector2f(5, -1);
				else if (playerFaceDirection.direction == sf::Vector2f(-1, 0))
					startingBulletPos += sf::Vector2f(-5, -1);
				else if (playerFaceDirection.direction == sf::Vector2f(0, 1) || playerFaceDirection.direction == sf::Vector2f(0, -1))
					startingBulletPos += sf::Vector2f(-4.5, 0);
				///////////////////////////////////////////////////////

				sf::Vector2f endingBulletPos = performShoot(playerFaceDirection.direction, startingBulletPos);
				createShotImage(startingBulletPos, endingBulletPos);

				--playerGunAttack.bullets;
			}
		}
	}
}

sf::Vector2f PendingGunAttacks::performShoot(const sf::Vector2f& playerFaceDirection, const sf::Vector2f& startingBulletPos)
{
	auto enemies = mRegistry.view<component::BodyRect, component::Killable>(entt::exclude<component::Player>);
	sf::Vector2f currentBulletPos = startingBulletPos;
	int bulletTravelledDist = 1;
	const int range = 250;

	while (bulletTravelledDist < range)
	{
		for (const auto enemy : enemies)
		{
			const auto& bodyRect = enemies.get<component::BodyRect>(enemy);
			if (bodyRect.rect.contains(currentBulletPos))
			{
				mRegistry.assign<component::DamageTag>(enemy, 50);
				return currentBulletPos;
			}
		}
		bulletTravelledDist += 5;
		currentBulletPos = getCurrentPosition(playerFaceDirection, startingBulletPos, bulletTravelledDist);
	}

	return currentBulletPos;
}

sf::Vector2f PendingGunAttacks::getCurrentPosition(const sf::Vector2f& playerFaceDirection, const sf::Vector2f& startingPos, const int bulletDistance) const
{
	sf::Vector2f newPosition;
	newPosition.x = startingPos.x + playerFaceDirection.x * bulletDistance;
	newPosition.y = startingPos.y + playerFaceDirection.y * bulletDistance;
	return newPosition;
}

void PendingGunAttacks::createShotImage(const sf::Vector2f& startingPosition, const sf::Vector2f& endingPosition)
{
	auto entity = mRegistry.create();
	mRegistry.assign<component::LastingShot>(entity, startingPosition, endingPosition);
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

}
