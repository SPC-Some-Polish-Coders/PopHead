#include "GameObjects/NonDrawableGameObjects/cutsceneArea.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "Utilities/rect.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

#include "Scenes/CutScenes/gateGuardDialogue.hpp"

namespace ph {

	CutSceneArea::CutSceneArea(GameData* const gameData, const std::string& cutSceneName, const sf::FloatRect area)
		:GameObject("cutSceneArea")
		,mGameData(gameData)
		,mArea(area)
		,mActivated(false)
		,mCutSceneName(cutSceneName)
	{
		mPlayer = dynamic_cast<Player*>(mRoot->getChild("LAYER_standingObjects")->getChild("player"));
	}

	void CutSceneArea::updateCurrent(const sf::Time delta)
	{
		if (mPlayer == nullptr || mPlayer->isDead() || mActivated)
			return;

		if (Rect<float>::doPositiveRectsIntersect(mPlayer->getGlobalBounds(), mArea))
			mActivated = true;
		else
			mActivated = false;

		if (mActivated)
			createCutScene();
	}

	void CutSceneArea::createCutScene()
	{
		if (mCutSceneName == "gateGuardDialogue")
			mGameData->getSceneManager().getScene().getCutSceneManager().activateCutscene(std::make_unique<GateGuardDialogue>(mGameData));
	}

}