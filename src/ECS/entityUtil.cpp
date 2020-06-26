#include "pch.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph {

static entt::registry* registry;

void initEntityUtil(entt::registry* reg)
{
	registry = reg;
}

using namespace component;

bool isPlayerOnScene()
{
	for(auto entity : registry->view<Player>())
		return true;
	return false;
}

bool isPlayerAlive()
{
	for(auto entity : registry->view<Player>())
		return !registry->has<DeadCharacter>(entity);
	return false;
}

bool isPlayerFallingIntoPit()
{
	for(auto entity : registry->view<Player>())
		return registry->has<FallingIntoPit>(entity);
	return false;
}

entt::entity getPlayerEntity()
{
	for(auto entity : registry->view<Player>())
		return entity;
	return entt::null;
}

BodyRect getPlayerBody()
{
	auto entity = getPlayerEntity();
	auto body = registry->get<BodyRect>(entity);
	return body;
}

BodyRect& getPlayerBodyRef()
{
	auto entity = getPlayerEntity();
	auto& body = registry->get<BodyRect>(entity);
	return body;
}

Vec2 getPlayerCenterPos()
{
	auto body = getPlayerBody();
	return body.center();
}

Vec2 getPlayerPos()
{
	auto body = getPlayerBody();
	return body.pos;
}

Vec2 getPlayerFaceDirection()
{
	auto entity = getPlayerEntity();
	Vec2 dir = registry->get<FaceDirection>(entity);
	return dir;
}

FloatRect getCurrentCameraBounds()
{
	auto cameras = registry->view<Camera>();
	for(auto entity : cameras)
	{
		const auto& camera = cameras.get<Camera>(entity);
		if(camera.name == Camera::currentCameraName)
			return camera.bounds;
	}
	return {};
}

FloatRect getPlayerCameraBounds()
{
	auto players = registry->view<Player, Camera>();
	for(auto entity : players)
	{
		const auto& camera = players.get<Camera>(entity);
		return camera.bounds;
	}
	PH_BREAKPOINT();
	return {};
}

}
