#include "startGameCutscene.hpp"
#include "GameObjects/gameObject.hpp"
#include "GameObjects/DrawableGameObjects/car.hpp"
#include "Renderer/camera.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Gui/gui.hpp"
#include "GameObjects/DrawableGameObjects/Characters/Npcs/crawlingNpc.hpp"
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
	,mHasStartedToSlowDown(false)
	,mHasChangedTheMusic(false)
	,mWasNpcCreated(false)
	,mHasPlayerTurnedToNpc(false)
{
	auto& car = dynamic_cast<Car&>(root.getChild("car"));
	car.setVelocity(120);
}

void StartGameCutScene::update(const sf::Time delta)
{
	const float cutsceneTimeInSeconds = mClock.getElapsedTime().asSeconds();

	auto& car = dynamic_cast<Car&>(mRoot.getChild("car"));
	mCamera.setCenter(car.getPosition() + sf::Vector2f(15, 10));

	updateGui(cutsceneTimeInSeconds, car);

	if(cutsceneTimeInSeconds < 5)
		car.speedUp();

	if(car.getPosition().x > 4965 && !mHasStartedToSlowDown) {
		mSoundPlayer.playAmbientSound("sounds/carTireScreech.ogg");
		mHasStartedToSlowDown = true;
	}
	
	if(car.getPosition().x > 5000)
		car.slowDown();

	if(cutsceneTimeInSeconds > 19 && !mHasChangedTheMusic) {
		mMusicPlayer.play("music/Menu.ogg");
		mHasChangedTheMusic = true;
	}

	if(cutsceneTimeInSeconds > 23 && !mWasNpcCreated) {
		auto playerNpc = std::make_unique<Npc>(mGameData, "playerNpc");
		playerNpc->setPosition({5640, 400});
		mRoot.addChild(std::move(playerNpc));
		mWasNpcCreated = true;
	}

	if(cutsceneTimeInSeconds > 24 && !mHasPlayerTurnedToNpc) {
		auto& playerNpc = dynamic_cast<Character&>(mRoot.getChild("playerNpc"));
		playerNpc.setAnimationState("rightUp");
		mHasPlayerTurnedToNpc = true;
		mCamera.setSize({320, 240});
	}

	if(cutsceneTimeInSeconds > 26.5) {
		auto& crawlingNpc = dynamic_cast<CrawlingNpc&>(mRoot.getChild("crawlingNpc"));
		crawlingNpc.die();
	}
}

void StartGameCutScene::updateGui(const float cutsceneTimeInSeconds, Car& car)
{
	auto canvas = mGui.getInterface("labels")->getWidget("canvas");
	if(cutsceneTimeInSeconds < 4) {
		canvas->getWidget("place")->hide();
		canvas->getWidget("time")->hide();
		canvas->getWidget("velocity")->hide();
		canvas->getWidget("speechBubble")->hide();
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
	else if(cutsceneTimeInSeconds > 22 && cutsceneTimeInSeconds < 24.5) {
		canvas->getWidget("velocity")->hide();
	}
	else if(cutsceneTimeInSeconds > 24 && cutsceneTimeInSeconds < 29) {
		auto speechBubble = canvas->getWidget("speechBubble");
		speechBubble->show();
		speechBubble->getWidget("speech2")->hide();
		speechBubble->getWidget("speech3")->hide();
	}
	else if(cutsceneTimeInSeconds > 28 && cutsceneTimeInSeconds < 31) {
		auto speechBubble = canvas->getWidget("speechBubble");
		speechBubble->getWidget("speech1")->hide();
		speechBubble->getWidget("speech1b")->hide();
		speechBubble->getWidget("speech2")->show();
	}
	else if(cutsceneTimeInSeconds > 31 && cutsceneTimeInSeconds < 35) {
		auto speechBubble = canvas->getWidget("speechBubble");
		speechBubble->getWidget("speech2")->hide();
		speechBubble->getWidget("speech3")->show();
	}
}

}
