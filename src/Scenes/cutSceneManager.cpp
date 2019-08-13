#include "cutSceneManager.hpp"

namespace ph {

CutSceneManager::CutSceneManager()
	:mMapStaringCutScene(nullptr)
{
}

void CutSceneManager::setMapStaringCutScene(std::unique_ptr<CutScene> startingCutScene)
{
	mMapStaringCutScene = std::move(startingCutScene);
}

void CutSceneManager::updateCutScene(const sf::Time delta)
{
	mMapStaringCutScene->update(delta);
}

bool CutSceneManager::isCutSceneActive()
{
	return mMapStaringCutScene != nullptr && mMapStaringCutScene->isActive();
}

}