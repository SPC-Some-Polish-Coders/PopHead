#include "gunPositioning.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void GunPositioning::update(float seconds)
	{
		auto playerView = mRegistry.view<component::GunAttacker, component::Player, component::BodyRect, component::FaceDirection>();
		for (auto player : playerView)
		{
			const auto& playerFaceDirection = playerView.get<component::FaceDirection>(player);
			const auto& gunAttacker = playerView.get<component::GunAttacker>(player);
			const auto& playerBody = playerView.get<component::BodyRect>(player);

			auto gunView = mRegistry.view<component::PlayerGun, component::TextureRect, component::BodyRect>();
			for (auto gun : gunView)
			{
				auto& gunTextureBody = gunView.get<component::TextureRect>(gun);
				auto& playerGun = gunView.get<component::PlayerGun>(gun);
				auto& gunBody = gunView.get<component::BodyRect>(gun);

				if (playerGun.cooldownSinceLastShot <= 0.f)
					mRegistry.assign_or_replace<component::HiddenForRenderer>(gun);
				else
					playerGun.cooldownSinceLastShot -= seconds;

				if (gunAttacker.isTryingToAttack)
				{
					playerGun.cooldownSinceLastShot = playerGun.timeBeforeHiding;
					if(mRegistry.has<component::HiddenForRenderer>(gun))
						mRegistry.remove<component::HiddenForRenderer>(gun);

					if(gunAttacker.bullets > 0 && gunAttacker.cooldownSinceLastShoot <= 0.f)
						updateGunTextureRect(playerFaceDirection.direction, gunTextureBody.rect);
					else 
						updateGunTextureRect(playerFaceDirection.direction, gunTextureBody.rect, 16);
				}
				else
					updateGunTextureRect(playerFaceDirection.direction, gunTextureBody.rect, 16);

				updateGunSpriteFlipping(playerFaceDirection.direction, gunBody.rect);
				updateGunSpritePosition(playerFaceDirection.direction, playerBody.rect.getTopLeft(), gunBody.rect);
			}
		}
	}

	void GunPositioning::updateGunTextureRect(const sf::Vector2f& playerFaceDirection, IntRect& textureBody, const int offsetX)
	{
		if (playerFaceDirection == sf::Vector2f(1.f, 0.f) || playerFaceDirection == sf::Vector2f(-1.f, 0.f))
			textureBody = IntRect( offsetX, 0, 13, 8 );
		else if (playerFaceDirection == sf::Vector2f(0.f, 1.f) || playerFaceDirection == sf::Vector2f(0.f, -1.f))
			textureBody = IntRect(offsetX, 10, 13, 11 );
		else if (playerFaceDirection == sf::Vector2f(-0.7f, -0.7f) || playerFaceDirection == sf::Vector2f(0.7f, -0.7f))
			textureBody = IntRect(offsetX, 21, 13, 11 );
		else if (playerFaceDirection == sf::Vector2f(-0.7f, 0.7f) || playerFaceDirection == sf::Vector2f(0.7f, 0.7f))
			textureBody = IntRect(offsetX, 34, 13, 11 );
	}

	void GunPositioning::updateGunSpriteFlipping(const sf::Vector2f& playerFaceDirection, FloatRect& gunBody)
	{
		// if (playerFaceDirection.x < 0)
		//	gunBody.width = -gunBody.width;
		//else if (playerFaceDirection == sf::Vector2f(0, 1))
		//	gunBody.height = -gunBody.height;
	}

	void GunPositioning::updateGunSpritePosition(const sf::Vector2f& playerFaceDirection, const sf::Vector2f& playerPosition, FloatRect& gunBody)
	{
		const sf::Vector2f rightHandPosition = getRightHandPosition(playerFaceDirection) + playerPosition;
		const sf::Vector2f gunBodySize = gunBody.getSize();
		if (playerFaceDirection == sf::Vector2f(1.f, 0.f))
			gunBody = FloatRect(rightHandPosition + sf::Vector2f(0.f, -3.f), gunBodySize);
		else if (playerFaceDirection == sf::Vector2f(-1.f, 0.f))
			gunBody = FloatRect(rightHandPosition + sf::Vector2f(3.f, -3.f), gunBodySize);
		else if (playerFaceDirection == sf::Vector2f(0.f, -1.f))
			gunBody = FloatRect(rightHandPosition + sf::Vector2f(-3.f, 1.f), gunBodySize);
		else if (playerFaceDirection == sf::Vector2f(0.f, 1.f))
			gunBody = FloatRect(rightHandPosition + sf::Vector2f(-3.f, 9.f), gunBodySize);
		else if (playerFaceDirection == sf::Vector2f(-0.7f, -0.7f))
			gunBody = FloatRect(rightHandPosition + sf::Vector2f(0.f, -8.f), gunBodySize);
		else if (playerFaceDirection == sf::Vector2f(0.7f, -0.7f))
			gunBody = FloatRect(rightHandPosition + sf::Vector2f(0.f, -8.f), gunBodySize);
		else if (playerFaceDirection == sf::Vector2f(-0.7f, 0.7f))
			gunBody = FloatRect(rightHandPosition + sf::Vector2f(3.f, -3.f), gunBodySize);
		else if (playerFaceDirection == sf::Vector2f(0.7f, 0.7f))
			gunBody = FloatRect(rightHandPosition + sf::Vector2f(-3.f, -1.f), gunBodySize);
		else
			gunBody = FloatRect(rightHandPosition, gunBodySize);
	}

	sf::Vector2f GunPositioning::getRightHandPosition(const sf::Vector2f& playerFaceDirection)
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
			return { 16, 7 };
	}
}
