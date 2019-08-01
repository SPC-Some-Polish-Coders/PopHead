#include "game.hpp"

#include <SFML/System.hpp>
#include <Input/eventLoop.hpp>
#include "Resources/loadFonts.hpp"

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
	,mSceneMachine{new SceneManager()}
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
		mSceneMachine.get(),
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
	//Logger::getInstance().setGameData(gameData); // logger.setGameData() must be called after mTerminal.init()
	mEfficiencyRegister->init(gameData);
	mMap->setGameData(gameData);
	mGui->init(gameData);
	EventLoop::init(gameData);
	mInput->setGameData(gameData);
	mRenderer->setGameData(gameData);
	mSceneMachine->setGameData(gameData);
	mSceneMachine->replaceScene("scenes/desertScene.xml");
}

void Game::run()
{
	sf::Clock clock;
	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
	sf::Time deltaTime = sf::Time::Zero;

	while(mGameData->getGameCloser().shouldGameBeClosed() == false)
	{
		mSceneMachine->changingScenesProcess();
		input();

		deltaTime += clock.restart();

		while(deltaTime >= timePerFrame) {
			input();
			update(getProperDeltaTime(deltaTime));
			deltaTime = sf::Time::Zero;
			draw();
		}
	}

	mRenderWindow.close();
}

void Game::input()
{
	EventLoop::eventLoop(mGameData.get());
	mSceneMachine->input();
	mInput->getGlobalKeyboardShortcutes().handleShortcuts();
	mTerminal->input();
	mEfficiencyRegister->input();
}

sf::Time Game::getProperDeltaTime(sf::Time deltaTime)
{
	const sf::Time minimalDeltaTimeConstrain = sf::seconds(1.f/20.f);
	return deltaTime > minimalDeltaTimeConstrain ? minimalDeltaTimeConstrain : deltaTime;
}

void Game::update(sf::Time deltaTime)
{
	mAIManager->update();
	mSceneMachine->update(deltaTime);
	mPhysicsEngine->update(deltaTime);
	mRenderer->update(deltaTime);
	mGui->update(deltaTime);
	mEfficiencyRegister->update();
}

void Game::draw()
{
	mRenderer->draw();
	mRenderWindow.display();
}

}