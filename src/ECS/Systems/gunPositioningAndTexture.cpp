#include "gunPositioningAndTexture.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "Utilities/profiling.hpp"
#include "Utilities/direction.hpp"

namespace ph::system {

	void GunPositioningAndTexture::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		auto playerView = mRegistry.view<component::GunAttacker, component::Player, component::BodyRect, component::FaceDirection>();
		for (auto player : playerView)
		{
			const auto& [playerFaceDirection, gunAttacker, playerBody] = playerView.get<component::FaceDirection, component::GunAttacker, component::BodyRect>(player);

			updateTexture(dt, playerFaceDirection.direction, gunAttacker.isTryingToAttack);
			updateGunPosition(playerFaceDirection.direction, playerBody.rect);
		}
	}

	void GunPositioningAndTexture::updateTexture(float dt, sf::Vector2f playerFaceDirection, bool wantToAttack) const
	{
		auto gunAttackerView = mRegistry.view<component::Player, component::GunAttacker>();
		auto gunView = mRegistry.view<component::CurrentGun, component::GunProperties, component::TextureRect>();
		for (auto gunAttacker : gunAttackerView)
		{
			auto& gunAttackerDetails = gunAttackerView.get<component::GunAttacker>(gunAttacker);
			for (auto gun : gunView)
			{
				auto& gunTextureBody = gunView.get<component::TextureRect>(gun);
				const auto& gunProperties = gunView.get<component::GunProperties>(gun);

				int offsetX = 50 + gunProperties.gunId * 100;
				if (wantToAttack)
				{
					gunAttackerDetails.timeToHide = gunAttackerDetails.timeBeforeHiding;
					if (gunAttackerDetails.bullets > 0)
						offsetX -= 50;
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
					gunTextureBody.rect = IntRect(offsetX, 0, 50, 50);
				else if (playerFaceDirection == sf::Vector2f(0.f, 1.f) || playerFaceDirection == sf::Vector2f(0.f, -1.f))
					gunTextureBody.rect = IntRect(offsetX, 50, 50, 50);
				else if (playerFaceDirection == sf::Vector2f(-0.7f, -0.7f) || playerFaceDirection == sf::Vector2f(0.7f, -0.7f))
					gunTextureBody.rect = IntRect(offsetX, 100, 50, 50);
				else if (playerFaceDirection == sf::Vector2f(-0.7f, 0.7f) || playerFaceDirection == sf::Vector2f(0.7f, 0.7f))
					gunTextureBody.rect = IntRect(offsetX, 150, 50, 50);
			}
		}
	}

	void GunPositioningAndTexture::updateGunPosition(sf::Vector2f playerFaceDirection, const FloatRect& playerBody) const
	{
		auto gunView = mRegistry.view<component::CurrentGun, component::BodyRect>(entt::exclude<component::HiddenForRenderer>);
		for (auto gun : gunView)
		{
			auto& gunBody = gunView.get<component::BodyRect>(gun);

			sf::Vector2f newGunPosition = getGunNewSpritePosition(playerFaceDirection, playerBody, gunBody.rect);
			sf::Vector2f newGunSize = getGunSpriteFlipping(playerFaceDirection, gunBody.rect.getSize());

			gunBody.rect.setPosition(newGunPosition);
			gunBody.rect.setSize(newGunSize);
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
		const sf::Vector2f gunOffset = getGunOffset(playerFaceDirection, playerBody.getSize());
		const sf::Vector2f gunNewPosition = playerBody.getCenter() + gunOffset;
		return gunNewPosition;
	}

	sf::Vector2f GunPositioningAndTexture::getGunOffset(sf::Vector2f playerFaceDirection, sf::Vector2f playerBodySize) const
	{
		float halfOfPlayerWidth = playerBodySize.x / 2;
		float halfOfPlayerHeight = playerBodySize.y / 2;

		if (playerFaceDirection == PH_EAST || playerFaceDirection == PH_WEST)
			return { 18.f * -playerFaceDirection.x + halfOfPlayerWidth * playerFaceDirection.x, -27.f };
		else if (playerFaceDirection == PH_SOUTH || playerFaceDirection == PH_NORTH)
			return { -26.5f, 32.f * playerFaceDirection.y + halfOfPlayerHeight * playerFaceDirection.y };
		else if (PH_IS_Y_AXIS_NEGATIVE(playerFaceDirection))
			return { 17.f * -playerFaceDirection.x + halfOfPlayerWidth * playerFaceDirection.x, -34.f - halfOfPlayerHeight};
		else if (PH_IS_Y_AXIS_POSITIVE(playerFaceDirection))
			return { 17.f * -playerFaceDirection.x + halfOfPlayerWidth * playerFaceDirection.x, -20.f + halfOfPlayerHeight};
		else
			return { 0.f, 0.f };	
	}


}
