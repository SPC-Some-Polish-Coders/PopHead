#include "movementControlsGuide.hpp"
#include "Scenes/sceneManager.hpp"
#include "Gui/gui.hpp"
#include "Logs/logs.hpp"
#include <SFML/Window.hpp>

namespace ph {

MovementContolsGuide::MovementContolsGuide(GUI& gui, SceneManager& sceneManager)
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

void MovementContolsGuide::update(const sf::Time delta)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mTimeSinceLastSkipPress.getElapsedTime().asSeconds() > 0.3f) {
		++mTimesPressedSkip;
		mTimeSinceLastSkipPress.restart();
	}

	auto* greyBackground = mGui.getInterface("hints")->getWidget("canvas")->getWidget("smallGreyBackground");
	switch(mTimesPressedSkip)
	{
		case 1:
			greyBackground->getWidget("awsdHint")->show();
			break;
		case 2:
 			greyBackground->getWidget("awsdHint")->hide();
			closeCutscene();
			break;
		default:
			PH_UNEXPECTED_SITUATION("mTimesPressedSkip should be either 1 or 2");
	}
}

void MovementContolsGuide::closeCutscene()
{
	mGui.getInterface("hints")->hide();
	mSceneManager.getScene().setPause(false);
	mIsActive = false;
}

}
