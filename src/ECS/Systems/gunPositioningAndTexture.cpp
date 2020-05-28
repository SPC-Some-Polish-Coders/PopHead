#include "pch.hpp"
#include "gunPositioningAndTexture.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "Utilities/direction.hpp"

namespace ph::system {

void GunPositioningAndTexture::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause)
		return;

	auto playerView = mRegistry.view<component::GunAttacker, component::Player, component::BodyRect, component::FaceDirection>();
	for(auto player : playerView)
	{
		const auto& [playerFaceDir, gunAttacker, playerBody] = playerView.get<component::FaceDirection, component::GunAttacker, component::BodyRect>(player);

		updateTexture(dt, playerFaceDir, gunAttacker.isTryingToAttack);
		updateGunPosition(playerFaceDir, playerBody);
	}
}

void GunPositioningAndTexture::updateTexture(float dt, Vec2 playerFaceDir, bool wantToAttack) const
{
	auto gunAttackerView = mRegistry.view<component::Player, component::GunAttacker, component::Bullets>();
	auto gunView = mRegistry.view<component::CurrentGun, component::GunProperties, component::TextureRect>();
	for(auto gunAttacker : gunAttackerView)
	{
		auto& gunAttackerDetails = gunAttackerView.get<component::GunAttacker>(gunAttacker);
		const auto& playerBullets = gunAttackerView.get<component::Bullets>(gunAttacker);
		for(auto gun : gunView)
		{
			auto& gunTextureBody = gunView.get<component::TextureRect>(gun);
			const auto& gunProperties = gunView.get<component::GunProperties>(gun);

			i32 offsetX = 50 + gunProperties.gunId * 100;
			if(wantToAttack)
			{
				gunAttackerDetails.timeToHide = gunAttackerDetails.timeBeforeHiding;
				if((gunProperties.type == component::GunProperties::Type::Pistol && playerBullets.numOfPistolBullets > 0) ||
					(gunProperties.type == component::GunProperties::Type::Shotgun && playerBullets.numOfShotgunBullets > 0)) 
					offsetX -= 50;
			}

			bool shouldHide = true;
			if(gunAttackerDetails.timeToHide > 0.f)
			{
				shouldHide = false;
				gunAttackerDetails.timeToHide -= dt;
			}

			if(shouldHide)
				mRegistry.assign_or_replace<component::HiddenForRenderer>(gun);
			else
				if(mRegistry.has<component::HiddenForRenderer>(gun))
					mRegistry.remove<component::HiddenForRenderer>(gun);

			if(playerFaceDir == Vec2(1.f, 0.f) || playerFaceDir == Vec2(-1.f, 0.f))
				gunTextureBody = IntRect(offsetX, 0, 50, 50);
			else if(playerFaceDir == Vec2(0.f, 1.f) || playerFaceDir == Vec2(0.f, -1.f))
				gunTextureBody = IntRect(offsetX, 50, 50, 50);
			else if(playerFaceDir == Vec2(-0.7f, -0.7f) || playerFaceDir == Vec2(0.7f, -0.7f))
				gunTextureBody = IntRect(offsetX, 100, 50, 50);
			else if(playerFaceDir == Vec2(-0.7f, 0.7f) || playerFaceDir == Vec2(0.7f, 0.7f))
				gunTextureBody = IntRect(offsetX, 150, 50, 50);
		}
	}
}

void GunPositioningAndTexture::updateGunPosition(Vec2 playerFaceDir, const FloatRect& playerBody) const
{
	auto gunView = mRegistry.view<component::CurrentGun, component::BodyRect>(entt::exclude<component::HiddenForRenderer>);
	for(auto gun : gunView)
	{
		auto& gunBody = gunView.get<component::BodyRect>(gun);
		gunBody.pos = getGunNewSpritePosition(playerFaceDir, playerBody, gunBody);
		gunBody.size = getGunSpriteFlipping(playerFaceDir, gunBody.size);
	}
}

Vec2 GunPositioningAndTexture::getGunSpriteFlipping(Vec2 playerFaceDir, Vec2 gunSize) const
{
	auto originalSize = gunSize;
	if(originalSize.x < 0.f)
		originalSize.x = -originalSize.x;
	if(originalSize.y < 0.f)
		originalSize.y = -originalSize.y;

	if(playerFaceDir.x < 0)
	{
		gunSize.x = -originalSize.x;
		gunSize.y = originalSize.y;
	}
	else if(playerFaceDir == Vec2(0, 1))
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

Vec2 GunPositioningAndTexture::getGunNewSpritePosition(Vec2 playerFaceDir, const FloatRect& playerBody,
													   const FloatRect& gunBody) const
{
	Vec2 gunOffset = getGunOffset(playerFaceDir, playerBody.size);
	Vec2 gunNewPosition = playerBody.center() + gunOffset;
	return gunNewPosition;
}

Vec2 GunPositioningAndTexture::getGunOffset(Vec2 playerFaceDir, Vec2 playerBodySize) const
{
	float halfOfPlayerWidth = playerBodySize.x / 2;
	float halfOfPlayerHeight = playerBodySize.y / 2;

	if(playerFaceDir == PH_EAST || playerFaceDir == PH_WEST)
		return { 18.f * -playerFaceDir.x + halfOfPlayerWidth * playerFaceDir.x, -27.f };
	else if(playerFaceDir == PH_SOUTH || playerFaceDir == PH_NORTH)
		return { -26.5f, 32.f * playerFaceDir.y + halfOfPlayerHeight * playerFaceDir.y };
	else if(PH_IS_Y_AXIS_NEGATIVE(playerFaceDir))
		return { 17.f * -playerFaceDir.x + halfOfPlayerWidth * playerFaceDir.x, -34.f - halfOfPlayerHeight};
	else if(PH_IS_Y_AXIS_POSITIVE(playerFaceDir))
		return { 17.f * -playerFaceDir.x + halfOfPlayerWidth * playerFaceDir.x, -20.f + halfOfPlayerHeight};
	else
		return { 0.f, 0.f };	
}

}
