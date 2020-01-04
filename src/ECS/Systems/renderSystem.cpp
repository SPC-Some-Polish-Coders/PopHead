#include "renderSystem.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/API/camera.hpp"
#include "Logs/logs.hpp"
#include "Utilities/profiling.hpp"
#include <entt/entity/utility.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace {
	ph::Camera defaultCamera;
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

	// get current camera
	auto cameras = mRegistry.view<component::Camera>();
	Camera* currentCamera = &defaultCamera;
	cameras.each([&currentCamera](component::Camera& camera) {
		if(camera.name == component::Camera::currentCameraName)
			currentCamera = &camera.camera;
	});

	// begin scene
	Renderer::beginScene(*currentCamera);

	// submit light sources
	auto lightSources = mRegistry.view<component::LightSource, component::BodyRect>();
	lightSources.each([](const component::LightSource& pointLight, const component::BodyRect& body)
	{
		PH_ASSERT_UNEXPECTED_SITUATION(pointLight.startAngle <= pointLight.endAngle, "start angle must be lesser or equal to end angle");
		Renderer::submitLight(pointLight.color, body.rect.getTopLeft() + pointLight.offset, pointLight.startAngle, pointLight.endAngle,
			pointLight.attenuationAddition, pointLight.attenuationFactor, pointLight.attenuationSquareFactor);
	});

	//submit light walls
	auto lightWalls = mRegistry.view<component::LightWall, component::BodyRect>();
	lightWalls.each([](const component::LightWall& bl, const component::BodyRect& body) 
	{
		if(bl.rect.top == -1.f)
			Renderer::submitLightBlockingQuad(body.rect.getTopLeft(), body.rect.getSize());
		else
			Renderer::submitLightBlockingQuad(body.rect.getTopLeft() + bl.rect.getTopLeft(), bl.rect.getSize());
	});

	// submit map chunks
	auto renderChunks = mRegistry.view<component::RenderChunk>();
	renderChunks.each([this, currentCamera](component::RenderChunk& chunk)
	{
		if(currentCamera->getBounds().doPositiveRectsIntersect(chunk.bounds))
			Renderer::submitBunchOfQuadsWithTheSameTexture(chunk.quads, &mTilesetTexture, nullptr, chunk.z);
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
	auto renderQuadsWithTextureRect = mRegistry.view<component::RenderQuad, component::TextureRect, component::BodyRect>(entt::exclude<component::HiddenForRenderer>);
	renderQuadsWithTextureRect.each([](const component::RenderQuad& quad, const component::TextureRect& textureRect, const component::BodyRect& body)
	{
		Renderer::submitQuad(
			quad.texture, &textureRect.rect, &quad.color, quad.shader,
			body.rect.getTopLeft(), body.rect.getSize(), quad.z, quad.rotation, quad.rotationOrigin);
	});
}

}
