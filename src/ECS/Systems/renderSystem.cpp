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
	submitTextureSpritesWithSingleColorMultiplicationRect();
	submitTextureSpritesWithSingleColorAndCustomShader();
}

void RenderSystem::submitSingleColorSprites() const
{
	auto bodyColorView = mRegistry.view
		<component::BodyRect, component::Color>
		(entt::exclude<component::TexturePtr>);

	bodyColorView.each([this](const component::BodyRect& body, const component::Color& color)
	{
		Renderer::submitQuad(color.color, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

void RenderSystem::submitTextureSprites() const
{
	auto bodyTextureView = mRegistry.view
		<component::BodyRect, component::TexturePtr>
		(entt::exclude<component::BodyRect>);

	bodyTextureView.each([this](const component::BodyRect& body, const component::TexturePtr texturePtr) 
	{
		Renderer::submitQuad(*texturePtr.texture, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

void RenderSystem::submitTextureSpritesWithCustomShader() const
{
	auto view = mRegistry.view
		<const component::BodyRect, const component::TexturePtr, const component::ShaderPtr>
		(entt::exclude<component::TextureRect>);
	
	view.each([this](const component::BodyRect& body, const component::TexturePtr texturePtr, const component::ShaderPtr shaderPtr) 
	{
		Renderer::submitQuad(*texturePtr.texture, shaderPtr.shader, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

void RenderSystem::submitTextureSpritesWithTextureRect() const
{
	auto view = mRegistry.view
		<const component::BodyRect, const component::TexturePtr, const component::TextureRect>
		(entt::exclude<component::ShaderPtr>);

	view.each([this](const component::BodyRect& body, const component::TexturePtr texturePtr, const component::TextureRect& textureRect) 
	{
		Renderer::submitQuad(*texturePtr.texture, textureRect.rect, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

void RenderSystem::submitTextureSpritesWithTextureRectAndCustomShader() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::TextureRect, component::ShaderPtr>
		(entt::exclude<component::Color>);

	view.each([this]
	(const component::BodyRect& body, const component::TexturePtr texPtr, const component::TextureRect& texRect, const component::ShaderPtr shaderPtr)
	{
		Renderer::submitQuad(*texPtr.texture, texRect.rect, shaderPtr.shader, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorMultiplicationRect() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::Color>
		(entt::exclude<component::TextureRect, component::ShaderPtr>);

	view.each([this](const component::BodyRect& body, const component::TexturePtr texturePtr, const component::Color& color) 
	{
		Renderer::submitQuad(*texturePtr.texture, color.color, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

void RenderSystem::submitTextureSpritesWithSingleColorAndCustomShader() const
{
	auto view = mRegistry.view
		<component::BodyRect, component::TexturePtr, component::Color, component::ShaderPtr>
		(entt::exclude<component::TextureRect>);

	view.each([this](const component::BodyRect& body, const component::TexturePtr texturePtr, const component::Color& color, const component::ShaderPtr shaderPtr)
	{
		Renderer::submitQuad(*texturePtr.texture, color.color, shaderPtr.shader, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

}
