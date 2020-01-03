#include "startGameCutscene.hpp"
#include "Renderer/API/camera.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "GUI/gui.hpp"
#include "Scenes/sceneManager.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"

namespace ph {

StartGameCutScene::StartGameCutScene(entt::registry& registry, SoundPlayer& soundPlayer, MusicPlayer& musicPlayer, GUI& gui, SceneManager& sceneManager)
	:mRegistry(registry)
	,mSoundPlayer(soundPlayer)
	,mMusicPlayer(musicPlayer)
	,mGui(gui)
	,mSceneManager(sceneManager)
	,mCutsceneTimeInSeconds(0.f)
	,mWasGuiHidden(false)
	,mHasStartedToSlowDown(false)
	,mWasPlayerCreated(false)
{
}

void StartGameCutScene::update(const sf::Time dt)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		closeCutScene();
		return;
	}

	mCutsceneTimeInSeconds += dt.asSeconds();
	
	// MOVE CAMERA
	sf::Vector2f carCenterPos;
	auto cars = mRegistry.view<component::Car, component::BodyRect>();
	cars.each([&carCenterPos](const component::Car, const component::BodyRect& body) {
		carCenterPos = body.rect.getCenter();
	});
	auto cameras = mRegistry.view<component::Camera>();
	cameras.each([carCenterPos](component::Camera& camera) {
		camera.camera.setCenter(carCenterPos);
	});
	

	auto canvas = mGui.getInterface("labels")->getWidget("canvas");

	// HIDE ALL GUI
	if(!mWasGuiHidden) {
		canvas->getWidget("place")->hide();
		canvas->getWidget("time")->hide();
		canvas->getWidget("velocity")->hide();
		canvas->getWidget("speechBubble")->hide();
		mWasGuiHidden = true;
	}

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
		cars.each([&velocityWidget](const component::Car& car, const component::BodyRect&) {
			int velocity = static_cast<int>(car.velocity / 4.7);
			velocityWidget->setString(std::to_string(velocity) + " MPH");
		});
	}
	else if(mCutsceneTimeInSeconds > 22 && mCutsceneTimeInSeconds < 24.5) {
		canvas->getWidget("velocity")->hide();
	}

	// CAR SLOWS DOWN
	cars.each([this](component::Car& car, const component::BodyRect& body) {
		if(body.rect.left > 5100.f && !mHasStartedToSlowDown) {
			mSoundPlayer.playAmbientSound("sounds/carTireScreech.ogg");
			mHasStartedToSlowDown = true;
		}
		if(body.rect.left > 5110.f) {
			car.shouldSpeedUp = false;
			car.shouldSlowDown = true;
		}
	});

	// AGENT LEAVES CAR
	if(mCutsceneTimeInSeconds > 23 && !mWasPlayerCreated) {
	/*	auto playerNpc = std::make_unique<Npc>(mGameData, "playerNpc");
		playerNpc->setAnimationState("stayingRight");
		playerNpc->setPosition({5760, 392});
		mRoot.addChild(std::move(playerNpc));*/
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
	mSceneManager.replaceScene("scenes/gateAreaMap.xml");
	mIsActive = false;
}

}
