#include "controlsGuide.hpp"
#include "Scenes/sceneManager.hpp"
#include "Gui/gui.hpp"
#include <SFML/Window.hpp>

namespace ph {

ContolsGuide::ContolsGuide(GameObject& root, GUI& gui, SceneManager& sceneManager)
	:CutScene(root)
	,mGui(gui)
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

void ContolsGuide::update(const sf::Time delta)
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
			greyBackground->getWidget("enterHint")->show();
			break;
		case 3:
			greyBackground->getWidget("enterHint")->hide();
			greyBackground->getWidget("backslashHint")->show();
			break;
		case 4:
			greyBackground->getWidget("backslashHint")->hide();
			closeCutscene();
			break;
		default:
			closeCutscene();
			break;
	}
}

void ContolsGuide::closeCutscene()
{
	mGui.getInterface("hints")->hide();
	mSceneManager.getScene().setPause(false);
	mIsActive = false;
}

}
