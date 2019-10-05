#include <GL/glew.h>
#include "game.hpp"
#include "Resources/loadFonts.hpp"
#include "Events/globalKeyboardShortcuts.hpp"
#include "Events/eventDispatcher.hpp"
#include "Events/actionEventManager.hpp"
#include "Logs/logs.hpp"
#include <SFML/System.hpp>

namespace ph {

Game::Game()
	:mGameData{}
	,mRenderWindow(sf::VideoMode::getDesktopMode(), "PopHead", sf::Style::Default,
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
	,mRenderer{new Renderer()}
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
	mSceneManager->setGameData(gameData);
	mSceneManager->replaceScene("scenes/mainMenu.xml");

	mRenderWindow.setVerticalSyncEnabled(true);

	ActionEventManager::init();
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

	mRenderWindow.close();
}

sf::Time Game::getProperDeltaTime(sf::Time deltaTime)
{
	const sf::Time minimalDeltaTimeConstrain = sf::seconds(1.f/20.f);
	return deltaTime > minimalDeltaTimeConstrain ? minimalDeltaTimeConstrain : deltaTime;
}

void Game::handleEvents()
{
	ph::Event phEvent;
	while(EventDispatcher::dispatchEvent(phEvent, mRenderWindow))
	{
		handleGlobalKeyboardShortcuts(mGameData->getRenderWindow(), mGameData->getGameCloser(), phEvent);
		mEfficiencyRegister->handleEvent(phEvent);
		mTerminal->handleEvent(phEvent);
		mGui->handleEvent(phEvent);
		
		if(!mTerminal->getSharedData()->mIsVisible)
			mSceneManager->handleEvent(phEvent);
	}
}

void Game::update(sf::Time deltaTime)
{
	/*mEfficiencyRegister->update();

	if(mRenderWindow.hasFocus())
	{
		mSceneManager->update(deltaTime);
		mAIManager->update();
		mPhysicsEngine->update(deltaTime);
		mGui->update(deltaTime);
		mTerminal->update();
	}*/

	// initialization
	static auto shader = std::make_shared<Shader>("resources/shaders/basic.vs.glsl", "resources/shaders/basic.fs.glsl");

	static std::array<float, 16> vertices = {
		// positions | texture coords
		0.5f,  0.5f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f  // top left 
	};

	static std::array<unsigned, 6> indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	static VertexBuffer vbo = createVertexBuffer(vertices.data(), vertices.size() * sizeof(float));
	static IndexBuffer ibo = createIndexBuffer(indices.data(), indices.size() * sizeof(unsigned));
	static auto vao = std::make_shared<VertexArray>();
	vao->setVertexBuffer(vbo, VertexBufferLayout::position2_texCoords2);
	vao->setIndexBuffer(ibo);

	static auto texture = std::make_shared<Texture>("resources/textures/others/gate.png");

	static auto camera = std::make_shared<Camera>();

	// Actual game loop
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		camera->move({-0.02f, 0.f});
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		camera->move({0.02f, 0.f});
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		camera->move({0.f, 0.02f});
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		camera->move({0.f, -0.02f});

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		camera->zoom(1.01);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		camera->zoom(0.99);

	mRenderer->beginScene(*camera);

	texture->bind();
	mRenderer->submit(vao, shader);

	mRenderer->endScene();
	
	mRenderWindow.display();
}

}
