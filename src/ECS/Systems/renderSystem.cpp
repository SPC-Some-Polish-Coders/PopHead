#include "pch.hpp"
#include "renderSystem.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Utilities/random.hpp"

namespace ph::system {

static FloatRect defaultCameraBounds = {0.f, 0.f, 640.f, 360.f};

RenderSystem::RenderSystem(entt::registry& registry, Texture& tileset)
	:System(registry)
{
	Renderer::setChunksTexture(tileset);
}

using namespace component;

void RenderSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	// shake camera
	mRegistry.view<CameraShake, Camera>().each([&]
	(auto cameraEntity, auto& shake, auto& camera)
	{
		if(shake.elapsedTime < shake.duration) 
		{
			auto cameraOffset = Random::generateVector({-1.f, -1.f}, {1.f, 1.f});
			cameraOffset *= shake.magnitude;
			cameraOffset *= (shake.duration - shake.elapsedTime) / shake.duration;
			auto newCameraPos = camera.bounds.pos + cameraOffset;
			if(shake.smooth)
				camera.bounds.pos = lerp(camera.bounds.pos, newCameraPos, 2.f);
			else
				camera.bounds.pos = newCameraPos;
			shake.elapsedTime += dt;
		}
		else 
		{
			mRegistry.remove<CameraShake>(cameraEntity);
		}
	});

	// get current camera
	FloatRect currentCameraBounds = defaultCameraBounds;
	mRegistry.view<Camera>().each([&]
	(auto& camera)
	{
		if(camera.name == Camera::currentCameraName)
			currentCameraBounds = camera.bounds;
	});

	// set camera
	Renderer::setGameWorldCamera(currentCameraBounds);

	auto getIndoorBlendColor = [](IndoorBlend ib)
	{
		u8 alpha = Cast<u8>(ib.alpha * 255.f);
		return sf::Color(255, 255, 255, alpha);
	};

	// submit ground chunks with indoor blend
	mRegistry.view<BodyRect, GroundRenderChunk, IndoorBlend, InsideSimRegion>().each([&]
	(auto bounds, auto& chunk, auto indoorBlend, auto)
	{
		if(intersect(currentCameraBounds, bounds))
			Renderer::submitGroundChunk(bounds.pos, chunk.textureRect, chunk.z, getIndoorBlendColor(indoorBlend));
	});

	// submit chunks with indoor blend
	mRegistry.view<BodyRect, RenderChunk, IndoorBlend, InsideSimRegion>().each([&]
	(auto bounds, auto& chunk, auto indoorBlend, auto)
	{
		if(intersect(currentCameraBounds, bounds) && !chunk.quads.empty())
			Renderer::submitChunk(chunk.quads, bounds, chunk.z, &chunk.rendererID, getIndoorBlendColor(indoorBlend));

		FloatRect lightWallsBounds = {bounds.x - 400.f, bounds.y - 400.f, bounds.w + 800.f, bounds.h + 800.f};
		if(!chunk.lightWalls.empty() && intersect(currentCameraBounds, lightWallsBounds)) 
			Renderer::submitBunchOfLightWalls(chunk.lightWalls);
	});

	auto getOutdoorBlendColor = [](OutdoorBlend ob)
	{
		u8 d = Cast<u8>(ob.brightness * 255.f);
		return sf::Color(d, d, d, 255);
	};

	// submit ground chunks with outdoor blend
	mRegistry.view<BodyRect, GroundRenderChunk, OutdoorBlend, InsideSimRegion>().each([&]
	(auto bounds, auto& chunk, auto outdoorBlend, auto)
	{
		if(intersect(currentCameraBounds, bounds))
			Renderer::submitGroundChunk(bounds.pos, chunk.textureRect, chunk.z, getOutdoorBlendColor(outdoorBlend));
	});

	// submit chunks with outdoor blend
	mRegistry.view<BodyRect, RenderChunk, OutdoorBlend, InsideSimRegion>().each([&]
	(auto bounds, auto& chunk, auto outdoorBlend, auto)
	{
		if(intersect(currentCameraBounds, bounds) && !chunk.quads.empty())
			Renderer::submitChunk(chunk.quads, bounds, chunk.z, &chunk.rendererID, getOutdoorBlendColor(outdoorBlend));

		FloatRect lightWallsBounds = {bounds.x - 400.f, bounds.y - 400.f, bounds.w + 800.f, bounds.h + 800.f};
		if(!chunk.lightWalls.empty() && intersect(currentCameraBounds, lightWallsBounds)) 
			Renderer::submitBunchOfLightWalls(chunk.lightWalls);
	});

	auto getIndoorOutdoorColor = [](IndoorOutdoorBlend io)
	{
		u8 bright = Cast<u8>(io.brightness * 255.f);
		u8 alpha = Cast<u8>(io.alpha * 255.f);
		return sf::Color(bright, bright, bright, alpha);
	};

	// submit render quads
	mRegistry.view<RenderQuad, IndoorOutdoorBlend, BodyRect, InsideSimRegion>
	(entt::exclude<HiddenForRenderer, TextureRect>).each([&]
	(const auto& quad, auto indoorOutdoorBlend, auto body, auto)
	{
		sf::Color color = quad.color * getIndoorOutdoorColor(indoorOutdoorBlend);
		Renderer::submitQuad(
			quad.texture, Null, &color, quad.shader,
			body, quad.z, quad.rotation, quad.rotationOrigin);
	});

	// submit render quads with texture rect
	mRegistry.view<RenderQuad, TextureRect, BodyRect, IndoorOutdoorBlend, InsideSimRegion>
	(entt::exclude<HiddenForRenderer>).each([&]
	(const auto& quad, auto textureRect, auto body, auto indoorOutdoorBlend, auto)
	{
		Renderer::submitQuad(
			quad.texture, &textureRect, &getIndoorOutdoorColor(indoorOutdoorBlend), quad.shader,
			body, quad.z, quad.rotation, quad.rotationOrigin);
	});

	// submit render quads with no indoor outdoor component
	mRegistry.view<RenderQuad, BodyRect, InsideSimRegion>
	(entt::exclude<HiddenForRenderer, TextureRect, IndoorOutdoorBlend>).each([&]
	(const auto& quad, auto body, auto)
	{
		Renderer::submitQuad(
			quad.texture, Null, &quad.color, quad.shader,
			body, quad.z, quad.rotation, quad.rotationOrigin);
	});

	// submit render quads with texture rect and no indoor outdoor component
	mRegistry.view<RenderQuad, TextureRect, BodyRect, InsideSimRegion>
	(entt::exclude<HiddenForRenderer, IndoorOutdoorBlend>).each([&]
	(const auto& quad, auto textureRect, auto body, auto)
	{
		Renderer::submitQuad(
			quad.texture, &textureRect, &quad.color, quad.shader,
			body, quad.z, quad.rotation, quad.rotationOrigin);
	});

	// submit light sources with indoor outdoor blend
	mRegistry.view<LightSource, IndoorOutdoorBlend, BodyRect, InsideSimRegion>().each([&]
	(const auto& light, auto io, auto body, auto)
	{
		sf::Color color = light.color * getIndoorOutdoorColor(io);
		PH_ASSERT_UNEXPECTED_SITUATION(light.startAngle <= light.endAngle, "start angle must be lesser or equal to end angle");
		Renderer::submitLight(color, body.pos + light.offset, light.startAngle, light.endAngle,
			light.attenuationAddition, light.attenuationFactor, light.attenuationSquareFactor, light.rayCollisionDetection);
	});

	// submit light sources with no indoor outdoor blend
	mRegistry.view<LightSource, BodyRect, InsideSimRegion>(entt::exclude<IndoorOutdoorBlend>).each([]
	(const auto& light, auto body, auto)
	{
		PH_ASSERT_UNEXPECTED_SITUATION(light.startAngle <= light.endAngle, "start angle must be lesser or equal to end angle");
		Renderer::submitLight(light.color, body.pos + light.offset, light.startAngle, light.endAngle,
			light.attenuationAddition, light.attenuationFactor, light.attenuationSquareFactor, light.rayCollisionDetection);
	});

	//submit single light walls
	if(Renderer::getNrOfCollisionLights() > 0)
	{
		mRegistry.view<LightWall, BodyRect, InsideSimRegion>().each([]
		(const auto& lightWall, auto body, auto) 
		{
			if(lightWall.y == -1.f)
				Renderer::submitLightWall(body);
			else
				Renderer::submitLightWall(FloatRect(body.pos + lightWall.pos, lightWall.size));
		});
	}
}

}
