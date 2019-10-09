#include <GL/glew.h>
#include "game.hpp"
#include "Resources/loadFonts.hpp"
#include "Events/globalKeyboardShortcuts.hpp"
#include "Events/eventDispatcher.hpp"
#include "Events/actionEventManager.hpp"
#include "Logs/logs.hpp"
#include "Renderer/renderer.hpp"
#include <SFML/System.hpp>

namespace ph {

Game::Game()
	:mGameData{}
	,mWindow(sf::VideoMode::getDesktopMode(), "PopHead", sf::Style::Default,
		sf::ContextSettings(24, 8, 0, 3, 3, sf::ContextSettings::Core)
	)
	,mSoundPlayer{new SoundPlayer()}
	,mMusicPlayer{new MusicPlayer()}
	,mTextures{new TextureHolder()}
	,mFonts{new FontHolder()}
	,mShaders{new ShaderHolder()}
	,mAIManager(new AIManager())
	,mSceneManager{new SceneManager()}
	,mMap(new Map())
	,mPhysicsEngine{new PhysicsEngine()}
	,mTerminal{new Terminal()}
	,mEfficiencyRegister{new EfficiencyRegister()}
	//,mGui{new GUI()}
{
	mGameData.reset(new GameData(
		&mWindow,
		mSoundPlayer.get(),
		mMusicPlayer.get(),
		mTextures.get(),
		mFonts.get(),
		mShaders.get(),
		mAIManager.get(),
		mSceneManager.get(),
		mMap.get(),
		mPhysicsEngine.get(),
		mTerminal.get(),
		mEfficiencyRegister.get(),
		nullptr//mGui.get()
	));

	GameData* gameData = mGameData.get();

	loadFonts(gameData);
	mTerminal->init(gameData);
	mEfficiencyRegister->init(gameData);
	mMap->setGameData(gameData);
	//mGui->init(gameData);
	mSceneManager->setGameData(gameData);
	mSceneManager->replaceScene("scenes/rendererTest.xml");

	mWindow.setVerticalSyncEnabled(true);

	ActionEventManager::init();

	Renderer::init();
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
		handleGlobalKeyboardShortcuts(mGameData->getWindow(), mGameData->getGameCloser(), phEvent);
		mEfficiencyRegister->handleEvent(phEvent);
		mTerminal->handleEvent(phEvent);
		//mGui->handleEvent(phEvent);
		
		if(!mTerminal->getSharedData()->mIsVisible)
			mSceneManager->handleEvent(phEvent);

		if(auto* sfEvent = std::get_if<sf::Event>(&phEvent))
			if(sfEvent->type == sf::Event::Resized)
				Renderer::onWindowResize(sfEvent->size.width, sfEvent->size.height);
	}
}

void Game::update(sf::Time deltaTime)
{
	//mEfficiencyRegister->update();

	if(mWindow.hasFocus())
	{
		mSceneManager->update(deltaTime);
		/*mAIManager->update();
		mPhysicsEngine->update(deltaTime);
		mGui->update(deltaTime);
		mTerminal->update();*/
	}

	static Camera camera;
	
	// Actual game loop
	Renderer::setClearColor({10, 10, 10, 255});

	if((sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)))
		camera.rotate(-1.f);
	if((sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)))
		camera.rotate(1.f);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		camera.move({-2.f, 0.f});
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		camera.move({2.f, 0.f});
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		camera.move({0.f, -2.f});
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		camera.move({0.f, 2.f});

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		camera.zoom(1.04f);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		camera.zoom(0.96f);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
		camera.setSize({640, 360});

	Renderer::beginScene(camera);
	mSceneManager->getScene().getRoot().draw(sf::Transform::Identity);
	Renderer::endScene();
	
	mWindow.display();
}

}
