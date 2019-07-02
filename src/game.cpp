#include "game.hpp"

#include "States/stateIdentifiers.hpp"
#include <SFML/System.hpp>
#include <Input/eventLoop.hpp>
#include "Resources/loadFonts.hpp"

namespace ph {

enum class StateID; 

Game::Game()
	:mGameData{}
	,mSoundPlayer{new SoundPlayer()}
	,mMusicPlayer{new MusicPlayer()}
	,mTextures{new TextureHolder()}
	,mFonts{new FontHolder()}
	,mShaders{new ShaderHolder()}
	,mStateMachine{new StateMachine()}
	,mInput{new Input()}
	,mRenderer{new Renderer()}
	,mPhysicsEngine{new PhysicsEngine()}
	,mTerminal{new Terminal()}
	,mEfficencyRegister{new EfficencyRegister()}
	,mGui{new GUI()}
{
	mGameData.reset(new GameData(
		mSoundPlayer.get(),
		mMusicPlayer.get(),
		mTextures.get(),
		mFonts.get(),
		mShaders.get(),
		mStateMachine.get(),
		mInput.get(),
		mRenderer.get(),
		mPhysicsEngine.get(),
		mTerminal.get(),
		mEfficencyRegister.get(),
		mGui.get()
	));

	loadFonts(mGameData.get());

	mTerminal->init(mGameData.get());

	mEfficencyRegister->init(mGameData.get());

	mStateMachine->setGameData(mGameData.get());
	mStateMachine->pushState(StateID::GameState);

	mGui->init(mGameData.get());

	EventLoop::init(mGameData.get());
	mInput->setGameData(mGameData.get());

	mRenderer->setGameData(mGameData.get());
}

void Game::run()
{
	sf::Clock clock;
	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while(mRenderer->getWindow().isOpen())
	{
		mStateMachine->changingStatesProcess();

		// temporary - TODO: move this somewhere else
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			break;

		input();

		timeSinceLastUpdate += clock.restart();

		while(timeSinceLastUpdate >= timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;

			update(timePerFrame);
			mRenderer->draw();
		}
	}
}

void Game::input()
{
	EventLoop::eventLoop(mGameData.get());
	mStateMachine->input();
	mTerminal->input();
	mEfficencyRegister->input();
}

void Game::update(sf::Time delta)
{
	mStateMachine->update(delta);
	mPhysicsEngine->update(delta);
	mRenderer->update(delta);
	mGui->update(delta);
	mEfficencyRegister->update();
}

}