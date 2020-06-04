#include "pch.hpp"
#include "renderSystem.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/API/camera.hpp"
#include "Utilities/random.hpp"

namespace {
	ph::Camera defaultCamera = ph::Camera({320, 180}, {640, 360});
}

namespace ph::system {

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
	mRegistry.view<CameraShake, component::Camera>().each([&]
	(auto cameraEntity, auto& shake, auto& camera)
	{
		if(shake.elapsedTime < shake.duration) 
		{
			auto cameraOffset = Random::generateVector({-1.f, -1.f}, {1.f, 1.f});
			cameraOffset *= shake.magnitude;
			cameraOffset *= (shake.duration - shake.elapsedTime) / shake.duration;
			auto newCameraPos = camera.center() + cameraOffset;
			if(shake.smooth)
				camera.setCenter(Math::lerp(camera.center(), newCameraPos, 2.f));
			else
				camera.setCenter(newCameraPos);
			shake.elapsedTime += dt;
		}
		else 
		{
			mRegistry.remove<CameraShake>(cameraEntity);
		}
	});

	// get current camera and update shake
	ph::Camera* currentCamera = &defaultCamera;
	mRegistry.view<component::Camera>().each([&currentCamera]
	(auto& camera) 
	{
		if(camera.name == component::Camera::currentCameraName)
			currentCamera = &camera;
	});

	// set camera
	Renderer::setGameWorldCamera(*currentCamera);

	// submit light sources
	mRegistry.view<LightSource, BodyRect>().each([]
	(const auto& light, auto body)
	{
		PH_ASSERT_UNEXPECTED_SITUATION(light.startAngle <= light.endAngle, "start angle must be lesser or equal to end angle");
		Renderer::submitLight(light.color, body.pos + light.offset, light.startAngle, light.endAngle,
			light.attenuationAddition, light.attenuationFactor, light.attenuationSquareFactor, light.rayCollisionDetection);
	});

	//submit single light walls
	if(Renderer::getNrOfLights() > 0)
	{
		mRegistry.view<LightWall, BodyRect>().each([]
		(const auto& lightWall, auto body) 
		{
			if(lightWall.y == -1.f)
				Renderer::submitLightWall(body);
			else
				Renderer::submitLightWall(FloatRect(body.pos + lightWall.pos, lightWall.size));
		});
	}

	auto getIndoorBlendColor = [](IndoorBlend ib)
	{
		u8 alpha = Cast<u8>(ib.alpha * 255.f);
		return sf::Color(255, 255, 255, alpha);
	};

	// submit ground chunks with indoor blend
	mRegistry.view<BodyRect, GroundRenderChunk, IndoorBlend>().each([&]
	(auto bounds, auto& chunk, auto indoorBlend)
	{
		if(intersect(currentCamera->getBounds(), bounds))
			Renderer::submitGroundChunk(bounds.pos, chunk.textureRect, chunk.z, getIndoorBlendColor(indoorBlend));
	});

	// submit chunks with indoor blend
	mRegistry.view<BodyRect, RenderChunk, IndoorBlend>().each([&]
	(auto bounds, auto& chunk, auto indoorBlend)
	{
		if(intersect(currentCamera->getBounds(), bounds) && !chunk.quads.empty())
			Renderer::submitChunk(chunk.quads, bounds, chunk.z, &chunk.rendererID, getIndoorBlendColor(indoorBlend));

		FloatRect lightWallsBounds = {bounds.x - 400.f, bounds.y - 400.f, bounds.w + 800.f, bounds.h + 800.f};
		if(!chunk.lightWalls.empty() && intersect(currentCamera->getBounds(), lightWallsBounds)) 
			Renderer::submitBunchOfLightWalls(chunk.lightWalls);
	});

	auto getOutdoorBlendColor = [](OutdoorBlend ob)
	{
		u8 d = Cast<u8>(ob.brightness * 255.f);
		return sf::Color(d, d, d, 255);
	};

	// submit ground chunks with outdoor blend
	mRegistry.view<BodyRect, GroundRenderChunk, OutdoorBlend>().each([&]
	(auto bounds, auto& chunk, auto outdoorBlend)
	{
		if(intersect(currentCamera->getBounds(), bounds))
			Renderer::submitGroundChunk(bounds.pos, chunk.textureRect, chunk.z, getOutdoorBlendColor(outdoorBlend));
	});

	// submit chunks with outdoor blend
	mRegistry.view<BodyRect, RenderChunk, OutdoorBlend>().each([&]
	(auto bounds, auto& chunk, auto outdoorBlend)
	{
		if(intersect(currentCamera->getBounds(), bounds) && !chunk.quads.empty())
			Renderer::submitChunk(chunk.quads, bounds, chunk.z, &chunk.rendererID, getOutdoorBlendColor(outdoorBlend));

		FloatRect lightWallsBounds = {bounds.x - 400.f, bounds.y - 400.f, bounds.w + 800.f, bounds.h + 800.f};
		if(!chunk.lightWalls.empty() && intersect(currentCamera->getBounds(), lightWallsBounds)) 
			Renderer::submitBunchOfLightWalls(chunk.lightWalls);
	});

	auto getIndoorOutdoorColor = [](IndoorOutdoorBlend io)
	{
		u8 bright = Cast<u8>(io.brightness * 255.f);
		u8 alpha = Cast<u8>(io.alpha * 255.f);
		return sf::Color(bright, bright, bright, alpha);
	};

	// submit render quads
	mRegistry.view<RenderQuad, IndoorOutdoorBlend, BodyRect>
	(entt::exclude<HiddenForRenderer, TextureRect>).each([&]
	(const auto& quad, auto indoorOutdoorBlend, auto body)
	{
		sf::Color color = quad.color * getIndoorOutdoorColor(indoorOutdoorBlend);
		Renderer::submitQuad(
			quad.texture, Null, &color, quad.shader,
			body.pos, body.size, quad.z, quad.rotation, quad.rotationOrigin);
	});

	// submit render quads with texture rect
	mRegistry.view<RenderQuad, TextureRect, BodyRect, IndoorOutdoorBlend>
	(entt::exclude<HiddenForRenderer>).each([&]
	(const auto& quad, auto textureRect, auto body, auto indoorOutdoorBlend)
	{
		Renderer::submitQuad(
			quad.texture, &textureRect, &getIndoorOutdoorColor(indoorOutdoorBlend), quad.shader,
			body.pos, body.size, quad.z, quad.rotation, quad.rotationOrigin);
	});

	// submit render quads with no indoor outdoor component
	mRegistry.view<RenderQuad, BodyRect>
	(entt::exclude<HiddenForRenderer, TextureRect, IndoorOutdoorBlend>).each([&]
	(const auto& quad, auto body)
	{
		Renderer::submitQuad(
			quad.texture, Null, &quad.color, quad.shader,
			body.pos, body.size, quad.z, quad.rotation, quad.rotationOrigin);
	});

	// submit render quads with texture rect and no indoor outdoor component
	mRegistry.view<RenderQuad, TextureRect, BodyRect>
	(entt::exclude<HiddenForRenderer, IndoorOutdoorBlend>).each([&]
	(const auto& quad, auto textureRect, auto body)
	{
		Renderer::submitQuad(
			quad.texture, &textureRect, &quad.color, quad.shader,
			body.pos, body.size, quad.z, quad.rotation, quad.rotationOrigin);
	});
}

}
