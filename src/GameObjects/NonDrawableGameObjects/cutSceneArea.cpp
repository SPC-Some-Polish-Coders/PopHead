#include "GameObjects/NonDrawableGameObjects/cutsceneArea.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "Utilities/math.hpp"
#include "Logs/logs.hpp"

namespace ph {

	CutSceneArea::CutSceneArea(const std::string& cutSceneName, const sf::FloatRect area)
		:GameObject("cutSceneArea")
		,mArea(area)
		,mActivated(false)
		,mCutSceneName(cutSceneName)
	{
		mPlayer = dynamic_cast<Player*>(mRoot->getChild("LAYER_standingObjects")->getChild("player"));
	}

	void CutSceneArea::updateCurrent(const sf::Time delta)
	{
		if (mPlayer == nullptr || mPlayer->isDead())
			return;

		if (Math::areTheyOverlapping(mPlayer->getGlobalBounds(), mArea))
			mActivated = true;
		else
			mActivated = false;

		if (mActivated)
			createCutScene();
	}

	void CutSceneArea::createCutScene()
	{
		if (mCutSceneName == "gateGuardDialogue");
			//createCutScene
	}

}