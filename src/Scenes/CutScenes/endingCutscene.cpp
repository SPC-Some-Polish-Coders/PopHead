#include "endingCutscene.hpp"
#include "GUI/gui.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Renderer/API/camera.hpp"
#include "Events/actionEventManager.hpp"
#include "AI/aiManager.hpp"
#include "Scenes/sceneManager.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph {

EndingCutScene::EndingCutScene(entt::registry& registry, GUI& gui, MusicPlayer& musicPlayer, AIManager& aiManager, SceneManager& sceneManager)
	:CutScene(false)
	,mRegistry(registry)
	,mGui(gui)
	,mMusicPlayer(musicPlayer)
	,mAIManager(aiManager)
	,mSceneManager(sceneManager)
	,mTimesPressedSkip(1)
{
	aiManager.setAIMode(AIMode::zombieAlwaysWalkRandomly);
	auto cameras = mRegistry.view<component::Camera>();
	cameras.each([](component::Camera& camera) {
		camera.camera.setSize({320, 180});
	});
	ActionEventManager::setEnabled(false);
	gui.hideInterface("gameplayCounters");
	initGui();
}

void EndingCutScene::initGui()
{
	mGui.showInterface("labels");
	auto* canvas = mGui.getInterface("labels")->getWidget("canvas");
	auto playerSpeechBubble = canvas->getWidget("playerSpeechBubble");
	auto guySpeechBubble = canvas->getWidget("guardSpeechBubble");
	auto hint = canvas->getWidget("hint");

	playerSpeechBubble->hide();
	guySpeechBubble->hide();
	hint->hide();

	guySpeechBubble->getWidget("characterName")->show();
	playerSpeechBubble->getWidget("player1")->hide();
	playerSpeechBubble->getWidget("player1b")->hide();
	playerSpeechBubble->getWidget("player2")->hide();
	playerSpeechBubble->getWidget("player2b")->hide();

	guySpeechBubble->getWidget("characterName")->show();
	guySpeechBubble->getWidget("guy1")->hide();
	guySpeechBubble->getWidget("guy1b")->hide();
	guySpeechBubble->getWidget("guy1c")->hide();
	guySpeechBubble->getWidget("guy2")->hide();
	guySpeechBubble->getWidget("guy3")->hide();
	guySpeechBubble->getWidget("guy4")->hide();
	guySpeechBubble->getWidget("guy5")->hide();
	guySpeechBubble->getWidget("guy5b")->hide();
	guySpeechBubble->getWidget("guy6")->hide();
	guySpeechBubble->getWidget("guy6b")->hide();
}

void EndingCutScene::update(const sf::Time dt)
{
	// TODO: Use events here

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mTimeSinceLastSkipPress.getElapsedTime().asSeconds() > 0.3f)
	{
		++mTimesPressedSkip;
		mTimeSinceLastSkipPress.restart();
	}

	auto* canvas = mGui.getInterface("labels")->getWidget("canvas");
	auto playerSpeechBubble = canvas->getWidget("playerSpeechBubble");
	auto guySpeechBubble = canvas->getWidget("guardSpeechBubble");
	auto hint = canvas->getWidget("hint");

	switch(mTimesPressedSkip)
	{
		case 1:
		{
			guySpeechBubble->show();
			hint->show();
			guySpeechBubble->getWidget("guy1")->show();
			guySpeechBubble->getWidget("guy1b")->show();
			guySpeechBubble->getWidget("guy1c")->show();
			break;
		}
		case 2:
		{
			guySpeechBubble->getWidget("guy1")->hide();
			guySpeechBubble->getWidget("guy1b")->hide();
			guySpeechBubble->getWidget("guy1c")->hide();
			guySpeechBubble->hide();

			playerSpeechBubble->show();
			playerSpeechBubble->getWidget("player1")->show();
			playerSpeechBubble->getWidget("player1b")->show();
			break;
		}
		case 3:
		{
			playerSpeechBubble->hide();
			playerSpeechBubble->getWidget("player1")->hide();
			playerSpeechBubble->getWidget("player1b")->hide();

			guySpeechBubble->show();
			guySpeechBubble->getWidget("guy2")->show();
			break;
		}
		case 4:
		{
			guySpeechBubble->hide();
			guySpeechBubble->getWidget("guy2")->hide();

			playerSpeechBubble->show();
			playerSpeechBubble->getWidget("player2")->show();
			playerSpeechBubble->getWidget("player2b")->show();
			break;
		}
		case 5:
		{
			playerSpeechBubble->hide();
			playerSpeechBubble->getWidget("player2")->hide();
			playerSpeechBubble->getWidget("player2b")->hide();

			guySpeechBubble->show();
			guySpeechBubble->getWidget("guy3")->show();
			break;
		}
		case 6:
		{
			guySpeechBubble->getWidget("guy3")->hide();
			guySpeechBubble->getWidget("guy4")->show();
			break;
		}
		case 7:
		{
			guySpeechBubble->getWidget("guy4")->hide();
			guySpeechBubble->getWidget("guy5")->show();
			guySpeechBubble->getWidget("guy5b")->show();
			break;
		}
		case 8:
		{
			guySpeechBubble->getWidget("guy5")->hide();
			guySpeechBubble->getWidget("guy5b")->hide();
			guySpeechBubble->getWidget("guy6")->show();
			guySpeechBubble->getWidget("guy6b")->show();
			hint->hide();
			break;
		}
		default:
			leaveCutScene();
			break;
	}
}

void EndingCutScene::leaveCutScene()
{
	mAIManager.setAIMode(AIMode::normal);
	ActionEventManager::setEnabled(true);
	mIsActive = false;
	mSceneManager.replaceScene("scenes/credits.xml");
}

}
