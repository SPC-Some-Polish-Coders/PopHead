#include "gateGuardDialogue.hpp"
#include "gameData.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "Events/actionEventManager.hpp"

namespace ph {

GateGuardDialogue::GateGuardDialogue(GameData* const gameData)
	:CutScene(gameData->getSceneManager().getScene().getRoot())
	,mGameData(gameData)
	,mPlayerOnThePosition(false)
	,mTimesPressedSkip(1)
{
	mPlayer = dynamic_cast<Player*>(mGameData->getSceneManager().getScene().getRoot().getChild("LAYER_standingObjects")->getChild("player"));
	ActionEventManager::setEnabled(false);
	mGameData->getGui().hideInterface("gameplayCounters");
	mGameData->getRenderer().getCamera().setSize({320, 240});
	initGui();
}

void GateGuardDialogue::initGui()
{
	mGameData->getGui().showInterface("labels");
	auto playerSpeechBubble = mGameData->getGui().getInterface("labels")->getWidget("canvas")->getWidget("playerSpeechBubble");
	auto guardSpeechBubble = mGameData->getGui().getInterface("labels")->getWidget("canvas")->getWidget("guardSpeechBubble");
	
	playerSpeechBubble->hide();
	guardSpeechBubble->hide();

	guardSpeechBubble->getWidget("characterName")->show();
	playerSpeechBubble->getWidget("player1")->hide();
	playerSpeechBubble->getWidget("player2")->hide();
	playerSpeechBubble->getWidget("player3")->hide();
	playerSpeechBubble->getWidget("player3b")->hide();
	playerSpeechBubble->getWidget("player4")->hide();
	playerSpeechBubble->getWidget("player4b")->hide();
	playerSpeechBubble->getWidget("player4c")->hide();
	playerSpeechBubble->getWidget("player5")->hide();
	playerSpeechBubble->getWidget("player5b")->hide();
	playerSpeechBubble->getWidget("player5c")->hide();
	playerSpeechBubble->getWidget("player6")->hide();
	playerSpeechBubble->getWidget("player7")->hide();

	guardSpeechBubble->getWidget("characterName")->show();
	guardSpeechBubble->getWidget("guard1")->hide();
	guardSpeechBubble->getWidget("guard1b")->hide();
	guardSpeechBubble->getWidget("guard2")->hide();
	guardSpeechBubble->getWidget("guard2b")->hide();
	guardSpeechBubble->getWidget("guard2c")->hide();
	guardSpeechBubble->getWidget("guard3")->hide();
	guardSpeechBubble->getWidget("guard3b")->hide();
	guardSpeechBubble->getWidget("guard4")->hide();
	guardSpeechBubble->getWidget("guard4b")->hide();
	guardSpeechBubble->getWidget("guard4c")->hide();
	guardSpeechBubble->getWidget("guard5")->hide();
	guardSpeechBubble->getWidget("guard5b")->hide();
	guardSpeechBubble->getWidget("guard5c")->hide();
	guardSpeechBubble->getWidget("guard6")->hide();
	guardSpeechBubble->getWidget("guard6b")->hide();
	guardSpeechBubble->getWidget("guard6c")->hide();
	guardSpeechBubble->getWidget("guard7")->hide();
	guardSpeechBubble->getWidget("guard7b")->hide();
}
	
void GateGuardDialogue::update(const sf::Time delta)
{
	if (!mPlayerOnThePosition)
	{
		if (mPlayer->getPosition().y > 600.f)
			mPlayer->move(sf::Vector2f(0.f, -20.f * delta.asSeconds()));
		else
			mPlayerOnThePosition = true;
	}

	// TODO: Use events here
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mTimeSinceLastSkipPress.getElapsedTime().asSeconds() > 0.3f)
	{
		++mTimesPressedSkip;
		mTimeSinceLastSkipPress.restart();
	}

	auto guardSpeechBubble = mGameData->getGui().getInterface("labels")->getWidget("canvas")->getWidget("guardSpeechBubble");
	auto playerSpeechBubble = mGameData->getGui().getInterface("labels")->getWidget("canvas")->getWidget("playerSpeechBubble");

	if (mPlayerOnThePosition) 
	{
		switch(mTimesPressedSkip)
		{
		case 1:
		{
			guardSpeechBubble->show();
			guardSpeechBubble->getWidget("guard1")->show();
			guardSpeechBubble->getWidget("guard1b")->show();
		} break;

		case 2:
		{
			guardSpeechBubble->getWidget("guard1")->hide();
			guardSpeechBubble->getWidget("guard1b")->hide();
			guardSpeechBubble->hide();

			playerSpeechBubble->show();
			playerSpeechBubble->getWidget("player1")->show();
		} break;

		case 3:
		{
			playerSpeechBubble->getWidget("player1")->hide();
			playerSpeechBubble->getWidget("player2")->show();
		} break;

		case 4:
		{
			playerSpeechBubble->getWidget("player2")->hide();
			playerSpeechBubble->getWidget("player3")->show();
			playerSpeechBubble->getWidget("player3b")->show();
		} break;

		case 5:
		{
			playerSpeechBubble->getWidget("player3")->hide();
			playerSpeechBubble->getWidget("player3b")->hide();
			playerSpeechBubble->hide();

			guardSpeechBubble->show();
			guardSpeechBubble->getWidget("guard2")->show();
			guardSpeechBubble->getWidget("guard2b")->show();
			guardSpeechBubble->getWidget("guard2c")->show();
		} break;

		case 6:
		{
			guardSpeechBubble->getWidget("guard2")->hide();
			guardSpeechBubble->getWidget("guard2b")->hide();
			guardSpeechBubble->getWidget("guard2c")->hide();
			guardSpeechBubble->hide();

			playerSpeechBubble->show();
			playerSpeechBubble->getWidget("player4")->show();
			playerSpeechBubble->getWidget("player4b")->show();
			playerSpeechBubble->getWidget("player4c")->show();
		} break;

		case 7:
		{
			playerSpeechBubble->getWidget("player4")->hide();
			playerSpeechBubble->getWidget("player4b")->hide();
			playerSpeechBubble->getWidget("player4c")->hide();

			playerSpeechBubble->getWidget("player5")->show();
			playerSpeechBubble->getWidget("player5b")->show();
			playerSpeechBubble->getWidget("player5c")->show();
		} break;

		case 8:
		{
			playerSpeechBubble->getWidget("player5")->hide();
			playerSpeechBubble->getWidget("player5b")->hide();
			playerSpeechBubble->getWidget("player5c")->hide();
			playerSpeechBubble->hide();

			guardSpeechBubble->show();
			guardSpeechBubble->getWidget("guard3")->show();
			guardSpeechBubble->getWidget("guard3b")->show();
		} break;


		case 9:
		{
			guardSpeechBubble->getWidget("guard3")->hide();
			guardSpeechBubble->getWidget("guard3b")->hide();

			guardSpeechBubble->getWidget("guard4")->show();
			guardSpeechBubble->getWidget("guard4b")->show();
			guardSpeechBubble->getWidget("guard4c")->show();
		} break;


		case 10:
		{
			guardSpeechBubble->getWidget("guard4")->hide();
			guardSpeechBubble->getWidget("guard4b")->hide();
			guardSpeechBubble->getWidget("guard4c")->hide();

			guardSpeechBubble->getWidget("guard5")->show();
			guardSpeechBubble->getWidget("guard5b")->show();
			guardSpeechBubble->getWidget("guard5c")->show();
		} break;

		case 11:
		{
			guardSpeechBubble->hide();
			guardSpeechBubble->getWidget("guard5")->hide();
			guardSpeechBubble->getWidget("guard5b")->hide();
			guardSpeechBubble->getWidget("guard5c")->hide();

			playerSpeechBubble->show();
			playerSpeechBubble->getWidget("player6")->show();
		} break;

		case 12:
		{
			playerSpeechBubble->getWidget("player6")->hide();
			playerSpeechBubble->hide();
			 
			guardSpeechBubble->show();
			guardSpeechBubble->getWidget("guard6")->show();
			guardSpeechBubble->getWidget("guard6b")->show();
			guardSpeechBubble->getWidget("guard6c")->show();
		} break;

		case 13:
		{
			guardSpeechBubble->getWidget("guard6")->hide();
			guardSpeechBubble->getWidget("guard6b")->hide();
			guardSpeechBubble->getWidget("guard6c")->hide();

			guardSpeechBubble->getWidget("guard7")->show();
			guardSpeechBubble->getWidget("guard7b")->show();	
		} break;

		case 14:
		{
			guardSpeechBubble->getWidget("guard7")->hide();
			guardSpeechBubble->getWidget("guard7b")->hide();
			guardSpeechBubble->hide();

			playerSpeechBubble->show();
			playerSpeechBubble->getWidget("player7")->show();

		} break;

		case 15:
		{
			playerSpeechBubble->getWidget("player7")->hide();
			playerSpeechBubble->hide();
			leaveCutScene();
		} break;

		default:
			leaveCutScene();
			break;

		}
	}
}

void GateGuardDialogue::leaveCutScene()
{
	ActionEventManager::setEnabled(true);
	mIsActive = false;
	mGameData->getGui().showInterface("gameplayCounters");
	mGameData->getRenderer().getCamera().setSize({640, 480});
}

}