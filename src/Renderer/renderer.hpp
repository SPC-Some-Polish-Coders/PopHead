#pragma once

#include "Renderer/camera.hpp"
#include "Renderer/layer.hpp"
#include "Renderer/layerID.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

namespace ph {

class GameData;

class Renderer
{
public:
	Renderer(sf::RenderTarget&);
	~Renderer();
	Renderer(Renderer&) = delete;
	Renderer& operator=(Renderer&) = delete;

	enum Viewports{ FullScreenViewport };

	void update(sf::Time delta);
	void draw() const;

	void addObject(Object* const);
	void addObject(Object* const, LayerID);

	void removeObject(const Object* const);
	void removeObject(std::string name, LayerID);
	void removeAllObjectsFromLayer(LayerID);
	void clear() noexcept;

	void startShaking(float shakeStrength) { mCamera.setShakeStrength(shakeStrength); }
	void moveCamera(sf::Vector2f center, float speed) { mCamera.move(center, speed); }
	auto getCamera() -> Camera& { return mCamera; }

	void setGameData(GameData* gameData) { mGameData = gameData; }

private:
	void setPositionOfStaticObjectsToCamera();
	std::string getLayerName(LayerID) const;

private:
	Camera mCamera;
	const std::map< Viewports, sf::Rect< float > > mViewports;
	std::map< LayerID, Layer > mLayers;
	sf::RenderTarget& mRenderTarget;
	GameData* mGameData;
};

}
