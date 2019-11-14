#include <GL/glew.h>
#include "game.hpp"
#include "Resources/loadFonts.hpp"
#include "Events/globalKeyboardShortcuts.hpp"
#include "Events/eventDispatcher.hpp"
#include "Events/actionEventManager.hpp"
#include "Logs/logs.hpp"
#include "Renderer/renderer.hpp"
#include <SFML/System.hpp>
#include <iostream>

namespace ph {

Game::Game()
	:mGameData{}
	,mWindow(sf::VideoMode::getDesktopMode(), "PopHead", sf::Style::Default,
		sf::ContextSettings(24, 8, 0, 3, 3/*, sf::ContextSettings::Core*/)
	)
	,mSoundPlayer{new SoundPlayer()}
	,mMusicPlayer{new MusicPlayer()}
	,mTextures{new TextureHolder()}
	,mFonts{new FontHolder()}
	,mShaders{new ShaderHolder()}
	//,mAIManager(new AIManager())
	,mSceneManager{new SceneManager()}
	//,mMap(new Map())
	//,mPhysicsEngine{new PhysicsEngine()}
	,mTerminal{new Terminal()}
	,mEfficiencyRegister{new EfficiencyRegister()}
	,mGui{new GUI()}
{
	mGameData.reset(new GameData(
		&mWindow,
		mSoundPlayer.get(),
		mMusicPlayer.get(),
		mTextures.get(),
		mFonts.get(),
		mShaders.get(),
		nullptr,
		mSceneManager.get(),
		nullptr,
		nullptr,
		mTerminal.get(),
		mEfficiencyRegister.get(),
		mGui.get()
	));

	GameData* gameData = mGameData.get();

	loadFonts(gameData);
	mTerminal->init(gameData);
	mEfficiencyRegister->init(gameData);
	//mMap->setGameData(gameData);
	mGui->init(gameData);
	mSceneManager->setGameData(gameData);
	mSceneManager->replaceScene("scenes/ecsTest.xml");

	mWindow.setVerticalSyncEnabled(true);

	ActionEventManager::init();

	Renderer::init(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
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
		}
	}

	Renderer::shutDown();
	mWindow.close();
}

sf::Time Game::getProperDeltaTime(sf::Time deltaTime)
{
	const sf::Time minimalDeltaTimeConstrain = sf::seconds(1.f/20.f);
	return deltaTime > minimalDeltaTimeConstrain ? minimalDeltaTimeConstrain : deltaTime;
}

void Game::handleEvents()
{
	ph::Event phEvent;
	while(EventDispatcher::dispatchEvent(phEvent, mWindow))
	{
		if (auto * event = std::get_if<sf::Event>(&phEvent))
			if (event->type == sf::Event::Closed)
				mGameData->getGameCloser().closeGame();

		handleGlobalKeyboardShortcuts(mGameData->getWindow(), mGameData->getGameCloser(), phEvent);
		mEfficiencyRegister->handleEvent(phEvent);
		mTerminal->handleEvent(phEvent);
		mGui->handleEvent(phEvent);
		
		if(!mTerminal->getSharedData()->mIsVisible)
			mSceneManager->handleEvent(phEvent);

		if(auto* sfEvent = std::get_if<sf::Event>(&phEvent))
			if(sfEvent->type == sf::Event::Resized)
				Renderer::onWindowResize(sfEvent->size.width, sfEvent->size.height);
	}
}

void Game::update(sf::Time deltaTime)
{
	mEfficiencyRegister->update();

	if(mWindow.hasFocus())
	{
		mSceneManager->update(deltaTime);
		//mAIManager->update();
		//mPhysicsEngine->update(deltaTime);
		mGui->update(deltaTime);
		mEfficiencyRegister->getDisplayer().draw();
		mTerminal->update();
		// TODO_ren: Move it to Terminal::update()
		mTerminal->getImage().draw(mWindow, sf::RenderStates::Default);

		static Texture wallTex("resources/textures/test/wall.jpg");
		static Texture gateTex("resources/textures/others/gate.png");
		static Texture bulletTex("resources/textures/others/bullet.png");
		static Texture awsdTex("resources/textures/others/awsd.png");
		static Texture backslashTex("resources/textures/others/backslashHint.png");
		static Texture barTex("resources/textures/others/bar.png");
		static Texture medkitTex("resources/textures/others/medkit.png");
		static Texture zombieHeadTex("resources/textures/others/zombieHead.png");

		/// test 1 of instancing
		/*static Texture* textures[] = {
			&wallTex, &wallTex, &wallTex, &gateTex, &bulletTex, &awsdTex, &backslashTex, &barTex, &medkitTex, &zombieHeadTex
		};

		for(int x = 0; x < 100.f; ++x)
			for(int y = 0; y < 10.f; ++y) {
				Texture* tex = textures[x / 10];
				Renderer::submitQuad(tex, nullptr, &sf::Color::Blue, sf::Vector2f(11.f * (float)x, 11.f * (float)y), sf::Vector2f(10.f, 10.f), 0.f);
		}*/
		
		/// test 2 of instancing
		Renderer::submitQuad(&wallTex, nullptr, nullptr, sf::Vector2f(70.f, 0.f), sf::Vector2f(0.f, 30.f), 0.f);
		Renderer::submitQuad(&gateTex, nullptr, nullptr, sf::Vector2f(70.f, 0.f), sf::Vector2f(50.f, 30.f), 0.f);
		Renderer::submitQuad(&bulletTex, nullptr, nullptr, sf::Vector2f(70.f, 0.f), sf::Vector2f(100.f, 30.f), 0.f);
		Renderer::submitQuad(&medkitTex, nullptr, &sf::Color::Blue, sf::Vector2f(70.f, 0.f), sf::Vector2f(150.f, 30.f), 0.f);
		Renderer::submitQuad(&zombieHeadTex, nullptr, &sf::Color::Red, sf::Vector2f(70.f, 0.f), sf::Vector2f(200.f, 30.f), 0.f);
		Renderer::submitQuad(nullptr, nullptr, &sf::Color::Green, sf::Vector2f(-92.f, -45.f), sf::Vector2f(250.f, 50.f), 30.f);

		Renderer::endScene(mWindow, *mEfficiencyRegister);
		mWindow.display();
	}
}

}
