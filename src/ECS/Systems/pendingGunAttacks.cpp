#pragma once

#include "pendingGunAttacks.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

#include "Events/actionEventManager.hpp"

namespace ph::system {

void PendingGunAttacks::update(float seconds)
{
	const auto gunAttackerView = mRegistry.view<component::GunAttacker, component::BodyRect>();
	for (const auto& gunAttacker : gunAttackerView)
	{
		auto& playerGunAttack = gunAttackerView.get<component::GunAttacker>(gunAttacker);
		if (playerGunAttack.isTryingToAttack)
		{
			if (!canShoot(playerGunAttack.bullets)) 
				return;

			setPlayerFacePosition();
			const auto& playerBody = gunAttackerView.get<component::BodyRect>(gunAttacker);
			const sf::Vector2f startingBulletPos = playerBody.rect.getCenter() + getGunPosition();
			performShoot(startingBulletPos);	

			--playerGunAttack.bullets;
			playerGunAttack.isTryingToAttack = false;
		}
	}
}

void PendingGunAttacks::setPlayerFacePosition()
{
	const auto playerView = mRegistry.view<component::FaceDirection, component::Player>();
	for (auto player : playerView)
	{
		const auto& playerFaceDirection = playerView.get<component::FaceDirection>(player);
		mPlayerFaceDirection = playerFaceDirection.direction;
	}
}

bool PendingGunAttacks::canShoot(int numOfBullets) const
{
	return numOfBullets > 0;
}

void PendingGunAttacks::performShoot(const sf::Vector2f& startingBulletPos)
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
				return;
			}
		}
		bulletTravelledDist += 5;
		currentBulletPos = getCurrentPosition(startingBulletPos, bulletTravelledDist);
	}
}

sf::Vector2f PendingGunAttacks::getCurrentPosition(const sf::Vector2f& startingPos, const int bulletDistance) const
{
	sf::Vector2f newPosition;
	newPosition.x = startingPos.x + mPlayerFaceDirection.x * bulletDistance;
	newPosition.y = startingPos.y + mPlayerFaceDirection.y * bulletDistance;
	return newPosition;
}

sf::Vector2f PendingGunAttacks::getGunPosition() const
{
	if (mPlayerFaceDirection == sf::Vector2f(1, 0))
		return { 16, 7 };
	else if (mPlayerFaceDirection == sf::Vector2f(-1, 0))
		return { -2, 7 };
	else if (mPlayerFaceDirection == sf::Vector2f(0, 1))
		return { 10, 18 };
	else if (mPlayerFaceDirection == sf::Vector2f(0, -1))
		return { 9, -12 };
	else if (mPlayerFaceDirection == sf::Vector2f(0.7f, -0.7f))
		return { 15, -5 };
	else if (mPlayerFaceDirection == sf::Vector2f(-0.7f, -0.7f))
		return { -1, -1 };
	else if (mPlayerFaceDirection == sf::Vector2f(0.7f, 0.7f))
		return { 17, 17 };
	else if (mPlayerFaceDirection == sf::Vector2f(-0.7f, 0.7f))
		return { -3, 17 };
	else
		return { 0, 0 };
}

}
