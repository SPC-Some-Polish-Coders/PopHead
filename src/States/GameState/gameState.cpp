#include "States/GameState/gameState.hpp"

#include "SFML/Graphics.hpp"
#include "World/Entity/Objects/Characters/player.hpp"
#include "World/Entity/Objects/map.hpp"
#include "World/Entity/Objects/staticObjectToCamera.hpp"
#include "World/Entity/Objects/shapeWithCollision.hpp"
#include "World/Entity/Objects/Characters/enemies/zombie.hpp"
#include "Base/gameData.hpp"
#include "Utilities/math.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"

using PopHead::States::GameState;

GameState::GameState(PopHead::Base::GameData* const gameData)
	:State{ gameData }
{
	loadResources();
	makeSceneTree();
}

void GameState::loadResources()
{
	mGameData->getTextures().load("resources/textures/characters/vaultManSheet.png");
	mGameData->getTextures().load("resources/textures/vehicles/boat.png");
	mGameData->getTextures().load("resources/textures/characters/vaultMan.png");
	mGameData->getTextures().load("resources/textures/characters/zombie.png");
	mGameData->getTextures().load("resources/textures/map/city.png");
	mGameData->getTextures().load("resources/textures/others/box.png");
	mGameData->getTextures().load("resources/textures/others/ball.png");
}

void GameState::makeSceneTree()
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
	makeStaticObjectToCamera();
	setCamera();
	playMusic();
}

void GameState::makeMap()
{
	std::unique_ptr<World::Entity::Map>
		city(new World::Entity::Map(mGameData, "cityMap", mGameData->getTextures().get("resources/textures/map/city.png"), 2));
	mRoot.addChild(std::move(city));
}

void GameState::makeWall()
{
	auto wall = std::make_unique<World::Entity::ShapeWithCollision>(mGameData);
	wall->setPosition(sf::Vector2f(2000, 5000));
	mRoot.getChild("cityMap").addChild(std::move(wall));
}

void GameState::makeBoat()
{
	std::unique_ptr<World::Entity::Character> boat(new World::Entity::Character(mGameData, "boat"));
	boat->getSprite().setTexture(mGameData->getTextures().get("resources/textures/vehicles/boat.png"));
	boat->setPosition(sf::Vector2f(1500, 5700));

	mRoot.addChild(std::move(boat));
}

void GameState::makeNpc()
{
	std::unique_ptr<World::Entity::Character> npc(new World::Entity::Character(mGameData, "npc"));
	npc->getSprite().setTexture(mGameData->getTextures().get("resources/textures/characters/vaultMan.png"));
	npc->setPosition(sf::Vector2f(1650, 5760));

	mRoot.getChild("boat").addChild(std::move(npc));
}

void GameState::makeNpcToBeAbleToTestDynamicCollisions()
{
	constexpr float mass = 50.f;
	std::unique_ptr<World::Entity::Character> npcq(new World::Entity::Character(
		mGameData, "dynamicCollisionsTesterNPC", PopHead::World::Animation(), 50, 100, 100, sf::FloatRect(0, 0, 30, 44), mass));
	npcq->getSprite().setTexture(mGameData->getTextures().get("resources/textures/characters/vaultMan.png"));
	npcq->setPosition(sf::Vector2f(1850, 5240));

	mRoot.addChild(std::move(npcq));
}

void GameState::makePlayer()
{
	std::unique_ptr<World::Entity::Player> player(new World::Entity::Player(mGameData));
	player->getSprite().setTexture(mGameData->getTextures().get("resources/textures/characters/vaultManSheet.png"));
	player->setPosition(sf::Vector2f(1900, 5240));
	mRoot.addChild(std::move(player));
}

void GameState::makeZombie()
{
	auto zombie = std::make_unique<World::Entity::Zombie>(mGameData);
	zombie->setPosition(sf::Vector2f(1935, 5000));
	mRoot.addChild(std::move(zombie));
}

void GameState::makeBox()
{
	constexpr float mass = 49.5f;
	auto box = std::make_unique<World::Entity::Character>(
		mGameData, "box", PopHead::World::Animation(), 0, 0, 0, sf::FloatRect(0, 0, 57, 81), mass);
	box->setPosition(sf::Vector2f(1800, 4900));
	box->getSprite().setTexture(mGameData->getTextures().get("resources/textures/others/box.png"));
	mRoot.addChild(std::move(box));
}

void GameState::makeBall()
{
	constexpr float mass = 15.f;
	auto ball = std::make_unique<World::Entity::Character>(
		mGameData, "ball", PopHead::World::Animation(), 0, 0, 0, sf::FloatRect(0, 0, 30, 30), mass);
	ball->setPosition(sf::Vector2f(1600, 5150));
	ball->setScale(sf::Vector2f(0.4f, 0.4f));
	ball->getSprite().setTexture(mGameData->getTextures().get("resources/textures/others/ball.png"));
	mRoot.addChild(std::move(ball));
}

void GameState::makeStaticObjectToCamera()
{
	auto object = std::make_unique<World::Entity::StaticObjectToCamera>(mGameData);
	mRoot.addChild(std::move(object));
}

void GameState::setCamera()
{
	const sf::Vector2f playerPosition(1900, 5220);
	mGameData->getRenderer().getCamera().setCenter(playerPosition);
}

void GameState::playMusic()
{
	mGameData->getMusicPlayer().play("resources/music/explorationTheme.ogg");
}

void GameState::input()
{
	mRoot.input();

	if (mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Space))
		mShouldCameraShake = true;

	handleCollisionDebugShortcuts();

	windowMinimalizeAndMaximalizeShortcut();
}

void GameState::handleCollisionDebugShortcuts()
{
	if (mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::F1)) {
		auto& collisionDebugSettings = PopHead::Physics::CollisionDebugSettings::getInstance();

		if (mGameData->getInput().getKeyboard().isKeyPressed(sf::Keyboard::LControl)) {
			switchCollisionDebugMode();
		}
		else if (mGameData->getInput().getKeyboard().isKeyPressed(sf::Keyboard::Num1)) {
			collisionDebugSettings.setColors(1);
		}
		else if (mGameData->getInput().getKeyboard().isKeyPressed(sf::Keyboard::Num2)) {
			collisionDebugSettings.setColors(2);
		}
		else if (mGameData->getInput().getKeyboard().isKeyPressed(sf::Keyboard::Num3)) {
			collisionDebugSettings.setColors(3);
		}
		else {
			turnOnAndTurnOffCollisionDebugSettings();
		}
	}
}

void GameState::switchCollisionDebugMode()
{
	auto& collisionDebugSettings = PopHead::Physics::CollisionDebugSettings::getInstance();

	++mCollisionDebugMode;
	if (mCollisionDebugMode == 4)
		mCollisionDebugMode = 1;

	switch (mCollisionDebugMode)
	{
	case 1:
		collisionDebugSettings.displayAllBodies();
		break;

	case 2:
		collisionDebugSettings.displayOnlyKinematicBodies();
		break;

	case 3:
		collisionDebugSettings.displayOnlyStaticBodies();
		break;
	}
}

void GameState::turnOnAndTurnOffCollisionDebugSettings()
{
	auto& collisionDebugSettings = PopHead::Physics::CollisionDebugSettings::getInstance();

	if (mIsCollisionDebugTurnOn) {
		collisionDebugSettings.turnOff();
		mIsCollisionDebugTurnOn = false;
	}
	else {
		collisionDebugSettings.turnOn();
		mIsCollisionDebugTurnOn = true;
	}
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
			mGameData->getRenderer().getWindow().create(sf::VideoMode(1000, 760), "PopHead", sf::Style::Default);
			break;
		case WindowSizeState::notFullScreen:
			windowSizeState = WindowSizeState::fullScreen;
			mGameData->getRenderer().getWindow().create(sf::VideoMode(), "PopHead", sf::Style::Fullscreen);
			break;
		}
	}
}

void GameState::update(sf::Time delta)
{
	mRoot.update(delta);

	if (mShouldCameraShake)
		cameraShake();

	cameraMovement(delta);

	boatMovement(delta);
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
	const sf::FloatRect characterBounds = dynamic_cast<World::Entity::Character&>(mRoot.getChild("player")).getSprite().getGlobalBounds();
	mGameData->getRenderer().moveCamera(Utilities::Math::getCenter(characterBounds), cameraMotionSpeed * delta.asSeconds());
}

void GameState::boatMovement(sf::Time delta)
{
	auto& boat = dynamic_cast<World::Entity::Character&>(mRoot.getChild("boat"));
	boat.move(sf::Vector2f(delta.asSeconds() * -15, 0));
}
