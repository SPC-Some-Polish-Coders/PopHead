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
	:mWindow(sf::VideoMode::getDesktopMode(), "PopHead", sf::Style::Default, sf::ContextSettings(24, 8, 0, 3, 3))
	,mGameData()
	,mSoundPlayer(std::make_unique<SoundPlayer>())
	,mMusicPlayer(std::make_unique<MusicPlayer>())
	,mTextures(std::make_unique<TextureHolder>())
	,mFonts(std::make_unique<OldFontHolder>())
	,mAIManager(std::make_unique<AIManager>())
	,mSceneManager(std::make_unique<SceneManager>())
	,mTerminal(std::make_unique<Terminal>())
	,mDebugCounter(std::make_unique<FPSCounter>())
	,mGui(std::make_unique<GUI>())
{
	mGameData.reset(new GameData(
		&mWindow,
		mSoundPlayer.get(),
		mMusicPlayer.get(),
		mTextures.get(),
		mFonts.get(),
		mAIManager.get(),
		mSceneManager.get(),
		mTerminal.get(),
		mGui.get()
	));

	Renderer::init(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	
	GameData* gameData = mGameData.get();

	loadFonts(gameData);
	mTerminal->init(gameData);
	mSceneManager->setGameData(gameData);
	mSceneManager->replaceScene("scenes/mainMenu.xml");

	mWindow.setVerticalSyncEnabled(true);
	mWindow.setKeyRepeatEnabled(false);

	// TODO_gui: do it some better way
	Widget::setWindow(&mWindow);
	Widget::setTextures(mTextures.get());

	ActionEventManager::init();

	// TEMPORARY GUI TEST
	auto menu = mGui->addInterface("buttons");

	auto* button1 = new Widget("button1");
	button1->setTexture("textures/gui/optionsButton.png");
	menu->addWidget(button1);

	auto* livebar = new Widget("livebar");
	livebar->setTexture("textures/gui/exitButton.png");
	livebar->setTopRightPosition({1.f, 0.f});
	menu->addWidget(livebar);

	auto* centerWidget = new Widget("button2");
	centerWidget->setTexture("textures/gui/creditsButton.png");
	centerWidget->setSize({0.5f, 0.5f});
	centerWidget->setCenterPosition({0.5f, 0.5f});
	menu->addWidget(centerWidget);

	auto* cbut1 = new Widget("cbut1");
	centerWidget->addChildWidget(cbut1);
	cbut1->setTexture("textures/gui/sewage.jpg");
	cbut1->setSize({0.25f, 0.25f});
	cbut1->setTopCenterPosition({0.5f, 0.f});
	cbut1->setColor(sf::Color::Magenta);

	auto* cbut2 = new TextWidget("cbut2");
	centerWidget->addChildWidget(cbut2);
	cbut2->setTexture("textures/gui/optionsButton.png");
	cbut2->setString("abCD123!");
	cbut2->setFontName("joystixMonospace.ttf");
	cbut2->setTopCenterPosition({0.5f, 0.f});
}

void Game::run()
{
	sf::Clock clock;
	while(mGameData->getGameCloser().shouldGameBeClosed() == false)
	{
		mSceneManager->changingScenesProcess();
		handleEvents();
		const float dt = clock.restart().asSeconds();
		constexpr float minimalDTConstrain = 1.f/20.f;
		update(dt > minimalDTConstrain ? minimalDTConstrain : dt);
	}

	Renderer::shutDown();
	mWindow.close();
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
		mDebugCounter->handleEvent(phEvent);
		mTerminal->handleEvent(phEvent);
		mGui->handleEvent(phEvent);
		
		if(!mTerminal->getSharedData()->isVisible)
			mSceneManager->handleEvent(phEvent);

		Renderer::handleEvent(phEvent);
	}
}

void Game::update(float dt)
{
	mDebugCounter->sampleFrame();

	if(mWindow.hasFocus())
	{
		mSceneManager->update(dt);
		mAIManager->update();
		mGui->update(dt);
		mTerminal->update();
		mDebugCounter->update();

		Renderer::endScene(mWindow);
		mWindow.display();
	}
}

}
