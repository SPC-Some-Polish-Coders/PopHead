#include "renderSystem.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Renderer/renderer.hpp"

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

	auto bodyTextureView = mRegistry.view<component::BodyRect, component::TextureRef>();
	bodyTextureView.each([this](const component::BodyRect& body, const component::TextureRef textureRef) {
		Renderer::submitQuad(*textureRef.texture, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});

	auto bodyTextureShaderView = mRegistry.view<const component::BodyRect, const component::TextureRef, const component::ShaderRef>();
	bodyTextureShaderView.each([this](const component::BodyRect& body, const component::TextureRef textureRef, const component::ShaderRef shaderRef) {
		Renderer::submitQuad(*textureRef.texture, shaderRef.shader, body.rect.getTopLeft(), static_cast<sf::Vector2i>(body.rect.getSize()));
	});
}

}
