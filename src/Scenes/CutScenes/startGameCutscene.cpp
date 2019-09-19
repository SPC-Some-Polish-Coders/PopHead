#include "startGameCutscene.hpp"
#include "GameObjects/gameObject.hpp"
#include "GameObjects/DrawableGameObjects/car.hpp"
#include "Renderer/camera.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Gui/gui.hpp"
#include "GameObjects/DrawableGameObjects/Characters/Npcs/crawlingNpc.hpp"
#include "GameObjects/DrawableGameObjects/bilbord.hpp"
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
	,mHasChangedTheMusicToMenuTheme(false)
	,mWasPlayerCreated(false)
	,mHasPlayerTurnedToNpc(false)
	,mHasBilbordFallenOver(false)
	,mWereZombieSpawned(false)
	,mHasChangedMusicToZombieAttackTheme(false)
{
	auto* car = dynamic_cast<Car*>(root.getChild("LAYER_standingObjects")->getChild("car"));
	car->setVelocity(120);
}

void StartGameCutScene::update(const sf::Time delta)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		closeCutScene();

	mCutsceneTimeInSeconds += delta.asSeconds();
	
	// HIDE ALL GUI
	if(!mWasGuiHidden) {
		auto canvas = mGui.getInterface("labels")->getWidget("canvas");
		canvas->getWidget("place")->hide();
		canvas->getWidget("time")->hide();
		canvas->getWidget("velocity")->hide();
		canvas->getWidget("speechBubble")->hide();
		mWasGuiHidden = true;
	}

	auto& car = dynamic_cast<Car&>(*mRoot.getChild("LAYER_standingObjects")->getChild("car"));
	
	if(mCutsceneTimeInSeconds < 23)
		mCamera.move(car.getPosition() + sf::Vector2f(15, 10), 10.f * delta.asSeconds());

	if(mCutsceneTimeInSeconds < 5)
		car.speedUp();

	// NARRATIVE SUBTITLES
	auto canvas = mGui.getInterface("labels")->getWidget("canvas");
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
	if(car.getPosition().x > 4965 && !mHasStartedToSlowDown) {
		mSoundPlayer.playAmbientSound("sounds/carTireScreech.ogg");
		mHasStartedToSlowDown = true;
	}
	if(car.getPosition().x > 4975)
		car.slowDown();
	if(mCutsceneTimeInSeconds > 19 && !mHasChangedTheMusicToMenuTheme) {
		mMusicPlayer.play("music/Menu.ogg");
		mHasChangedTheMusicToMenuTheme = true;
	}

	// PLAYER LEAVES CAR
	if(mCutsceneTimeInSeconds > 23 && !mWasPlayerCreated) {
		auto playerNpc = std::make_unique<Npc>(mGameData, "playerNpc");
		playerNpc->setPosition({5650, 380});
		mRoot.addChild(std::move(playerNpc));
		mWasPlayerCreated = true;
	}

	// CAMERA VIEW BECOMES SMALLER
	if(mCutsceneTimeInSeconds > 23.7 && !mHasPlayerTurnedToNpc) {
		mCamera.setSize({320, 240});
		mCamera.setCenter({5745, 395});
		auto& playerNpc = dynamic_cast<Character&>(*mRoot.getChild("playerNpc"));
		playerNpc.setAnimationState("right");
		mHasPlayerTurnedToNpc = true;
	}

	// PLAYER TALKS TO CRAWLING NPC, CRAWLING NPC DIES
	auto speechBubble = canvas->getWidget("speechBubble");

	if(mCutsceneTimeInSeconds > 25 && mCutsceneTimeInSeconds < 27) {
		speechBubble->show();
		speechBubble->getWidget("speech2")->hide();
		speechBubble->getWidget("speech3")->hide();
	}
	else if(mCutsceneTimeInSeconds > 27 && mCutsceneTimeInSeconds < 29) {
		speechBubble->getWidget("speech1")->hide();
		speechBubble->getWidget("speech1b")->hide();
		speechBubble->getWidget("speech2")->show();
	}
	if(mCutsceneTimeInSeconds > 27.5) {
		auto* crawlingNpc = dynamic_cast<CrawlingNpc*>(mRoot.getChild("LAYER_lyingObjects")->getChild("crawlingNpc"));
		crawlingNpc->die();
	}
	
	// BILBORD FALLS OVER
	if(mCutsceneTimeInSeconds > 28 && !mHasBilbordFallenOver) {
		auto* bilbord = dynamic_cast<Bilbord*>(mRoot.getChild("LAYER_standingObjects")->getChild("bilbord"));
		bilbord->fallOver();
		mHasBilbordFallenOver = true;
	}
	if(mCutsceneTimeInSeconds > 29) {
		auto& playerNpc = dynamic_cast<Character&>(*mRoot.getChild("playerNpc"));
		auto& animation = playerNpc.getAnimation();
		animation.changeState("left");
	}

	if(mCutsceneTimeInSeconds > 29.5 && !mHasChangedMusicToZombieAttackTheme)
		mMusicPlayer.play("music/zombieAttack.ogg");

	// SAY FUCK
	if(mCutsceneTimeInSeconds > 30 && mCutsceneTimeInSeconds < 33) {
		speechBubble->getWidget("speech2")->hide();
		speechBubble->getWidget("speech3")->show();
	}
	else if(mCutsceneTimeInSeconds > 33 && mCutsceneTimeInSeconds < 34)
		speechBubble->hide();

	// SPAWN ZOMBIES AND EXPAND CAMERA VIEW
	if(mCutsceneTimeInSeconds > 34 && !mWereZombieSpawned) {
		createZombie({5585, 305});
		createZombie({5560, 340});
		createZombie({5525, 380});
		createZombie({5530, 420});
		createZombie({5520, 420});
		createZombie({5580, 475});
		createZombie({5605, 500});
		mWereZombieSpawned = true;
		mCamera.setSize({640, 480});
	}

	if(mCutsceneTimeInSeconds > 36)
		closeCutScene();
}

void StartGameCutScene::createZombie(const sf::Vector2f position)
{
	auto zombie = std::make_unique<Npc>(mGameData);
	zombie->setPosition(position);
	zombie->getSprite().setTexture(mGameData->getTextures().get("textures/characters/zombieFullAnimation.png"));
	mRoot.addChild(std::move(zombie));
}

void StartGameCutScene::closeCutScene()
{
	mGameData->getSceneManager().replaceScene("scenes/gateAreaMap.xml");
	mIsActive = false;
}

}
