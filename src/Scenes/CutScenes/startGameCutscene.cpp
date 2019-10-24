#include "startGameCutscene.hpp"
#include "GameObjects/gameObject.hpp"
#include "GameObjects/DrawableGameObjects/car.hpp"
#include "Renderer/camera.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Gui/gui.hpp"
#include "GameObjects/DrawableGameObjects/Characters/npc.hpp"
#include "gameData.hpp"

namespace ph {

StartGameCutScene::StartGameCutScene(GameObject& root, Camera& camera, SoundPlayer& soundPlayer, MusicPlayer& musicPlayer,
	GUI& gui, GameData* const gameData)
	:CutScene(root)
	,mCamera(camera)
	,mSoundPlayer(soundPlayer)
	,mMusicPlayer(musicPlayer)
	,mGui(gui)
	,mGameData(gameData)
	,mCutsceneTimeInSeconds(0.f)
	,mWasGuiHidden(false)
	,mHasStartedToSlowDown(false)
	,mWasPlayerCreated(false)
{
	auto* car = dynamic_cast<Car*>(root.getChild("LAYER_lyingObjects")->getChild("car"));
	car->setVelocity(120);
}

void StartGameCutScene::update(const sf::Time delta)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		closeCutScene();

	mCutsceneTimeInSeconds += delta.asSeconds();
	
	auto canvas = mGui.getInterface("labels")->getWidget("canvas");
	
	// HIDE ALL GUI
	if(!mWasGuiHidden) {
		canvas->getWidget("place")->hide();
		canvas->getWidget("time")->hide();
		canvas->getWidget("velocity")->hide();
		canvas->getWidget("speechBubble")->hide();
		mWasGuiHidden = true;
	}

	auto& car = dynamic_cast<Car&>(*mRoot.getChild("LAYER_lyingObjects")->getChild("car"));
	
	/*if(mCutsceneTimeInSeconds < 23)
		mCamera.move(car.getPosition() + sf::Vector2f(15, 10), 10.f * delta.asSeconds());*/

	if(mCutsceneTimeInSeconds < 5)
		car.speedUp();

	// NARRATIVE SUBTITLES
	if(mCutsceneTimeInSeconds > 4 && mCutsceneTimeInSeconds < 8) {
		canvas->getWidget("place")->show();
	}
	else if(mCutsceneTimeInSeconds > 8 && mCutsceneTimeInSeconds < 10) {
		canvas->getWidget("place")->hide();
	}
	else if(mCutsceneTimeInSeconds > 10 && mCutsceneTimeInSeconds < 15) {
		canvas->getWidget("time")->show();
	}
	else if(mCutsceneTimeInSeconds > 15 && mCutsceneTimeInSeconds < 17) {
		canvas->getWidget("time")->hide();
	}
	else if(mCutsceneTimeInSeconds > 17 && mCutsceneTimeInSeconds < 22) {
		auto velocityWidget = dynamic_cast<TextWidget*>(canvas->getWidget("velocity"));
		velocityWidget->show();
		int velocity = static_cast<int>(car.getVelocity() / 4.7);
		velocityWidget->setString(std::to_string(velocity) + " MPH");
	}
	else if(mCutsceneTimeInSeconds > 22 && mCutsceneTimeInSeconds < 24.5) {
		canvas->getWidget("velocity")->hide();
	}

	// CAR STARTS TO SLOW DOWN
	if(car.getPosition().x > 5100 && !mHasStartedToSlowDown) {
		mSoundPlayer.playAmbientSound("sounds/carTireScreech.ogg");
		mHasStartedToSlowDown = true;
	}
	if(car.getPosition().x > 5110)
		car.slowDown();

	// AGENT LEAVES CAR
	if(mCutsceneTimeInSeconds > 23 && !mWasPlayerCreated) {
		auto playerNpc = std::make_unique<Npc>(mGameData, "playerNpc");
		playerNpc->setAnimationState("stayingRight");
		playerNpc->setPosition({5760, 392});
		mRoot.addChild(std::move(playerNpc));
		mWasPlayerCreated = true;
	}

	// AGENT TALKS
	auto* speechBubble = canvas->getWidget("speechBubble");
	if(mCutsceneTimeInSeconds > 24 && mCutsceneTimeInSeconds < 25) {
		speechBubble->getWidget("speech2")->hide();
		speechBubble->getWidget("speech2b")->hide();
		speechBubble->getWidget("speech3")->hide();
		speechBubble->getWidget("speech3b")->hide();
		
		speechBubble->show();
		speechBubble->getWidget("speech1")->show();
	}
	else if(mCutsceneTimeInSeconds > 25 && mCutsceneTimeInSeconds < 29) {
		speechBubble->getWidget("speech1")->hide();

		speechBubble->getWidget("speech2")->show();
		speechBubble->getWidget("speech2b")->show();
	}
	else if(mCutsceneTimeInSeconds > 29 && mCutsceneTimeInSeconds < 33) {
		speechBubble->getWidget("speech2")->hide();
		speechBubble->getWidget("speech2b")->hide();

		speechBubble->getWidget("speech3")->show();
		speechBubble->getWidget("speech3b")->show();
	}

	if(mCutsceneTimeInSeconds > 33) {
		speechBubble->hide();
		closeCutScene();
	}
}

void StartGameCutScene::closeCutScene()
{
	mGameData->getSceneManager().replaceScene("scenes/gateAreaMap.xml");
	mIsActive = false;
}

}
