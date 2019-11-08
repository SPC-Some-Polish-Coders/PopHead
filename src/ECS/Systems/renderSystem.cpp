#include "renderSystem.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/camera.hpp"
#include "entt/entity/utility.hpp"

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
	return *currentCamera;
}

void RenderSystem::submitSingleColorSprites() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Color>
		(entt::exclude<component::TexturePtr, component::Rotation>);

	view.each([this](const component::BodyRect& body, const component::Color& color)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(nullptr, nullptr, &color.color, nullptr, pos, size);
	});
}

void RenderSystem::submitTextureSprites() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr>
		(entt::exclude<component::BodyRect, component::Rotation>);

	view.each([this](const component::BodyRect& body, const component::TexturePtr texPtr) 
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, nullptr, nullptr, nullptr, pos, size);
	});
}

void RenderSystem::submitTextureSpritesWithCustomShader() const
{
	auto view = mRegistry.view
		<const component::BodyRect, const component::TexturePtr, const component::ShaderPtr>
		(entt::exclude<component::TextureRect, component::Rotation>);
	
	view.each([this](const component::BodyRect& body, const component::TexturePtr texPtr, const component::ShaderPtr shaderPtr) 
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, nullptr, nullptr, shaderPtr.shader, pos, size);
	});
}

void RenderSystem::submitTextureSpritesWithTextureRect() const
{
	auto view = mRegistry.view
		<const component::BodyRect, const component::TexturePtr, const component::TextureRect>
		(entt::exclude<component::ShaderPtr, component::Rotation>);

	view.each([this](const component::BodyRect& body, const component::TexturePtr texPtr, const component::TextureRect& texRect) 
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, &texRect.rect, nullptr, nullptr, pos, size);
	});
}

void RenderSystem::submitTextureSpritesWithTextureRectAndCustomShader() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::TextureRect, component::ShaderPtr>
		(entt::exclude<component::Color, component::Rotation>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texPtr, const component::TextureRect& texRect, const component::ShaderPtr shaderPtr)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, &texRect.rect, nullptr, shaderPtr.shader, pos, size);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorMultiplication() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::Color>
		(entt::exclude<component::TextureRect, component::ShaderPtr, component::Rotation>);

	view.each([this](const component::BodyRect& body, const component::TexturePtr texPtr, const component::Color& color) 
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, nullptr, &color.color, nullptr, pos, size);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorAndCustomShader() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::Color, component::ShaderPtr>
		(entt::exclude<component::TextureRect, component::Rotation>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texPtr, const component::Color& color, const component::ShaderPtr shaderPtr)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, nullptr, &color.color, shaderPtr.shader, pos, size);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorAndTextureRect() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::TextureRect, component::Color>
		(entt::exclude<component::ShaderPtr, component::Rotation>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texPtr, const component::TextureRect& texRect, const component::Color& color)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, &texRect.rect, &color.color, nullptr, pos, size);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorTextureRectAndCustomShader() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::TextureRect, component::Color, component::ShaderPtr>
		(entt::exclude<component::Rotation>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texPtr, const component::TextureRect& texRect, const component::Color& color,
	 const component::ShaderPtr shaderPtr)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, &texRect.rect, &color.color, shaderPtr.shader, pos, size);
	});
}

void RenderSystem::submitSingleColorSpritesWithRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::Color, component::Rotation>
		(entt::exclude<component::TexturePtr>);

	view.each([this](const component::BodyRect& body, const component::Color& color, const component::Rotation rotation)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(nullptr, nullptr, &color.color, nullptr, pos, size, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::Rotation>
		(entt::exclude<component::BodyRect>);

	view.each([this](const component::BodyRect& body, const component::TexturePtr texPtr, const component::Rotation rotation)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, nullptr, nullptr, nullptr, pos, size, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithCustomShaderAndRotation() const
{
	auto view = mRegistry.view
		<const component::BodyRect, const component::TexturePtr, const component::ShaderPtr, component::Rotation>
		(entt::exclude<component::TextureRect>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texPtr, const component::ShaderPtr shaderPtr, const component::Rotation rotation)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, nullptr, nullptr, shaderPtr.shader, pos, size, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithTextureRectAndRotation() const
{
	auto view = mRegistry.view
		<const component::BodyRect, const component::TexturePtr, const component::TextureRect, component::Rotation>
		(entt::exclude<component::ShaderPtr>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texPtr, const component::TextureRect& texRect,
	 const component::Rotation rotation)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, &texRect.rect, nullptr, nullptr, pos, size, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithTextureRectCustomShaderAndRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::TextureRect, component::ShaderPtr, component::Rotation>
		(entt::exclude<component::Color>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texPtr, const component::TextureRect& texRect,
	 const component::ShaderPtr shaderPtr, const component::Rotation rotation)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, &texRect.rect, nullptr, shaderPtr.shader, pos, size, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorMultiplicationAndRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::Color, component::Rotation>
		(entt::exclude<component::TextureRect, component::ShaderPtr>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texPtr, const component::Color& color, const component::Rotation rotation)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, nullptr, &color.color, nullptr, pos, size, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorCustomShaderAndRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::Color, component::ShaderPtr, component::Rotation>
		(entt::exclude<component::TextureRect>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texPtr, const component::Color& color,
	 const component::ShaderPtr shaderPtr, const component::Rotation rotation)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, nullptr, &color.color, shaderPtr.shader, pos, size, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorTextureRectAndRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::TextureRect, component::Color, component::Rotation>
		(entt::exclude<component::ShaderPtr>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texPtr, const component::TextureRect& texRect,
	 const component::Color& color, const component::Rotation rotation)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, &texRect.rect, &color.color, nullptr, pos, size, rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorTextureRectCustomShaderAndRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::TextureRect, component::Color, component::ShaderPtr, component::Rotation>();

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texPtr, const component::TextureRect& texRect, const component::Color& color,
	 const component::ShaderPtr shaderPtr, const component::Rotation rotation)
	{
		auto pos = body.rect.getTopLeft();
		auto size = static_cast<sf::Vector2i>(body.rect.getSize());
		Renderer::submitQuad(texPtr.texture, &texRect.rect, &color.color, shaderPtr.shader, pos, size, rotation.angle);
	});
}

}
