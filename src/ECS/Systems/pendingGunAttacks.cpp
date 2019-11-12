#pragma once

#include "pendingGunAttacks.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

#include "Events/actionEventManager.hpp"

namespace ph::system {

void PendingGunAttacks::update(float seconds)
{
	direction = getPlayerDirection(); //NOTE: Temporary solution

	const auto gunAttackerView = mRegistry.view<component::GunAttacker, component::BodyRect>();
	for (const auto& gunAttacker : gunAttackerView)
	{
		auto& playerGunAttack = gunAttackerView.get<component::GunAttacker>(gunAttacker);
		if (playerGunAttack.isTryingToAttack)
		{
			if (!canShoot(playerGunAttack.bullets)) 
				return;

			const auto& playerBody = gunAttackerView.get<component::BodyRect>(gunAttacker);
			const sf::Vector2f startingBulletPos = playerBody.rect.getCenter() + getGunPosition();
			performShoot(startingBulletPos);	

			--playerGunAttack.bullets;
			playerGunAttack.isTryingToAttack = false;
		}
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
	newPosition.x = startingPos.x + direction.x * bulletDistance;
	newPosition.y = startingPos.y + direction.y * bulletDistance;
	return newPosition;
}

sf::Vector2f PendingGunAttacks::getPlayerDirection() const
{
	bool up = ActionEventManager::isActionPressed("movingUp");
	bool down = ActionEventManager::isActionPressed("movingDown");
	bool left = ActionEventManager::isActionPressed("movingLeft");
	bool right = ActionEventManager::isActionPressed("movingRight");

	const float diagonal = std::sqrt(2.f) / 2.f;

	if (up && left)  return sf::Vector2f(-diagonal, -diagonal);
	if (up && right) return sf::Vector2f(+diagonal, -diagonal);
	if (down && left)  return sf::Vector2f(-diagonal, +diagonal);
	if (down && right) return sf::Vector2f(+diagonal, +diagonal);

	if (up)    return sf::Vector2f(0.f, -1.f);
	if (down)  return sf::Vector2f(0.f, 1.f);
	if (left)  return sf::Vector2f(-1.f, 0.f);
	if (right) return sf::Vector2f(1.f, 0.f);

	return direction;
}

sf::Vector2f PendingGunAttacks::getGunPosition() const
{
	if (direction == sf::Vector2f(1, 0))
		return { 16, 7 };
	else if (direction == sf::Vector2f(-1, 0))
		return { -2, 7 };
	else if (direction == sf::Vector2f(0, 1))
		return { 10, 18 };
	else if (direction == sf::Vector2f(0, -1))
		return { 9, -12 };
	else if (direction == sf::Vector2f(0.7f, -0.7f))
		return { 15, -5 };
	else if (direction == sf::Vector2f(-0.7f, -0.7f))
		return { -1, -1 };
	else if (direction == sf::Vector2f(0.7f, 0.7f))
		return { 17, 17 };
	else if (direction == sf::Vector2f(-0.7f, 0.7f))
		return { -3, 17 };
	else
		return { 0, 0 };
}

}
