#pragma once

#include "Renderer/camera.hpp"
#include "renderCommand.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

namespace ph {

class GameObject;
class Map;

class Renderer
{
public:
	Renderer(sf::RenderTarget&);
	
	Renderer(Renderer&) = delete;
	Renderer& operator=(Renderer&) = delete;

	void startSceneRendering();
	void startUIRendering();

	void draw(const sf::Drawable&);
	void draw(const Map&);
	void setUpModernOpenGlTest();
	void drawModernOpenGlTest();

	auto getCamera() -> Camera& { return mCamera; }
	void setDebugRenderingMode(bool mode) { mDebugRenderingMode = mode; }

private:
	sf::FloatRect getProperCameraBounds() const;

private:
	unsigned shaderProgram, vao;
	Camera mCamera;
	Camera mStaticObjectsCamera;
	RenderCommand mRenderCommand;
	sf::RenderTarget& mRenderTarget;
	bool mDebugRenderingMode;
};

}
