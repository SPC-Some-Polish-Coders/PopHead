#include "States/GameState/gameState.hpp"

#include "SFML/Graphics.hpp"
#include "World/Entity/Objects/Characters/player.hpp"
#include "World/Entity/Objects/map.hpp"
#include "World/Entity/Objects/shapeWithCollision.hpp"
#include "World/Entity/Objects/Characters/enemies/zombie.hpp"
#include "gameData.hpp"
#include "Utilities/math.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"

ph::GameState::GameState(GameData* const gameData)
	:State(gameData)
{
	loadResources();
	makeSceneTree();
}

void ph::GameState::loadResources()
{
	mGameData->getTextures().load("textures/map/FULL_DESERT_TILESET_WIP.png");
	mGameData->getTextures().load("textures/characters/vaultManSheet.png");
	mGameData->getTextures().load("textures/vehicles/boat.png");
	mGameData->getTextures().load("textures/characters/vaultMan.png");
	mGameData->getTextures().load("textures/characters/zombie.png");
	mGameData->getTextures().load("textures/others/box.png");
	mGameData->getTextures().load("textures/others/ball.png");
}

void ph::GameState::makeSceneTree()
{
	makeMap();
	makeWall();
	makeBoat();
	makeNpc();
	makeNpcToBeAbleToTestDynamicCollisions();
	makePlayer();
	makeZombie();
	makeBox();
	makeBall();
	playMusic();
}

void ph::GameState::makeMap()
{
	auto map = std::make_unique<Map>(mGameData, "desertMap");
	map->loadFromFile("maps/desertMap.tmx");
	mRoot.addChild(std::move(map));
}

void ph::GameState::makeWall()
{
	auto wall = std::make_unique<ShapeWithCollision>(mGameData);
	wall->setPosition(sf::Vector2f(50, 50));
	mRoot.getChild("desertMap").addChild(std::move(wall));
}

void ph::GameState::makeBoat()
{
	std::unique_ptr<Character> boat(new Character(mGameData, "boat"));
	boat->getSprite().setTexture(mGameData->getTextures().get("textures/vehicles/boat.png"));
	boat->setPosition(sf::Vector2f(-300, 700));

	mRoot.addChild(std::move(boat));
}

void ph::GameState::makeNpc()
{
	std::unique_ptr<Character> npc(new Character(mGameData, "npc"));
	npc->getSprite().setTexture(mGameData->getTextures().get("textures/characters/vaultMan.png"));
	npc->setPosition(sf::Vector2f(-150, 760));

	mRoot.getChild("boat").addChild(std::move(npc));
}

void ph::GameState::makeNpcToBeAbleToTestDynamicCollisions()
{
	constexpr float mass = 50.f;
	std::unique_ptr<Character> npcq(new Character(
		mGameData, "dynamicCollisionsTesterNPC", Animation(), 50, 100, 100, sf::FloatRect(0, 0, 30, 44), mass));
	npcq->getSprite().setTexture(mGameData->getTextures().get("textures/characters/vaultMan.png"));
	npcq->setPosition(sf::Vector2f(400, 400));

	mRoot.addChild(std::move(npcq));
}

void ph::GameState::makePlayer()
{
	std::unique_ptr<Player> player(new Player(mGameData));
	player->getSprite().setTexture(mGameData->getTextures().get("textures/characters/vaultManSheet.png"));
	mRoot.addChild(std::move(player));
}

void ph::GameState::makeZombie()
{
	auto zombie = std::make_unique<Zombie>(mGameData);
	zombie->setPosition(sf::Vector2f(800, 300));
	mRoot.addChild(std::move(zombie));
}

void ph::GameState::makeBox()
{
	constexpr float mass = 49.5f;
	auto box = std::make_unique<Character>(
		mGameData, "box", Animation(), 0, 0, 0, sf::FloatRect(0, 0, 57, 81), mass);
	box->setPosition(sf::Vector2f(100, 300));
	box->getSprite().setTexture(mGameData->getTextures().get("textures/others/box.png"));
	mRoot.addChild(std::move(box));
}

void ph::GameState::makeBall()
{
	constexpr float mass = 15.f;
	auto ball = std::make_unique<Character>(
		mGameData, "ball", Animation(), 0, 0, 0, sf::FloatRect(0, 0, 30, 30), mass);
	ball->setPosition(sf::Vector2f(505, 505));
	ball->setScale(sf::Vector2f(0.4f, 0.4f));
	ball->getSprite().setTexture(mGameData->getTextures().get("textures/others/ball.png"));
	mRoot.addChild(std::move(ball));
}

void ph::GameState::playMusic()
{
	mGameData->getMusicPlayer().play("music/explorationTheme.ogg");
}

void ph::GameState::input()
{
	mRoot.input();
	handleCameraShakeShortcut();
	windowMinimalizeAndMaximalizeShortcut();
	shotgunShot();
}

void ph::GameState::handleCameraShakeShortcut()
{
	if(mGameData->getInput().getAction().isActionJustPressed("cameraShake"))
		mShouldCameraShake = true;
}

void ph::GameState::windowMinimalizeAndMaximalizeShortcut()
{
	enum class WindowSizeState {fullScreen, notFullScreen};
	static WindowSizeState windowSizeState = WindowSizeState::fullScreen;

	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::F11)) {
		switch(windowSizeState)
		{
		case WindowSizeState::fullScreen:
			windowSizeState = WindowSizeState::notFullScreen;
			mGameData->getRenderer().getWindow().create(sf::VideoMode(1000, 760), "PopHead", sf::Style::Default);
			break;
		case WindowSizeState::notFullScreen:
			windowSizeState = WindowSizeState::fullScreen;
			mGameData->getRenderer().getWindow().create(sf::VideoMode(), "PopHead", sf::Style::Fullscreen);
			break;
		}
	}
}

void ph::GameState::shotgunShot()
{
	// It's an sound player test.
	if(mGameData->getInput().getAction().isActionJustPressed("shotgunShot"))
		mGameData->getSoundPlayer().playAmbientSound("sounds/barretaShot.wav");
}

void ph::GameState::update(sf::Time delta)
{
	mRoot.update(delta);
	if (mShouldCameraShake)
		cameraShake();
	cameraMovement(delta);
	boatMovement(delta);
	updateListenerPosition();
}

void ph::GameState::cameraShake()
{
	constexpr float cameraShakeStrength = 10.f;
	mGameData->getRenderer().startShaking(cameraShakeStrength);
	mShouldCameraShake = false;
}

void ph::GameState::cameraMovement(sf::Time delta) const
{
	constexpr float cameraMotionSpeed = 4.f;
	const sf::FloatRect characterBounds = dynamic_cast<Character&>(mRoot.getChild("player")).getSprite().getGlobalBounds();
	mGameData->getRenderer().moveCamera(Math::getCenter(characterBounds), cameraMotionSpeed * delta.asSeconds());
}

void ph::GameState::boatMovement(sf::Time delta)
{
	auto& boat = dynamic_cast<Character&>(mRoot.getChild("boat"));
	boat.move(sf::Vector2f(delta.asSeconds() * -15, 0));
}

void ph::GameState::updateListenerPosition()
{
	Object& player = dynamic_cast<Object&>(mRoot.getChild("player"));
	mGameData->getSoundPlayer().setListenerPosition(player.getPosition());
}
