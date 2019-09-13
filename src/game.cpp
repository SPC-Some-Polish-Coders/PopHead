#include "game.hpp"
#include "Resources/loadFonts.hpp"
#include <SFML/System.hpp>
#include <Input/eventLoop.hpp>

namespace ph {

Game::Game()
	:mGameData{}
	,mRenderWindow(sf::VideoMode::getDesktopMode(), "PopHead")
	,mSoundPlayer{new SoundPlayer()}
	,mMusicPlayer{new MusicPlayer()}
	,mTextures{new TextureHolder()}
	,mFonts{new FontHolder()}
	,mShaders{new ShaderHolder()}
	,mAIManager(new AIManager())
	,mSceneManager{new SceneManager()}
	,mMap(new Map())
	,mInput{new Input()}
	,mRenderer{new Renderer(mRenderWindow)}
	,mPhysicsEngine{new PhysicsEngine()}
	,mTerminal{new Terminal()}
	,mEfficiencyRegister{new EfficiencyRegister()}
	,mGui{new GUI()}
{
	mGameData.reset(new GameData(
		&mRenderWindow,
		mSoundPlayer.get(),
		mMusicPlayer.get(),
		mTextures.get(),
		mFonts.get(),
		mShaders.get(),
		mAIManager.get(),
		mSceneManager.get(),
		mMap.get(),
		mInput.get(),
		mRenderer.get(),
		mPhysicsEngine.get(),
		mTerminal.get(),
		mEfficiencyRegister.get(),
		mGui.get()
	));

	GameData* gameData = mGameData.get();

	loadFonts(gameData);
	mTerminal->init(gameData);
	mEfficiencyRegister->init(gameData);
	mMap->setGameData(gameData);
	mGui->init(gameData);
	mInput->setGameData(gameData);
	mSceneManager->setGameData(gameData);
	mSceneManager->replaceScene("scenes/mainMenu.xml");

	mRenderWindow.setVerticalSyncEnabled(true);
}

void Game::run()
{
	sf::Clock clock;
	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
	sf::Time deltaTime = sf::Time::Zero;

	while(mGameData->getGameCloser().shouldGameBeClosed() == false)
	{
		mSceneManager->changingScenesProcess();

		deltaTime += clock.restart();

		while(deltaTime >= timePerFrame) {
			handleEvents();
			update(getProperDeltaTime(deltaTime));
			deltaTime = sf::Time::Zero;
			draw();
		}
	}

	mRenderWindow.close();
}

sf::Time Game::getProperDeltaTime(sf::Time deltaTime)
{
	const sf::Time minimalDeltaTimeConstrain = sf::seconds(1.f/20.f);
	return deltaTime > minimalDeltaTimeConstrain ? minimalDeltaTimeConstrain : deltaTime;
}

void Game::handleEvents()
{
	sf::Event e;
	while(mRenderWindow.pollEvent(e))
	{
		mInput->getGlobalKeyboardShortcuts().handleEvent(e);
		mEfficiencyRegister->handleEvent(e);
		mTerminal->handleEvent(e);
		mGui->handleEvent(e);
		
		if(!mTerminal->getSharedData()->mIsVisible)
			mSceneManager->handleEvent(e);
	}
}

void Game::update(sf::Time deltaTime)
{
	mEfficiencyRegister->update();

	if(mRenderWindow.hasFocus())
	{
		mAIManager->update();
		mSceneManager->update(deltaTime);
		mPhysicsEngine->update(deltaTime);
		mRenderer->update(deltaTime);
		mGui->update(deltaTime);
		mTerminal->update();
		mInput->getGlobalKeyboardShortcuts().update();
	}
}

void Game::draw()
{
	mRenderer->startSceneRendering();
	mRenderer->draw(*mMap);
	mRenderer->draw(mSceneManager->getScene().getRoot());
	mRenderer->draw(mPhysicsEngine->getCollisionDebugManager());

	mRenderer->startUIRendering();
	mRenderer->draw(mGui->getGuiDrawer());
	mRenderer->draw(mEfficiencyRegister->getDisplayer());
	mRenderer->draw(mTerminal->getImage());

	mRenderWindow.display();
}

}
