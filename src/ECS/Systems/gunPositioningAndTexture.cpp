#include "gunPositioningAndTexture.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void GunPositioningAndTexture::update(float dt)
	{
		auto playerView = mRegistry.view<component::GunAttacker, component::Player, component::BodyRect, component::FaceDirection>();
		for (auto player : playerView)
		{
			const auto& [playerFaceDirection, gunAttacker, playerBody] = playerView.get<component::FaceDirection, component::GunAttacker, component::BodyRect>(player);

			updateGunPosition(playerFaceDirection.direction, playerBody.rect);
			updateTexture(dt, playerFaceDirection.direction, gunAttacker.isTryingToAttack, gunAttacker.canAttack);
		}
	}

	void GunPositioningAndTexture::updateGunPosition(const sf::Vector2f& playerFaceDirection, const FloatRect& playerBody)
	{
		auto gunView = mRegistry.view<component::CurrentGun, component::BodyRect>();
		for (auto gun : gunView)
		{
			auto& playerGun = gunView.get<component::CurrentGun>(gun);
			auto& gunBody = gunView.get<component::BodyRect>(gun);

			updateGunSpriteFlipping(playerFaceDirection, gunBody.rect);
			updateGunSpritePosition(playerFaceDirection, playerBody.getTopLeft(), gunBody.rect);
		}
	}

	void GunPositioningAndTexture::updateGunSpriteFlipping(const sf::Vector2f& playerFaceDirection, FloatRect& gunBody)
	{
		auto originalSize = gunBody.getSize();
		if (originalSize.x < 0.f)
			originalSize.x = -originalSize.x;
		if (originalSize.y < 0.f)
			originalSize.y = -originalSize.y;

		if (playerFaceDirection.x < 0)
		{
			gunBody.width = -originalSize.x;
			gunBody.height = originalSize.y;
		}
		else if (playerFaceDirection == sf::Vector2f(0, 1))
		{
			gunBody.width = originalSize.x;
			gunBody.height = -originalSize.y;
		}
		else
		{
			gunBody.width = originalSize.x;
			gunBody.height = originalSize.y;
		}
	}

	void GunPositioningAndTexture::updateGunSpritePosition(const sf::Vector2f& playerFaceDirection, const sf::Vector2f& playerPosition,
	                                                       FloatRect& gunBody)
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

	sf::Vector2f GunPositioningAndTexture::getRightHandPosition(const sf::Vector2f& playerFaceDirection)
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

	void GunPositioningAndTexture::updateTexture(float dt, const sf::Vector2f& playerFaceDirection, bool wantToAttack, bool canAttack)
	{
		auto gunView = mRegistry.view<component::CurrentGun, component::TextureRect>();
		for (auto gun : gunView)
		{
			auto& [gunTextureBody, playerGun] = gunView.get<component::TextureRect, component::CurrentGun>(gun);

			int offsetX = 16;
			if (wantToAttack)
			{
				playerGun.cooldownSinceLastShot = playerGun.timeBeforeHiding;
				if (canAttack)
					offsetX = 0;
			}

			bool shouldHide = true;
			if (playerGun.cooldownSinceLastShot > 0.f)
			{
				shouldHide = false;
				playerGun.cooldownSinceLastShot -= dt;
			}

			if (shouldHide)
				mRegistry.assign_or_replace<component::HiddenForRenderer>(gun);
			else
				if (mRegistry.has<component::HiddenForRenderer>(gun))
					mRegistry.remove<component::HiddenForRenderer>(gun);

			if (playerFaceDirection == sf::Vector2f(1.f, 0.f) || playerFaceDirection == sf::Vector2f(-1.f, 0.f))
				gunTextureBody.rect = IntRect(offsetX, 0, 13, 8);
			else if (playerFaceDirection == sf::Vector2f(0.f, 1.f) || playerFaceDirection == sf::Vector2f(0.f, -1.f))
				gunTextureBody.rect = IntRect(offsetX, 10, 13, 11);
			else if (playerFaceDirection == sf::Vector2f(-0.7f, -0.7f) || playerFaceDirection == sf::Vector2f(0.7f, -0.7f))
				gunTextureBody.rect = IntRect(offsetX, 21, 13, 11);
			else if (playerFaceDirection == sf::Vector2f(-0.7f, 0.7f) || playerFaceDirection == sf::Vector2f(0.7f, 0.7f))
				gunTextureBody.rect = IntRect(offsetX, 34, 13, 11);
		}
	}
}
