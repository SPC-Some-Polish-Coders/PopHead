#include "gunPositioningAndTexture.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/profiling.hpp"
#include <iostream>

namespace ph::system {

	void GunPositioningAndTexture::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		auto playerView = mRegistry.view<component::GunAttacker, component::Player, component::BodyRect, component::FaceDirection>();
		for (auto player : playerView)
		{
			const auto& [playerFaceDirection, gunAttacker, playerBody] = playerView.get<component::FaceDirection, component::GunAttacker, component::BodyRect>(player);

			updateTexture(dt, playerFaceDirection.direction, gunAttacker.isTryingToAttack, gunAttacker.canAttack);
			updateGunPosition(playerFaceDirection.direction, playerBody.rect);
		}
	}

	void GunPositioningAndTexture::updateTexture(float dt, sf::Vector2f playerFaceDirection, bool wantToAttack, bool canAttack) const
	{
		auto gunAttackerView = mRegistry.view<component::Player, component::GunAttacker>();
		auto gunView = mRegistry.view<component::CurrentGun, component::TextureRect>();
		for (auto gunAttacker : gunAttackerView)
		{
			auto& gunAttackerDetails = gunAttackerView.get<component::GunAttacker>(gunAttacker);
			for (auto gun : gunView)
			{
				auto& gunTextureBody = gunView.get<component::TextureRect>(gun);

				int offsetX = 16;
				if (wantToAttack)
				{
					gunAttackerDetails.timeToHide = gunAttackerDetails.timeBeforeHiding;
					if (canAttack)
						offsetX = 0;
				}

				bool shouldHide = true;
				if (gunAttackerDetails.timeToHide > 0.f)
				{
					shouldHide = false;
					gunAttackerDetails.timeToHide -= dt;
				}

				if (shouldHide)
					mRegistry.assign_or_replace<component::HiddenForRenderer>(gun);
				else
					if (mRegistry.has<component::HiddenForRenderer>(gun))
						mRegistry.remove<component::HiddenForRenderer>(gun);

				if (playerFaceDirection == sf::Vector2f(1.f, 0.f) || playerFaceDirection == sf::Vector2f(-1.f, 0.f))
					gunTextureBody.rect = IntRect(offsetX, 0, 16, 8);
				else if (playerFaceDirection == sf::Vector2f(0.f, 1.f) || playerFaceDirection == sf::Vector2f(0.f, -1.f))
					gunTextureBody.rect = IntRect(offsetX, 16, 14, 10);
				else if (playerFaceDirection == sf::Vector2f(-0.7f, -0.7f) || playerFaceDirection == sf::Vector2f(0.7f, -0.7f))
					gunTextureBody.rect = IntRect(offsetX, 32, 11, 13);
				else if (playerFaceDirection == sf::Vector2f(-0.7f, 0.7f) || playerFaceDirection == sf::Vector2f(0.7f, 0.7f))
					gunTextureBody.rect = IntRect(offsetX, 48, 13, 11);

				//if (playerFaceDirection == sf::Vector2f(1.f, 0.f) || playerFaceDirection == sf::Vector2f(-1.f, 0.f))
				//	gunTextureBody.rect = IntRect(offsetX, 0, 15, 8);
				//else if (playerFaceDirection == sf::Vector2f(0.f, 1.f) || playerFaceDirection == sf::Vector2f(0.f, -1.f))
				//	gunTextureBody.rect = IntRect(offsetX, 10, 13, 11);
				//else if (playerFaceDirection == sf::Vector2f(-0.7f, -0.7f) || playerFaceDirection == sf::Vector2f(0.7f, -0.7f))
				//	gunTextureBody.rect = IntRect(offsetX, 21, 13, 11);
				//else if (playerFaceDirection == sf::Vector2f(-0.7f, 0.7f) || playerFaceDirection == sf::Vector2f(0.7f, 0.7f))
				//	gunTextureBody.rect = IntRect(offsetX, 34, 13, 11);
			}
		}
	}

	void GunPositioningAndTexture::updateGunPosition(sf::Vector2f playerFaceDirection, const FloatRect& playerBody) const
	{
		auto gunView = mRegistry.view<component::CurrentGun, component::BodyRect>(entt::exclude<component::HiddenForRenderer>);
		for (auto gun : gunView)
		{
			auto& gunBody = gunView.get<component::BodyRect>(gun);

			sf::Vector2f newGunSize = getGunSpriteFlipping(playerFaceDirection, gunBody.rect.getSize());
			sf::Vector2f newGunPosition = getGunNewSpritePosition(playerFaceDirection, playerBody, gunBody.rect);

			gunBody.rect.width = newGunSize.x;
			gunBody.rect.height = newGunSize.y;
			gunBody.rect.left = newGunPosition.x;
			gunBody.rect.top = newGunPosition.y;
		}
	}

	sf::Vector2f GunPositioningAndTexture::getGunSpriteFlipping(sf::Vector2f playerFaceDirection, sf::Vector2f gunSize) const
	{
		auto originalSize = gunSize;
		if (originalSize.x < 0.f)
			originalSize.x = -originalSize.x;
		if (originalSize.y < 0.f)
			originalSize.y = -originalSize.y;

		if (playerFaceDirection.x < 0)
		{
			gunSize.x = -originalSize.x;
			gunSize.y = originalSize.y;
		}
		else if (playerFaceDirection == sf::Vector2f(0, 1))
		{
			gunSize.x = originalSize.x;
			gunSize.y = -originalSize.y;
		}
		else
		{
			gunSize.x = originalSize.x;
			gunSize.y = originalSize.y;
		}

		return gunSize;
	}

	sf::Vector2f GunPositioningAndTexture::getGunNewSpritePosition(sf::Vector2f playerFaceDirection, const FloatRect& playerBody,
	                                                       const FloatRect& gunBody) const
	{
		const sf::Vector2f gunOffset = getGunOffset(playerFaceDirection, gunBody.getSize(), playerBody.getSize());
		const sf::Vector2f gunNewPosition = playerBody.getCenter() + gunOffset;
		return gunNewPosition;
	}

	sf::Vector2f GunPositioningAndTexture::getGunOffset(sf::Vector2f playerFaceDirection, sf::Vector2f gunBodySize, sf::Vector2f playerBodySize) const
	{
		gunBodySize.x = std::abs(gunBodySize.y);
		gunBodySize.y = std::abs(gunBodySize.x);

		float halfOfGunWidth = gunBodySize.x / 2;
		float halfOfGunHeight = gunBodySize.y / 2;
		float halfOfPlayerWidth = playerBodySize.x / 2;
		float halfOfPlayerHeight = playerBodySize.y / 2;

		if (playerFaceDirection == sf::Vector2f(1, 0))
			return { gunBodySize.x, -halfOfGunHeight };
		else if (playerFaceDirection == sf::Vector2f(-1, 0))
			return { -halfOfPlayerWidth, -halfOfGunHeight };
		else if (playerFaceDirection == sf::Vector2f(0, 1))
			return { -2.7f, gunBodySize.y + halfOfPlayerHeight };
		else if (playerFaceDirection == sf::Vector2f(0, -1))
			return { -2.7f, -playerBodySize.y };
		else if (playerFaceDirection == sf::Vector2f(0.7f, -0.7f))
			return { gunBodySize.x, -halfOfPlayerHeight - halfOfGunHeight };
		else if (playerFaceDirection == sf::Vector2f(-0.7f, 0.7f))
			return { -gunBodySize.x, halfOfGunHeight};
		else if (playerFaceDirection == sf::Vector2f(-0.7f, -0.7f))
			return { -gunBodySize.x, -halfOfPlayerHeight - halfOfGunHeight};
		else if (playerFaceDirection == sf::Vector2f(0.7f, 0.7f))
			return { gunBodySize.x, halfOfGunHeight };
		else
			return { 0.f, 0.f };	
	}


}
