#include "Base/game.hpp"

#include "States/stateIdentifiers.hpp"
#include <SFML/System.hpp>
#include <Input/eventLoop.hpp>

namespace PopHead { namespace States { enum class StateID; } }

using PopHead::Base::Game;


Game::Game()
	: mGameData{}
	, mSoundPlayer{new Audio::SoundPlayer()}
	, mMusicPlayer{new Audio::MusicPlayer()}
	, mTextures{new Resources::TextureHolder()}
	, mFonts{new Resources::FontHolder()}
	, mShaders{new Resources::ShaderHolder()}
	, mStateMachine{new States::StateMachine()}
	, mInput{new Input::Input()}
	, mRenderer{new Renderer::Renderer()}
	, mPhysicsEngine{new Physics::PhysicsEngine()}
	, mTerminal{new Terminal::Terminal()}
	, mGui{new GUI::GUI()}
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


	mStateMachine->setGameData(mGameData.get());
	mStateMachine->pushState(States::StateID::GameState);

	Input::EventLoop::init(mGameData.get());
	mInput->setGameData(mGameData.get());

	mTerminal->init(mGameData.get());

	mRenderer->setGameData(mGameData.get());
	
	mGui->init(mGameData.get());
}

void Game::run()
{
	sf::Clock clock;
	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while(mRenderer->getWindow().isOpen())
	{
		mStateMachine->changingStatesProcess();

		// temporary
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

void Game::input()
{
	Input::EventLoop::eventLoop(mGameData.get());
	mStateMachine->input();
	mTerminal->input();
}

void Game::update(sf::Time delta)
{
	mStateMachine->update(delta);
	mPhysicsEngine->update(delta);
	mRenderer->update(delta);
	mGui->update(delta);
}

void Game::draw()
{
	mRenderer->draw();
}
