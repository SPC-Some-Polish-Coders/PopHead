#include "gunTexture.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph::system {

	void GunTexture::update(float seconds)
	{
		auto playerView = mRegistry.view<component::GunAttacker, component::Player, component::FaceDirection>();
		for (auto player : playerView)
		{
			const auto& playerFaceDirection = playerView.get<component::FaceDirection>(player).direction;
			const auto& gunAttacker = playerView.get<component::GunAttacker>(player);

			auto gunView = mRegistry.view<component::PlayerGun, component::TextureRect>();
			for (auto gun : gunView)
			{
				auto& gunTextureBody = gunView.get<component::TextureRect>(gun);
				auto& playerGun = gunView.get<component::PlayerGun>(gun);

				if (playerGun.cooldownSinceLastShot <= 0.f)
					mRegistry.assign_or_replace<component::HiddenForRenderer>(gun);
				else
					playerGun.cooldownSinceLastShot -= seconds;

				int offsetX = 0;
				if (gunAttacker.isTryingToAttack)
				{
					playerGun.cooldownSinceLastShot = playerGun.timeBeforeHiding;
					if (mRegistry.has<component::HiddenForRenderer>(gun))
						mRegistry.remove<component::HiddenForRenderer>(gun);

					if (!gunAttacker.canAttack)
						offsetX = 16;
				}
				else
					offsetX = 16;

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

}
