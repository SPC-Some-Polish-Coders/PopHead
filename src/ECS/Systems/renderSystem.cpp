#include "renderSystem.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/camera.hpp"
#include "Utilities/profiling.hpp"
#include <entt/entity/utility.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace {
	ph::Camera defaultCamera;
}

namespace ph::system {

// TODO: For what do we need window???

RenderSystem::RenderSystem(entt::registry& registry, sf::Window& window)
	:System(registry)
	,mWindow(window)
{
}

void RenderSystem::update(float seconds)
{
	PH_PROFILE_FUNCTION();

	Renderer::beginScene(getCameraWithTheBiggestPriority());

	submitLights();
	submitRenderQuads();
	submitRenderQuadsWithTextureRect();
}

Camera& RenderSystem::getCameraWithTheBiggestPriority()
{
	auto view = mRegistry.view<component::Camera>();

	unsigned currentCameraPriority = 0;
	Camera* currentCamera = &defaultCamera;
	view.each([&currentCameraPriority, &currentCamera](component::Camera& camera)
	{
		if(camera.priority > currentCameraPriority) {
			currentCameraPriority = camera.priority;
			currentCamera = &camera.camera;
		}
	});

	// NOTE: Temporary Renderer debug stuff
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
		currentCamera->zoom(1.04f);
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
		currentCamera->zoom(0.96f);

	return *currentCamera;
}

void RenderSystem::submitLights() const
{
	auto view = mRegistry.view<component::PointLight, component::BodyRect>();
	view.each([](const component::PointLight& pointLight, const component::BodyRect& body)
	{
		Renderer::submitLight(pointLight.color, body.rect.getTopLeft() + pointLight.offset, 0.f, 360.f, pointLight.range);
	});
}

void RenderSystem::submitRenderQuads() const
{
	auto view = mRegistry.view<component::RenderQuad, component::BodyRect>
		(entt::exclude<component::HiddenForRenderer, component::TextureRect>);
	view.each([](const component::RenderQuad& quad, const component::BodyRect& body)
	{
		Renderer::submitQuad(
			quad.texture, nullptr, &quad.color, quad.shader,
			body.rect.getTopLeft(), body.rect.getSize(), quad.z, quad.rotation, quad.blocksLight);
	});
}

void RenderSystem::submitRenderQuadsWithTextureRect() const
{
	auto view = mRegistry.view<component::RenderQuad, component::TextureRect, component::BodyRect>
		(entt::exclude<component::HiddenForRenderer>);
	view.each([](const component::RenderQuad& quad, const component::TextureRect& textureRect, const component::BodyRect& body)
	{
		Renderer::submitQuad(
			quad.texture, &textureRect.rect, &quad.color, quad.shader,
			body.rect.getTopLeft(), body.rect.getSize(), quad.z, quad.rotation, quad.blocksLight);
	});
}

}
