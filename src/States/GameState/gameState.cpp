#include "States/GameState/gameState.hpp"

#include "SFML/Graphics.hpp"
#include "World/Entity/Objects/Characters/player.hpp"
#include "World/Entity/Objects/Map/map.hpp"
#include "World/Entity/Objects/shapeWithCollision.hpp"
#include "World/Entity/Objects/Characters/enemies/zombie.hpp"
#include "World/Entity/Objects/Characters/npc.hpp"
#include "gameData.hpp"
#include "Utilities/math.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"

namespace ph {

GameState::GameState(GameData* const gameData)
	:State(gameData)
{
	loadResources();
	makeSceneTree();
}

void GameState::loadResources()
{
	mGameData->getTextures().load("textures/map/FULL_DESERT_TILESET_WIP.png");
	mGameData->getTextures().load("textures/characters/vaultManSheet.png");
	mGameData->getTextures().load("textures/characters/vaultMan.png");
	mGameData->getTextures().load("textures/characters/zombie.png");
}

void GameState::makeSceneTree()
{
	makeMap();
	makeNpc();
	makePlayer();
	makeZombie();
	playMusic();
}

void GameState::makeMap()
{
	auto map = std::make_unique<Map>(mGameData, "desertMap");
	map->loadFromFile("maps/mediumDesertMap.tmx");
	mRoot.addChild(std::move(map));
}

void GameState::makeNpc()
{
	auto npc = std::make_unique<Npc>(mGameData);
	npc->setPosition(sf::Vector2f(50, 50));
	mRoot.addChild(std::move(npc));
}

void GameState::makePlayer()
{
	auto player = std::make_unique<Player>(mGameData);
	//std::unique_ptr<Player> player(new Player(mGameData));
	player->getSprite().setTexture(mGameData->getTextures().get("textures/characters/vaultManSheet.png"));
	mRoot.addChild(std::move(player));
}

void GameState::makeZombie()
{
	auto zombie = std::make_unique<Zombie>(mGameData);
	zombie->setPosition(sf::Vector2f(800, 300));
	mRoot.addChild(std::move(zombie));
}

void GameState::playMusic()
{
	mGameData->getMusicPlayer().play("music/explorationTheme.ogg");
}

void GameState::input()
{
	mRoot.input();
	handleCameraShakeShortcut();
	windowMinimalizeAndMaximalizeShortcut();
	shotgunShot();
}

void GameState::handleCameraShakeShortcut()
{
	if(mGameData->getInput().getAction().isActionJustPressed("cameraShake"))
		mShouldCameraShake = true;
}

void GameState::windowMinimalizeAndMaximalizeShortcut()
{
	enum class WindowSizeState {fullScreen, notFullScreen};
	static WindowSizeState windowSizeState = WindowSizeState::fullScreen;

	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::F11)) {
		switch(windowSizeState)
		{
		case WindowSizeState::fullScreen:
			windowSizeState = WindowSizeState::notFullScreen;
			mGameData->getRenderer().getWindow().create(sf::VideoMode(1000, 750), "PopHead", sf::Style::Default);
			break;
		case WindowSizeState::notFullScreen:
			windowSizeState = WindowSizeState::fullScreen;
			mGameData->getRenderer().getWindow().create(sf::VideoMode(), "PopHead", sf::Style::Fullscreen);
			break;
		}
	}
}

void GameState::shotgunShot()
{
	// It's an sound player test.
	if(mGameData->getInput().getAction().isActionJustPressed("shotgunShot"))
		mGameData->getSoundPlayer().playAmbientSound("sounds/barretaShot.wav");
}

void GameState::update(sf::Time delta)
{
	mRoot.update(delta);
	if (mShouldCameraShake)
		cameraShake();
	cameraMovement(delta);
	updateListenerPosition();
}

void GameState::cameraShake()
{
	constexpr float cameraShakeStrength = 10.f;
	mGameData->getRenderer().startShaking(cameraShakeStrength);
	mShouldCameraShake = false;
}

void GameState::cameraMovement(sf::Time delta) const
{
	constexpr float cameraMotionSpeed = 4.f;
	const sf::FloatRect characterBounds = dynamic_cast<Character&>(mRoot.getChild("player")).getSprite().getGlobalBounds();
	mGameData->getRenderer().moveCamera(Math::getCenter(characterBounds), cameraMotionSpeed * delta.asSeconds());
}


void GameState::updateListenerPosition()
{
	Object& player = dynamic_cast<Object&>(mRoot.getChild("player"));
	mGameData->getSoundPlayer().setListenerPosition(player.getPosition());
}

}