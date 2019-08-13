#pragma once

#include "cutScene.hpp"
#include <SFML/System.hpp>
#include <memory>

namespace ph {

class TiledGameObjectParser;

class CutSceneManager
{
public:
	CutSceneManager();

	void setMapStaringCutScene(std::unique_ptr<CutScene> startingCutScene);

	void updateCutScene(const sf::Time delta);

	bool isCutSceneActive();

private:
	std::unique_ptr<CutScene> mMapStaringCutScene;
};

}
