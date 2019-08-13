#include "startGameCutscene.hpp"
#include "GameObjects/gameObject.hpp"
#include "GameObjects/DrawableGameObjects/car.hpp"
#include "Renderer/camera.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Gui/gui.hpp"

namespace ph {

StartGameCutScene::StartGameCutScene(GameObject& root, Camera& camera, SoundPlayer& soundPlayer, GUI& gui)
	:CutScene(root)
	,mCamera(camera)
	,mSoundPlayer(soundPlayer)
	,mGui(gui)
	,mHasStartedToSlowDown(false)
{
	auto& car = dynamic_cast<Car&>(root.getChild("car"));
	car.setVelocity(120);
}

void StartGameCutScene::update(const sf::Time delta)
{
	const float cutsceneTimeInSeconds = mClock.getElapsedTime().asSeconds();

	auto& car = dynamic_cast<Car&>(mRoot.getChild("car"));
	mCamera.setCenter(car.getPosition());

	updateGui(cutsceneTimeInSeconds, car);

	if(cutsceneTimeInSeconds < 5)
		car.speedUp();

	if(car.getPosition().x > 5000) {
		if(!mHasStartedToSlowDown) {
			mSoundPlayer.playAmbientSound("sounds/carTireScreech.ogg");
			mHasStartedToSlowDown = true;
		}
		car.slowDown();
	}
}

void StartGameCutScene::updateGui(const float cutsceneTimeInSeconds, Car& car)
{
	auto canvas = mGui.getInterface("labels")->getWidget("canvas");
	if(cutsceneTimeInSeconds < 4) {
		canvas->getWidget("place")->hide();
		canvas->getWidget("time")->hide();
		canvas->getWidget("velocity")->hide();
	}
	else if(cutsceneTimeInSeconds > 4 && cutsceneTimeInSeconds < 8) {
		canvas->getWidget("place")->show();
	}
	else if(cutsceneTimeInSeconds > 8 && cutsceneTimeInSeconds < 10) {
		canvas->getWidget("place")->hide();
	}
	else if(cutsceneTimeInSeconds > 10 && cutsceneTimeInSeconds < 15) {
		canvas->getWidget("time")->show();
	}
	else if(cutsceneTimeInSeconds > 15 && cutsceneTimeInSeconds < 17) {
		canvas->getWidget("time")->hide();
	}
	else if(cutsceneTimeInSeconds > 17 && cutsceneTimeInSeconds < 22) {
		auto velocityWidget = dynamic_cast<TextWidget*>(canvas->getWidget("velocity"));
		velocityWidget->show();
		int velocity = static_cast<int>(car.getVelocity() / 4.7);
		velocityWidget->setString(std::to_string(velocity) + " MPH");
	}
}

}
