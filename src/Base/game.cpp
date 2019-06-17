#include "Base/game.hpp"

#include "States/stateIdentifiers.hpp"
#include <SFML/System.hpp>
#include <Input/eventLoop.hpp>
#include "Logs/logger.hpp"

namespace ph { enum class StateID; }

ph::Game::Game()
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
		mGui.get()
	));

	mTerminal->init(mGameData.get());

	//logger.setGameData() has to be called after mTerminal.init() - this comment should be replaced by proper unit test
	Logger::getInstance().setGameData(mGameData.get());

	mStateMachine->setGameData(mGameData.get());
	mStateMachine->pushState(StateID::GameState);

	mGui->init(mGameData.get());

	EventLoop::init(mGameData.get());
	mInput->setGameData(mGameData.get());

	mRenderer->setGameData(mGameData.get());
}

void ph::Game::run()
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
			draw();
		}
	}
}

void ph::Game::input()
{
	EventLoop::eventLoop(mGameData.get());
	mStateMachine->input();
	mTerminal->input();
}

void ph::Game::update(sf::Time delta)
{
	mStateMachine->update(delta);
	mPhysicsEngine->update(delta);
	mRenderer->update(delta);
	mGui->update(delta);
}

void ph::Game::draw()
{
	mRenderer->draw();
}
