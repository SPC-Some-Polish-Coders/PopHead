#include "renderSystem.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Renderer/renderer.hpp"
#include "entt/entity/utility.hpp"

namespace ph::system {

RenderSystem::RenderSystem(entt::registry& registry, sf::Window& window)
	:System(registry)
	,mCamera()
	,mWindow(window)
{
}

void RenderSystem::update(float seconds)
{
	// TODO_ren: Move camera somewhere. To separate system for example.
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
		mCamera.zoom(1.04f);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
		mCamera.zoom(0.96f);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		mCamera.move({-1.f, 0.f});
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		mCamera.move({1.f, 0.f});
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		mCamera.move({0.f, -1.f});
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		mCamera.move({0.f, 1.f});
	
	// NOTE: beginScene() should be probably where endScene() is
	Renderer::beginScene(mCamera);

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

void RenderSystem::submitSingleColorSprites() const
{
	auto bodyColorView = mRegistry.view
		<component::BodyRect, component::Color>
		(entt::exclude<component::TexturePtr, component::Rotation>);

	bodyColorView.each([this](const component::BodyRect& body, const component::Color& color)
	{
		Renderer::submitQuad(color.color, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

void RenderSystem::submitTextureSprites() const
{
	auto bodyTextureView = mRegistry.view
		<component::BodyRect, component::TexturePtr>
		(entt::exclude<component::BodyRect, component::Rotation>);

	bodyTextureView.each([this](const component::BodyRect& body, const component::TexturePtr texturePtr) 
	{
		Renderer::submitQuad(*texturePtr.texture, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

void RenderSystem::submitTextureSpritesWithCustomShader() const
{
	auto view = mRegistry.view
		<const component::BodyRect, const component::TexturePtr, const component::ShaderPtr>
		(entt::exclude<component::TextureRect, component::Rotation>);
	
	view.each([this](const component::BodyRect& body, const component::TexturePtr texturePtr, const component::ShaderPtr shaderPtr) 
	{
		Renderer::submitQuad(*texturePtr.texture, shaderPtr.shader, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

void RenderSystem::submitTextureSpritesWithTextureRect() const
{
	auto view = mRegistry.view
		<const component::BodyRect, const component::TexturePtr, const component::TextureRect>
		(entt::exclude<component::ShaderPtr, component::Rotation>);

	view.each([this](const component::BodyRect& body, const component::TexturePtr texturePtr, const component::TextureRect& textureRect) 
	{
		Renderer::submitQuad(*texturePtr.texture, textureRect.rect, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
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
		Renderer::submitQuad(*texPtr.texture, texRect.rect, shaderPtr.shader, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorMultiplication() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::Color>
		(entt::exclude<component::TextureRect, component::ShaderPtr, component::Rotation>);

	view.each([this](const component::BodyRect& body, const component::TexturePtr texturePtr, const component::Color& color) 
	{
		Renderer::submitQuad(*texturePtr.texture, color.color, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorAndCustomShader() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::Color, component::ShaderPtr>
		(entt::exclude<component::TextureRect, component::Rotation>);

	view.each([this](const component::BodyRect& body, const component::TexturePtr texturePtr, const component::Color& color, const component::ShaderPtr shaderPtr)
	{
		Renderer::submitQuad(*texturePtr.texture, color.color, shaderPtr.shader, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorAndTextureRect() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::TextureRect, component::Color>
		(entt::exclude<component::ShaderPtr, component::Rotation>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texturePtr, const component::TextureRect& textureRect, const component::Color& color)
	{
		Renderer::submitQuad(*texturePtr.texture, color.color, textureRect.rect, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
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
		Renderer::submitQuad(*texPtr.texture, color.color, texRect.rect, shaderPtr.shader,
			body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

void RenderSystem::submitSingleColorSpritesWithRotation() const
{
	auto bodyColorView = mRegistry.view
		<component::BodyRect, component::Color, component::Rotation>
		(entt::exclude<component::TexturePtr>);

	bodyColorView.each([this](const component::BodyRect& body, const component::Color& color, const component::Rotation rotation)
	{
		Renderer::submitQuad(color.color, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()), rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithRotation() const
{
	auto bodyTextureView = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::Rotation>
		(entt::exclude<component::BodyRect>);

	bodyTextureView.each([this](const component::BodyRect& body, const component::TexturePtr texturePtr, const component::Rotation rotation)
	{
		Renderer::submitQuad(*texturePtr.texture, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()), rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithCustomShaderAndRotation() const
{
	auto view = mRegistry.view
		<const component::BodyRect, const component::TexturePtr, const component::ShaderPtr, component::Rotation>
		(entt::exclude<component::TextureRect>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texturePtr, const component::ShaderPtr shaderPtr, const component::Rotation rotation)
	{
		Renderer::submitQuad(*texturePtr.texture, shaderPtr.shader, body.rect.getTopLeft(),
			static_cast<sf::Vector2i>(body.rect.getSize()), rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithTextureRectAndRotation() const
{
	auto view = mRegistry.view
		<const component::BodyRect, const component::TexturePtr, const component::TextureRect, component::Rotation>
		(entt::exclude<component::ShaderPtr>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texturePtr, const component::TextureRect& textureRect,
	 const component::Rotation rotation)
	{
		Renderer::submitQuad(*texturePtr.texture, textureRect.rect, body.rect.getTopLeft(),
			static_cast<sf::Vector2i>(body.rect.getSize()), rotation.angle);
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
		Renderer::submitQuad(*texPtr.texture, texRect.rect, shaderPtr.shader, body.rect.getTopLeft(),
			static_cast<sf::Vector2i>(body.rect.getSize()), rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorMultiplicationAndRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::Color, component::Rotation>
		(entt::exclude<component::TextureRect, component::ShaderPtr>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texturePtr, const component::Color& color, const component::Rotation rotation)
	{
		Renderer::submitQuad(*texturePtr.texture, color.color, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()), rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorCustomShaderAndRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::Color, component::ShaderPtr, component::Rotation>
		(entt::exclude<component::TextureRect>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texturePtr, const component::Color& color,
	 const component::ShaderPtr shaderPtr, const component::Rotation rotation)
	{
		Renderer::submitQuad(*texturePtr.texture, color.color, shaderPtr.shader, body.rect.getTopLeft(),
			static_cast<sf::Vector2i>(body.rect.getSize()), rotation.angle);
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorTextureRectAndRotation() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::TextureRect, component::Color, component::Rotation>
		(entt::exclude<component::ShaderPtr>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texturePtr, const component::TextureRect& textureRect,
	 const component::Color& color, const component::Rotation rotation)
	{
		Renderer::submitQuad(*texturePtr.texture, color.color, textureRect.rect, body.rect.getTopLeft(),
			static_cast<sf::Vector2i>(body.rect.getSize()), rotation.angle);
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
		Renderer::submitQuad(*texPtr.texture, color.color, texRect.rect, shaderPtr.shader,
			body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()), rotation.angle);
	});
}

}
