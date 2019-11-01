#pragma once

#include "Renderer/camera.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

namespace ph {

class Map;

class Renderer
{
public:
	Renderer(sf::RenderTarget&);
	~Renderer();
	Renderer(Renderer&) = delete;
	Renderer& operator=(Renderer&) = delete;

	enum Viewports{ FullScreenViewport };

	void update(sf::Time delta);

	void startSceneRendering();
	void startUIRendering();

	void draw(const sf::Drawable&) const;
	void draw(const Map&) const;

	auto getCamera() -> Camera& { return mCamera; }
	void setDebugRenderingMode(bool mode) { mDebugRenderingMode = mode; }
	void startShaking(float shakeStrength);
	void moveCamera(sf::Vector2f center, float speed);

private:
	sf::FloatRect getProperCameraBounds() const;

private:
	Camera mCamera;
	Camera mStaticObjectsCamera;
	const std::map< Viewports, sf::Rect< float > > mViewports;
	sf::RenderTarget& mRenderTarget;
	sf::FloatRect mProperCameraBounds;
	bool mDebugRenderingMode;
};

}
