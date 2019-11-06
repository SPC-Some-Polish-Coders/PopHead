#pragma once

#include "ECS/system.hpp"
#include "Renderer/camera.hpp"

namespace sf {
	class Window;	
}

namespace ph::system {

class RenderSystem : public System
{
public:
	RenderSystem(entt::registry& registry, sf::Window& window);

	void update(float seconds) override;

private:
	void submitSingleColorSprites() const;
	void submitTextureSprites() const;
	void submitTextureSpritesWithCustomShader() const;
	void submitTextureSpritesWithTextureRect() const;
	void submitTextureSpritesWithTextureRectAndCustomShader() const;
	void submitTextureSpritesWithSingleColorMultiplication() const;
	void submitTextureSpritesWithSingleColorAndCustomShader() const;
	void submitTextureSpritesWithSingleColorAndTextureRect() const;
	void submitTextureSpritesWithSingleColorTextureRectAndCustomShader() const;
	void submitSingleColorSpritesWithRotation() const;
	void submitTextureSpritesWithRotation() const;
	void submitTextureSpritesWithCustomShaderAndRotation() const;
	void submitTextureSpritesWithTextureRectAndRotation() const;
	void submitTextureSpritesWithTextureRectCustomShaderAndRotation() const;
	void submitTextureSpritesWithSingleColorMultiplicationAndRotation() const;
	void submitTextureSpritesWithSingleColorCustomShaderAndRotation() const;
	void submitTextureSpritesWithSingleColorTextureRectAndRotation() const;
	void submitTextureSpritesWithSingleColorTextureRectCustomShaderAndRotation() const;

private:
	Camera mCamera;
	sf::Window& mWindow;
};

}
