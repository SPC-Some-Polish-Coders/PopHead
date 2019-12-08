#include "renderSystem.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/camera.hpp"
#include "entt/entity/utility.hpp"
#include <SFML/Window/Keyboard.hpp>

namespace {
	ph::Camera defaultCamera;
}

namespace ph::system {

RenderSystem::RenderSystem(entt::registry& registry, sf::Window& window)
	:System(registry)
	,mWindow(window)
{
}

void RenderSystem::update(float seconds)
{
	Renderer::beginScene(getCameraWithTheBiggestPriority());

	submitSingleColorSprites();
	submitLights();
	submitTextureSprites();
	submitTextureSpritesWithCustomShader();
	submitTextureSpritesWithTextureRect();
	submitTextureSpritesWithTextureRectAndCustomShader();
	submitTextureSpritesWithSingleColorMultiplication();
	submitTextureSpritesWithSingleColorAndCustomShader();
	submitTextureSpritesWithSingleColorAndTextureRect();
	submitTextureSpritesWithSingleColorTextureRectAndCustomShader();
	submitSingleColorSpritesWithRotation();
	submitTextureSpritesWithRotation();
	submitTextureSpritesWithCustomShaderAndRotation();
	submitTextureSpritesWithTextureRectAndRotation();
	submitTextureSpritesWithTextureRectCustomShaderAndRotation();
	submitTextureSpritesWithSingleColorMultiplicationAndRotation();
	submitTextureSpritesWithSingleColorCustomShaderAndRotation();
	submitTextureSpritesWithSingleColorTextureRectAndRotation();
	submitTextureSpritesWithSingleColorTextureRectCustomShaderAndRotation();
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

void RenderSystem::submitSingleColorSprites() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::Color>
		(entt::exclude<component::TexturePtr, component::Rotation, component::HiddenForRenderer>);

	view.each([this](const component::BodyRect& body, const component::Z z, const component::Color& color) 
	{
		Renderer::submitQuad(nullptr, nullptr, &color.color, nullptr, body.rect.getTopLeft(), body.rect.getSize(), z.z);
	});
}

void RenderSystem::submitTextureSprites() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::TexturePtr>
		(entt::exclude<component::TextureRect, component::Rotation, component::HiddenForRenderer>);

	view.each([this](const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr)
	{
		Renderer::submitQuad(texPtr.texture, nullptr, nullptr, nullptr, body.rect.getTopLeft(), body.rect.getSize(), z.z);
	});
}

void RenderSystem::submitTextureSpritesWithCustomShader() const
{
	auto view = mRegistry.view
		<const component::BodyRect, component::Z, const component::TexturePtr, const component::ShaderPtr>
		(entt::exclude<component::TextureRect, component::Rotation, component::HiddenForRenderer>);
	
	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, const component::ShaderPtr shaderPtr)
	{
		Renderer::submitQuad(texPtr.texture, nullptr, nullptr, shaderPtr.shader, body.rect.getTopLeft(), body.rect.getSize(), z.z);
	});
}

void RenderSystem::submitTextureSpritesWithTextureRect() const
{
	auto view = mRegistry.view
		<const component::BodyRect, component::Z, const component::TexturePtr, const component::TextureRect>
		(entt::exclude<component::ShaderPtr, component::Rotation, component::Color, component::HiddenForRenderer>);

	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr,const component::TextureRect& texRect)
	{
		Renderer::submitQuad(texPtr.texture, &texRect.rect, nullptr, nullptr, body.rect.getTopLeft(), body.rect.getSize(), z.z);
	});
}

void RenderSystem::submitTextureSpritesWithTextureRectAndCustomShader() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::TexturePtr, component::TextureRect, component::ShaderPtr>
		(entt::exclude<component::Color, component::Rotation, component::HiddenForRenderer>);

	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, const component::TextureRect& texRect,
	 const component::ShaderPtr shaderPtr)
	{
		Renderer::submitQuad(texPtr.texture, &texRect.rect, nullptr, shaderPtr.shader, body.rect.getTopLeft(), body.rect.getSize(), z.z);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorMultiplication() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::TexturePtr, component::Color>
		(entt::exclude<component::TextureRect, component::ShaderPtr, component::Rotation, component::HiddenForRenderer>);

	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, const component::Color& color)
	{
		Renderer::submitQuad(texPtr.texture, nullptr, &color.color, nullptr, body.rect.getTopLeft(), body.rect.getSize(), z.z);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorAndCustomShader() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::TexturePtr, component::Color, component::ShaderPtr>
		(entt::exclude<component::TextureRect, component::Rotation, component::HiddenForRenderer>);

	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, const component::Color& color,
	 const component::ShaderPtr shaderPtr)
	{
		Renderer::submitQuad(texPtr.texture, nullptr, &color.color, shaderPtr.shader, body.rect.getTopLeft(), body.rect.getSize(), z.z);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorAndTextureRect() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::TexturePtr, component::TextureRect, component::Color>
		(entt::exclude<component::ShaderPtr, component::Rotation, component::HiddenForRenderer>);

	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, const component::TextureRect& texRect,
	 const component::Color& color)
	{
		Renderer::submitQuad(texPtr.texture, &texRect.rect, &color.color, nullptr, body.rect.getTopLeft(), body.rect.getSize(), z.z);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorTextureRectAndCustomShader() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::TexturePtr, component::TextureRect, component::Color, component::ShaderPtr>
		(entt::exclude<component::Rotation, component::HiddenForRenderer>);

	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, const component::TextureRect& texRect,
	 const component::Color& color, const component::ShaderPtr shaderPtr)
	{
		Renderer::submitQuad(
			texPtr.texture, &texRect.rect, &color.color, shaderPtr.shader, body.rect.getTopLeft(), body.rect.getSize(), z.z);
	});
}

void RenderSystem::submitSingleColorSpritesWithRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::Color, component::Rotation>
		(entt::exclude<component::TexturePtr, component::HiddenForRenderer>);

	view.each([this](const component::BodyRect& body, const component::Z z, const component::Color& color, const component::Rotation rotation)
	{
		Renderer::submitQuad(nullptr, nullptr, &color.color, nullptr, body.rect.getTopLeft(), body.rect.getSize(), z.z, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::TexturePtr, component::Rotation>
		(entt::exclude<component::BodyRect, component::HiddenForRenderer>);

	view.each([this](const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, const component::Rotation rotation)
	{
		Renderer::submitQuad(texPtr.texture, nullptr, nullptr, nullptr, body.rect.getTopLeft(), body.rect.getSize(), z.z, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithCustomShaderAndRotation() const
{
	auto view = mRegistry.view
		<const component::BodyRect, component::Z, const component::TexturePtr, const component::ShaderPtr, component::Rotation>
		(entt::exclude<component::TextureRect, component::HiddenForRenderer>);

	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, const component::ShaderPtr shaderPtr, const component::Rotation rotation)
	{
		Renderer::submitQuad(
			texPtr.texture, nullptr, nullptr, shaderPtr.shader, body.rect.getTopLeft(), body.rect.getSize(), z.z, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithTextureRectAndRotation() const
{
	auto view = mRegistry.view
		<const component::BodyRect, component::Z, const component::TexturePtr, const component::TextureRect, component::Rotation>
		(entt::exclude<component::ShaderPtr, component::HiddenForRenderer>);

	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, const component::TextureRect& texRect,
	 const component::Rotation rotation)
	{
		Renderer::submitQuad(
			texPtr.texture, &texRect.rect, nullptr, nullptr, body.rect.getTopLeft(), body.rect.getSize(), z.z, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithTextureRectCustomShaderAndRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::TexturePtr, component::TextureRect, component::ShaderPtr, component::Rotation>
		(entt::exclude<component::Color, component::HiddenForRenderer>);

	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, const component::TextureRect& texRect,
	 const component::ShaderPtr shaderPtr, const component::Rotation rotation)
	{
		Renderer::submitQuad(
			texPtr.texture, &texRect.rect, nullptr, shaderPtr.shader, body.rect.getSize(), body.rect.getSize(), z.z, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorMultiplicationAndRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::TexturePtr, component::Color, component::Rotation>
		(entt::exclude<component::TextureRect, component::ShaderPtr, component::HiddenForRenderer>);

	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, 
	 const component::Color& color, const component::Rotation rotation)
	{
		Renderer::submitQuad(
			texPtr.texture, nullptr, &color.color, nullptr, body.rect.getTopLeft(), body.rect.getSize(), z.z, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorCustomShaderAndRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::TexturePtr, component::Color, component::ShaderPtr, component::Rotation>
		(entt::exclude<component::TextureRect, component::HiddenForRenderer>);

	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, const component::Color& color,
	 const component::ShaderPtr shaderPtr, const component::Rotation rotation)
	{
		Renderer::submitQuad(
			texPtr.texture, nullptr, &color.color, shaderPtr.shader, body.rect.getTopLeft(), body.rect.getSize(), z.z, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorTextureRectAndRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::TexturePtr, component::TextureRect, component::Color, component::Rotation>
		(entt::exclude<component::ShaderPtr, component::HiddenForRenderer>);

	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, const component::TextureRect& texRect,
	 const component::Color& color, const component::Rotation rotation)
	{
		Renderer::submitQuad(
			texPtr.texture, &texRect.rect, &color.color, nullptr, body.rect.getTopLeft(), body.rect.getSize(), z.z, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorTextureRectCustomShaderAndRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Z, component::TexturePtr, component::TextureRect, component::Color, component::ShaderPtr, component::Rotation>
		(entt::exclude<component::HiddenForRenderer>);

	view.each([this]
	(const component::BodyRect& body, const component::Z z, const component::TexturePtr texPtr, const component::TextureRect& texRect,
	 const component::Color& color, const component::ShaderPtr shaderPtr, const component::Rotation rotation)
	{
		Renderer::submitQuad(texPtr.texture, &texRect.rect, &color.color, shaderPtr.shader,
			body.rect.getTopLeft(), body.rect.getSize(), z.z, rotation.angle);
	});
}

}
