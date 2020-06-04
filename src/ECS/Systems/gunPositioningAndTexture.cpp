#include "pch.hpp"
#include "gunPositioningAndTexture.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "Utilities/direction.hpp"

namespace ph::system {

using namespace component;

void GunPositioningAndTexture::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<Player, GunAttacker, Bullets, BodyRect, FaceDirection>().each([&]
	(auto, auto& gunAttacker, auto playerBullets, auto playerBody, auto playerFaceDir)
	{
		mRegistry.view<CurrentGun, GunProperties, TextureRect, BodyRect>().each([&]
		(auto gunEntity, auto, const auto& gunProperties, auto& gunTextureRect, auto& gunBody)
		{
			// update gun texture
			i32 offsetX = 50 + gunProperties.gunId * 100;
			if(gunAttacker.isTryingToAttack)
			{
				gunAttacker.timeToHide = gunAttacker.timeBeforeHiding;
				if((gunProperties.type == GunProperties::Type::Pistol && playerBullets.numOfPistolBullets > 0) ||
					(gunProperties.type == GunProperties::Type::Shotgun && playerBullets.numOfShotgunBullets > 0)) 
				{
					offsetX -= 50;
				}
			}

			bool shouldHide = true;
			if(gunAttacker.timeToHide > 0.f)
			{
				shouldHide = false;
				gunAttacker.timeToHide -= dt;
			}

			if(shouldHide)
			{
				mRegistry.assign_or_replace<HiddenForRenderer>(gunEntity);
			}
			else
			{
				if(mRegistry.has<HiddenForRenderer>(gunEntity))
					mRegistry.remove<HiddenForRenderer>(gunEntity);
			}

			if(playerFaceDir == Vec2(1.f, 0.f) || playerFaceDir == Vec2(-1.f, 0.f))
				gunTextureRect = IntRect(offsetX, 0, 50, 50);
			else if(playerFaceDir == Vec2(0.f, 1.f) || playerFaceDir == Vec2(0.f, -1.f))
				gunTextureRect = IntRect(offsetX, 50, 50, 50);
			else if(playerFaceDir == Vec2(-0.7f, -0.7f) || playerFaceDir == Vec2(0.7f, -0.7f))
				gunTextureRect = IntRect(offsetX, 100, 50, 50);
			else if(playerFaceDir == Vec2(-0.7f, 0.7f) || playerFaceDir == Vec2(0.7f, 0.7f))
				gunTextureRect = IntRect(offsetX, 150, 50, 50);

			// update sprite position
			float halfOfPlayerWidth = playerBody.w / 2.f;
			float halfOfPlayerHeight = playerBody.h / 2.f;

			Vec2 gunOffset;
			if(playerFaceDir == PH_EAST || playerFaceDir == PH_WEST)
				gunOffset = Vec2(18.f * -playerFaceDir.x + halfOfPlayerWidth * playerFaceDir.x, -27.f);
			else if(playerFaceDir == PH_SOUTH || playerFaceDir == PH_NORTH)
				gunOffset = Vec2(-26.5f, 32.f * playerFaceDir.y + halfOfPlayerHeight * playerFaceDir.y);
			else if(PH_IS_Y_AXIS_NEGATIVE(playerFaceDir))
				gunOffset = Vec2(17.f * -playerFaceDir.x + halfOfPlayerWidth * playerFaceDir.x, -34.f - halfOfPlayerHeight);
			else if(PH_IS_Y_AXIS_POSITIVE(playerFaceDir))
				gunOffset = Vec2(17.f * -playerFaceDir.x + halfOfPlayerWidth * playerFaceDir.x, -20.f + halfOfPlayerHeight);
			else
				gunOffset = Vec2();

			gunBody.pos = playerBody.center() + gunOffset;

			// gun sprite flipping
			auto originalSize = gunBody.size;
			if(originalSize.x < 0.f)
				originalSize.x = -originalSize.x;
			if(originalSize.y < 0.f)
				originalSize.y = -originalSize.y;

			if(playerFaceDir.x < 0)
				gunBody.size = Vec2(-originalSize.x, originalSize.y);
			else if(playerFaceDir == Vec2(0, 1))
				gunBody.size = Vec2(originalSize.x, -originalSize.y);
			else
				gunBody.size = originalSize;
		});
	});
}

}
