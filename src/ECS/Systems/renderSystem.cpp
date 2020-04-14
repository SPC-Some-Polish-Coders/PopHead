#include "renderSystem.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/API/camera.hpp"
#include "Logs/logs.hpp"
#include "Utilities/profiling.hpp"
#include "Utilities/random.hpp"
#include "Utilities/math.hpp"
#include <entt/entity/utility.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace {
	ph::Camera defaultCamera = ph::Camera({320, 180}, {640, 360});
}

namespace ph::system {

RenderSystem::RenderSystem(entt::registry& registry, Texture& tileset)
	:System(registry)
	,mTilesetTexture(tileset)
{
}

void RenderSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	// shake camera
	auto shakingCameras = mRegistry.view<component::CameraShake, component::Camera>();
	for(auto cameraEntity : shakingCameras)
	{
		auto& [shake, camera] = shakingCameras.get<component::CameraShake, component::Camera>(cameraEntity);
		if(shake.elapsedTime < shake.duration) {
			sf::Vector2f cameraOffset = Random::generateVector({-1.f, -1.f}, {1.f, 1.f});
			cameraOffset *= shake.magnitude;
			cameraOffset *= (shake.duration - shake.elapsedTime) / shake.duration;
			sf::Vector2f cameraPos = camera.camera.getCenter();
			sf::Vector2f newCameraPos = cameraPos + cameraOffset;
			if(shake.smooth)
				camera.camera.setCenter(Math::lerp(cameraPos, newCameraPos, 2.f));
			else
				camera.camera.setCenter(newCameraPos);
			shake.elapsedTime += dt;
		}
		else {
			mRegistry.remove<component::CameraShake>(cameraEntity);
		}
	}

	// get current camera and update shake
	auto cameras = mRegistry.view<component::Camera>();
	Camera* currentCamera = &defaultCamera;
	cameras.each([&currentCamera](component::Camera& camera) {
		if(camera.name == component::Camera::currentCameraName) {
			currentCamera = &camera.camera;
		}
	});

	// set camera
	Renderer::setGameWorldCamera(*currentCamera);

	// submit light sources
	auto lightSources = mRegistry.view<component::LightSource, component::BodyRect>();
	lightSources.each([](const component::LightSource& pointLight, const component::BodyRect& body)
	{
		PH_ASSERT_UNEXPECTED_SITUATION(pointLight.startAngle <= pointLight.endAngle, "start angle must be lesser or equal to end angle");
		Renderer::submitLight(pointLight.color, body.rect.getTopLeft() + pointLight.offset, pointLight.startAngle, pointLight.endAngle,
			pointLight.attenuationAddition, pointLight.attenuationFactor, pointLight.attenuationSquareFactor);
	});

	//submit single light walls
	if(Renderer::getNrOfLights() > 0)
	{
		auto lightWalls = mRegistry.view<component::LightWall, component::BodyRect>();
		lightWalls.each([](const component::LightWall& lw, const component::BodyRect& body) 
		{
			if(lw.rect.top == -1.f)
				Renderer::submitLightWall(body.rect);
			else
				Renderer::submitLightWall(FloatRect(body.rect.getTopLeft() + lw.rect.getTopLeft(), lw.rect.getSize()));
		});
	}

	// submit map ground chunks
	auto groundChunks = mRegistry.view<component::GroundRenderChunk>();
	groundChunks.each([this, currentCamera](component::GroundRenderChunk& chunk)
	{
		if(currentCamera->getBounds().doPositiveRectsIntersect(chunk.bounds))
			Renderer::submitGroundChunk(chunk.bounds.getTopLeft(), mTilesetTexture, chunk.textureRect, chunk.z);
	});

	// submit map chunks
	auto renderChunks = mRegistry.view<component::RenderChunk>();
	renderChunks.each([this, currentCamera](component::RenderChunk& chunk)
	{
		if(currentCamera->getBounds().doPositiveRectsIntersect(chunk.quadsBounds) && !chunk.quads.empty())
			Renderer::submitChunk(chunk.quads, mTilesetTexture, chunk.quadsBounds, chunk.z, &chunk.rendererID);
		if(!chunk.lightWalls.empty() && currentCamera->getBounds().doPositiveRectsIntersect(chunk.lightWallsBounds))
			Renderer::submitBunchOfLightWalls(chunk.lightWalls);
	});

	// submit render quads
	auto renderQuads = mRegistry.view<component::RenderQuad, component::BodyRect>(entt::exclude<component::HiddenForRenderer, component::TextureRect>);
	renderQuads.each([](const component::RenderQuad& quad, const component::BodyRect& body)
	{
		Renderer::submitQuad(
			quad.texture, nullptr, &quad.color, quad.shader,
			body.rect.getTopLeft(), body.rect.getSize(), quad.z, quad.rotation, quad.rotationOrigin);
	});

	// submit render quads with texture rect
	auto renderQuadsWithTextureRect = 
		mRegistry.view<component::RenderQuad, component::TextureRect, component::BodyRect>(entt::exclude<component::HiddenForRenderer>);
	renderQuadsWithTextureRect.each([](const component::RenderQuad& quad, const component::TextureRect& textureRect, const component::BodyRect& body)
	{
		Renderer::submitQuad(
			quad.texture, &textureRect.rect, &quad.color, quad.shader,
			body.rect.getTopLeft(), body.rect.getSize(), quad.z, quad.rotation, quad.rotationOrigin);
	});
}

}
