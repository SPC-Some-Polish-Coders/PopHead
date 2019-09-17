#include "gateGuardDialogue.hpp"
#include "gameData.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"

namespace ph {

GateGuardDialogue::GateGuardDialogue(GameData* const gameData)
	:CutScene(gameData->getSceneManager().getScene().getRoot())
	,mGameData(gameData)
{
	mPlayer = dynamic_cast<Player*>(mGameData->getSceneManager().getScene().getRoot().getChild("LAYER_standingObjects")->getChild("player"));
	mGameData->getGui().hideInterface("gameplayCounters");
	mViewBeforeCutScene = mGameData->getRenderer().getCamera().getSize();
	mGameData->getRenderer().getCamera().setSize(mViewBeforeCutScene - sf::Vector2f{200.f, 200.f});

}

void GateGuardDialogue::update(const sf::Time delta)
{
	mTimeSinceStart += delta;

	if (mPlayer->getPosition().y > 286.f)
		mPlayer->move(sf::Vector2f(0.f, -0.3f));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		leaveCutScene();
}

void GateGuardDialogue::leaveCutScene()
{
	mIsActive = false;
	mGameData->getGui().showInterface("gameplayCounters");
	mGameData->getRenderer().getCamera().setSize(mViewBeforeCutScene);
}

}