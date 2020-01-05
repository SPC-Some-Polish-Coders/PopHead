#include "subtitlesBeforeStartGameCutscene.hpp"
#include "Scenes/sceneManager.hpp"
#include "GUI/gui.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

SubtitlesBeforeStartGameCutscene::SubtitlesBeforeStartGameCutscene(SceneManager& sceneManager, GUI& gui)
	:CutScene(false)
	,mSceneManager(sceneManager)
	,mGui(gui)
	,mTimesPressedSkip(1)
{
	gui.showInterface("labels");
	auto* playerSpeechBubble = gui.getInterface("labels")->getWidget("canvas")->getWidget("playerSpeechBubble");
	auto* phoneSpeechBubble = gui.getInterface("labels")->getWidget("canvas")->getWidget("phoneSpeechBubble");

	playerSpeechBubble->show();
	phoneSpeechBubble->hide();

	playerSpeechBubble->getWidget("characterName")->show();
	playerSpeechBubble->getWidget("player1")->show();
	playerSpeechBubble->getWidget("player2")->hide();
	playerSpeechBubble->getWidget("player3")->hide();
	playerSpeechBubble->getWidget("player3b")->hide();

	phoneSpeechBubble->getWidget("characterName")->show();
	phoneSpeechBubble->getWidget("phone1")->hide();
	phoneSpeechBubble->getWidget("phone2")->hide();
	phoneSpeechBubble->getWidget("phone2b")->hide();
	phoneSpeechBubble->getWidget("phone2c")->hide();
	phoneSpeechBubble->getWidget("phone3")->hide();
	phoneSpeechBubble->getWidget("phone3b")->hide();
	phoneSpeechBubble->getWidget("phone3c")->hide();
	phoneSpeechBubble->getWidget("phone4")->hide();
	phoneSpeechBubble->getWidget("phone4b")->hide();
}

void SubtitlesBeforeStartGameCutscene::update(sf::Time dt)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		mIsActive = false;
		mSceneManager.replaceScene("scenes/startGameCutscene.xml");
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mClock.getElapsedTime().asSeconds() > 0.3f)
	{
		mClock.restart();

		auto* playerSpeechBubble = mGui.getInterface("labels")->getWidget("canvas")->getWidget("playerSpeechBubble");
		auto* phoneSpeechBubble = mGui.getInterface("labels")->getWidget("canvas")->getWidget("phoneSpeechBubble");
		switch(++mTimesPressedSkip)
		{
			case 2:
			{
				playerSpeechBubble->hide();
				playerSpeechBubble->getWidget("player1")->hide();

				phoneSpeechBubble->show();
				phoneSpeechBubble->getWidget("phone1")->show();
				break;
			}
			case 3:
			{
				phoneSpeechBubble->getWidget("phone1")->hide();
				
				phoneSpeechBubble->getWidget("phone2")->show();
				phoneSpeechBubble->getWidget("phone2b")->show();
				phoneSpeechBubble->getWidget("phone2c")->show();
				break;
			}
			case 4:
			{
				phoneSpeechBubble->getWidget("phone2")->hide();
				phoneSpeechBubble->getWidget("phone2b")->hide();
				phoneSpeechBubble->getWidget("phone2c")->hide();

				phoneSpeechBubble->getWidget("phone3")->show();
				phoneSpeechBubble->getWidget("phone3b")->show();
				phoneSpeechBubble->getWidget("phone3c")->show();
				break;
			}
			case 5:
			{
				phoneSpeechBubble->hide();
				phoneSpeechBubble->getWidget("phone3")->hide();
				phoneSpeechBubble->getWidget("phone3b")->hide();
				phoneSpeechBubble->getWidget("phone3c")->hide();

				playerSpeechBubble->show();
				playerSpeechBubble->getWidget("player2")->show();
				break;
			}
			case 6:
			{
				playerSpeechBubble->hide();
				playerSpeechBubble->getWidget("player2")->hide();

				phoneSpeechBubble->show();
				phoneSpeechBubble->getWidget("phone4")->show();
				phoneSpeechBubble->getWidget("phone4b")->show();
				break;
			}
			case 7:
			{
				phoneSpeechBubble->hide();
				phoneSpeechBubble->getWidget("phone4")->hide();
				phoneSpeechBubble->getWidget("phone4b")->hide();

				playerSpeechBubble->show();
				playerSpeechBubble->getWidget("player3")->show();
				playerSpeechBubble->getWidget("player3b")->show();
				break;
			}
			default:
			{
				playerSpeechBubble->hide();
				playerSpeechBubble->getWidget("player3")->hide();
				playerSpeechBubble->getWidget("player3b")->hide();
				mGui.getInterface("labels")->getWidget("canvas")->getWidget("hint")->hide();
				mIsActive = false;
				mSceneManager.replaceScene("scenes/startGameCutscene.xml");
				break;
			}
		}
	}
}

}

