#include "fightControlsGuide.hpp"
#include "Scenes/sceneManager.hpp"
#include "GUI/gui.hpp"
#include "Logs/logs.hpp"
#include <SFML/Window.hpp>

namespace ph {

FightControlsGuide::FightControlsGuide(GUI& gui, SceneManager& sceneManager)
	:mGui(gui)
	,mSceneManager(sceneManager)
	,mTimesPressedSkip(1)
{
	mSceneManager.getScene().setPause(true);

	auto* hints = mGui.getInterface("hints");
	hints->show();
	auto* greyBackground = hints->getWidget("canvas")->getWidget("smallGreyBackground");
	greyBackground->getWidget("awsdHint")->hide();
	greyBackground->getWidget("enterHint")->hide();
	greyBackground->getWidget("backslashHint")->hide();
}

void FightControlsGuide::update(const sf::Time dt)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mTimeSinceLastSkipPress.getElapsedTime().asSeconds() > 0.3f) {
		++mTimesPressedSkip;
		mTimeSinceLastSkipPress.restart();
	}

	auto* greyBackground = mGui.getInterface("hints")->getWidget("canvas")->getWidget("smallGreyBackground");
	switch(mTimesPressedSkip)
	{
	case 1:
		greyBackground->getWidget("enterHint")->show();
		break;
	case 2:
		greyBackground->getWidget("enterHint")->hide();
		greyBackground->getWidget("backslashHint")->show();
		break;
	case 3:
		greyBackground->getWidget("backslashHint")->hide();
		closeCutscene();
		break;
	default:
		PH_UNEXPECTED_SITUATION("mTimesPressedSkip should be either 1, 2 or 3");
	}
}

void FightControlsGuide::closeCutscene()
{
	mGui.getInterface("hints")->hide();
	mSceneManager.getScene().setPause(false);
	mIsActive = false;
}

}
