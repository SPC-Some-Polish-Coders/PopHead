#include "gateGuardDialogue.hpp"
#include "gameData.hpp"
#include "Events/actionEventManager.hpp"

namespace ph {

GateGuardDialogue::GateGuardDialogue(GameData* const gameData)
	:mGameData(gameData)
	,mPlayerOnThePosition(false)
	,mTimesPressedSkip(1)
{
	//mPlayer = dynamic_cast<Player*>(mGameData->getSceneManager().getScene().getRoot().getChild("LAYER_standingObjects")->getChild("player"));
	ActionEventManager::setEnabled(false);
	mGameData->getGui().hideInterface("gameplayCounters");
	/*mGameData->getRenderer().getCamera().setSize({320, 240});*/
	initGui();
}

void GateGuardDialogue::initGui()
{
	mGameData->getGui().showInterface("labels");
	auto playerSpeechBubble = mGameData->getGui().getInterface("labels")->getWidget("canvas")->getWidget("playerSpeechBubble");
	auto guardSpeechBubble = mGameData->getGui().getInterface("labels")->getWidget("canvas")->getWidget("guardSpeechBubble");
	auto hint = mGameData->getGui().getInterface("labels")->getWidget("canvas")->getWidget("hint");
	
	playerSpeechBubble->hide();
	guardSpeechBubble->hide();
	hint->hide();

	guardSpeechBubble->getWidget("characterName")->show();
	playerSpeechBubble->getWidget("player1")->hide();
	playerSpeechBubble->getWidget("player2")->hide();

	guardSpeechBubble->getWidget("characterName")->show();
	guardSpeechBubble->getWidget("guard1")->hide();
	guardSpeechBubble->getWidget("guard1b")->hide();
	guardSpeechBubble->getWidget("guard1c")->hide();
	guardSpeechBubble->getWidget("guard2")->hide();
	guardSpeechBubble->getWidget("guard2b")->hide();
	guardSpeechBubble->getWidget("guard2c")->hide();
	guardSpeechBubble->getWidget("guard3")->hide();
	guardSpeechBubble->getWidget("guard3b")->hide();
	guardSpeechBubble->getWidget("guard3c")->hide();
	guardSpeechBubble->getWidget("guard4")->hide();
	guardSpeechBubble->getWidget("guard4b")->hide();
	guardSpeechBubble->getWidget("guard5")->hide();
	guardSpeechBubble->getWidget("guard5b")->hide();
	guardSpeechBubble->getWidget("guard5c")->hide();
	guardSpeechBubble->getWidget("guard5c")->hide();
	guardSpeechBubble->getWidget("guard6")->hide();
	guardSpeechBubble->getWidget("guard6b")->hide();
	guardSpeechBubble->getWidget("guard6c")->hide();
	guardSpeechBubble->getWidget("guard7")->hide();
}
	
void GateGuardDialogue::update(const sf::Time dt)
{
	if (!mPlayerOnThePosition)
	{
		/*if (mPlayer->getPosition().y > 600.f)
			mPlayer->move(sf::Vector2f(0.f, -20.f * delta.asSeconds()));
		else
			mPlayerOnThePosition = true;*/
	}

	// TODO: Use events here
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mTimeSinceLastSkipPress.getElapsedTime().asSeconds() > 0.3f)
	{
		++mTimesPressedSkip;
		mTimeSinceLastSkipPress.restart();
	}

	auto guardSpeechBubble = mGameData->getGui().getInterface("labels")->getWidget("canvas")->getWidget("guardSpeechBubble");
	auto playerSpeechBubble = mGameData->getGui().getInterface("labels")->getWidget("canvas")->getWidget("playerSpeechBubble");
	auto hint = mGameData->getGui().getInterface("labels")->getWidget("canvas")->getWidget("hint");

	if(mPlayerOnThePosition) 
	{
		switch(mTimesPressedSkip)
		{
		case 1:
		{
			hint->show();
			playerSpeechBubble->show();
			playerSpeechBubble->getWidget("player1")->show();
			playerSpeechBubble->getWidget("player1b")->show();
		} break;

		case 2:
		{
			playerSpeechBubble->hide();
			playerSpeechBubble->getWidget("player1")->hide();
			playerSpeechBubble->getWidget("player1b")->hide();

			guardSpeechBubble->show();
			guardSpeechBubble->getWidget("guard1")->show();
			guardSpeechBubble->getWidget("guard1b")->show();
			guardSpeechBubble->getWidget("guard1c")->show();
		} break;

		case 3:
		{
			guardSpeechBubble->hide();
			guardSpeechBubble->getWidget("guard1")->hide();
			guardSpeechBubble->getWidget("guard1b")->hide();
			guardSpeechBubble->getWidget("guard1c")->hide();

			playerSpeechBubble->show();
			playerSpeechBubble->getWidget("player2")->show();
		} break;

		case 4:
		{
			playerSpeechBubble->hide();
			playerSpeechBubble->getWidget("player2")->hide();

			guardSpeechBubble->show();
			guardSpeechBubble->getWidget("guard2")->show();
			guardSpeechBubble->getWidget("guard2b")->show();
			guardSpeechBubble->getWidget("guard2c")->show();
		} break;

		case 5:
		{
			guardSpeechBubble->getWidget("guard2")->hide();
			guardSpeechBubble->getWidget("guard2b")->hide();
			guardSpeechBubble->getWidget("guard2c")->hide();

			guardSpeechBubble->getWidget("guard3")->show();
			guardSpeechBubble->getWidget("guard3b")->show();
			guardSpeechBubble->getWidget("guard3c")->show();
		} break;

		case 6:
		{
			guardSpeechBubble->getWidget("guard3")->hide();
			guardSpeechBubble->getWidget("guard3b")->hide();
			guardSpeechBubble->getWidget("guard3c")->hide();

			guardSpeechBubble->getWidget("guard4")->show();
			guardSpeechBubble->getWidget("guard4b")->show();
		} break;

		case 7:
		{
			guardSpeechBubble->getWidget("guard4")->hide();
			guardSpeechBubble->getWidget("guard4b")->hide();

			guardSpeechBubble->getWidget("guard5")->show();
			guardSpeechBubble->getWidget("guard5b")->show();
			guardSpeechBubble->getWidget("guard5c")->show();
		} break;

		case 8:
		{
			guardSpeechBubble->getWidget("guard5")->hide();
			guardSpeechBubble->getWidget("guard5b")->hide();
			guardSpeechBubble->getWidget("guard5c")->hide();

			guardSpeechBubble->getWidget("guard6")->show();
			guardSpeechBubble->getWidget("guard6b")->show();
			guardSpeechBubble->getWidget("guard6c")->show();
		} break;

		case 9:
		{
			guardSpeechBubble->getWidget("guard6")->hide();
			guardSpeechBubble->getWidget("guard6b")->hide();
			guardSpeechBubble->getWidget("guard6c")->hide();

			guardSpeechBubble->getWidget("guard7")->show();
		} break;

		default:
			guardSpeechBubble->getWidget("guard7")->hide();
			guardSpeechBubble->hide();
			playerSpeechBubble->hide();
			hint->hide();
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
	/*mGameData->getRenderer().getCamera().setSize({640, 480});*/
}

}
