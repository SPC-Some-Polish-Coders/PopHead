#include "GameObjects/NonDrawableGameObjects/cutsceneArea.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "Utilities/rect.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

#include "Scenes/CutScenes/fightControlsGuide.hpp"
#include "Scenes/CutScenes/gateGuardDialogue.hpp"
#include "Scenes/CutScenes/endingCutscene.hpp"

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

		if(Rect<float>::doPositiveRectsIntersect(mPlayer->getGlobalBounds(), mArea)) {
			mActivated = true;
			createCutScene();
		}
		else
			mActivated = false;

	}

	void CutSceneArea::createCutScene()
	{
		auto& cutsceneManager = mGameData->getSceneManager().getScene().getCutSceneManager();
		if(mCutSceneName == "fightControlsGuide")
			cutsceneManager.activateCutscene(std::make_unique<FightControlsGuide>(
				*mRoot,
				mGameData->getGui(),
				mGameData->getSceneManager()
			));
		else if(mCutSceneName == "gateGuardDialogue")
			cutsceneManager.activateCutscene(std::make_unique<GateGuardDialogue>(mGameData));
		else if(mCutSceneName == "endingDialogue")
			cutsceneManager.activateCutscene(std::make_unique<EndingCutScene>(
				*mRoot,
				mGameData->getGui(),
				mGameData->getMusicPlayer(),
				mGameData->getRenderer().getCamera(),
				mGameData->getAIManager(),
				mGameData->getSceneManager()
			));
	}

}