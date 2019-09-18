#include "endingCutscene.hpp"
#include "Gui/gui.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Renderer/camera.hpp"
#include "Events/actionEventManager.hpp"
#include "AI/aiManager.hpp"
#include "Scenes/sceneManager.hpp"

namespace ph {

EndingCutScene::EndingCutScene(GameObject& root, GUI& gui, MusicPlayer& musicPlayer,
                               Camera& camera, AIManager& aiManager, SceneManager& sceneManager)
	:CutScene(root)
	,mGui(gui)
	,mMusicPlayer(musicPlayer)
	,mCamera(camera)
	,mSceneManager(sceneManager)
	,mTimesPressedSkip(1.f)
{
	aiManager.setAIMode(AIMode::zombieAlwaysWalkRandomly);
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

	playerSpeechBubble->hide();
	guySpeechBubble->hide();

	guySpeechBubble->getWidget("characterName")->show();
	playerSpeechBubble->getWidget("player1")->hide();
	playerSpeechBubble->getWidget("player2")->hide();

	guySpeechBubble->getWidget("characterName")->show();
	guySpeechBubble->getWidget("guy1")->hide();
	guySpeechBubble->getWidget("guy1b")->hide();
	guySpeechBubble->getWidget("guy2")->hide();
	guySpeechBubble->getWidget("guy3")->hide();
	guySpeechBubble->getWidget("guy4")->hide();
	guySpeechBubble->getWidget("guy5")->hide();
	guySpeechBubble->getWidget("guy5b")->hide();
	guySpeechBubble->getWidget("guy6")->hide();
}

void EndingCutScene::update(const sf::Time delta)
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

	switch(mTimesPressedSkip)
	{
		case 1:
		{
			guySpeechBubble->show();
			guySpeechBubble->getWidget("guy1")->show();
			guySpeechBubble->getWidget("guy1b")->show();
			break;
		}
		case 2:
		{
			guySpeechBubble->getWidget("guy1")->hide();
			guySpeechBubble->getWidget("guy1b")->hide();
			guySpeechBubble->hide();

			playerSpeechBubble->show();
			playerSpeechBubble->getWidget("player1")->show();
			break;
		}
		case 3:
		{
			playerSpeechBubble->hide();
			playerSpeechBubble->getWidget("player1")->hide();

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
			break;
		}
		case 5:
		{
			playerSpeechBubble->hide();
			playerSpeechBubble->getWidget("player2")->hide();

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
			break;
		}
		default:
			leaveCutScene();
			break;
	}
}

void EndingCutScene::leaveCutScene()
{
	ActionEventManager::setEnabled(true);
	mIsActive = false;
	mSceneManager.replaceScene("scenes/mainMenu.xml");
}

}
