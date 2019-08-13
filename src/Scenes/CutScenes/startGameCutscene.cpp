#include "startGameCutscene.hpp"
#include "GameObjects/gameObject.hpp"
#include "GameObjects/DrawableGameObjects/car.hpp"
#include "Renderer/camera.hpp"
#include "Audio/Sound/soundPlayer.hpp"

namespace ph {

StartGameCutScene::StartGameCutScene(GameObject& root, Camera& camera, SoundPlayer& soundPlayer)
	:CutScene(root)
	,mCamera(camera)
	,mSoundPlayer(soundPlayer)
	,mHasStartedToSlowDown(false)
{
	auto& car = dynamic_cast<Car&>(root.getChild("car"));
	car.setVelocity(120);
}

void StartGameCutScene::update(const sf::Time delta)
{
	auto& car = dynamic_cast<Car&>(mRoot.getChild("car"));
	mCamera.setCenter(car.getPosition());

	if(mClock.getElapsedTime().asSeconds() < 5)
		car.speedUp();

	if(car.getPosition().x > 2900) {
		if(!mHasStartedToSlowDown) {
			mSoundPlayer.playAmbientSound("sounds/carTireScreech.ogg");
			mHasStartedToSlowDown = true;
		}
		car.slowDown();
	}
}

}
