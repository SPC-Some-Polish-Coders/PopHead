#include "States/GameState/gameState.hpp"

#include "World/Entity/Objects/Characters/player.hpp"
#include "World/Entity/Objects/map.hpp"
#include "World/Entity/Objects/staticObjectToCamera.hpp"
#include "Base/gameData.hpp"
#include "Utilities/math.hpp"

using PopHead::States::GameState;

GameState::GameState(PopHead::Base::GameData* const gameData)
:State{gameData}
{
    loadResources();
    makeSceneTree();
}

void GameState::loadResources()
{
	mGameData->getTextures().load("resources/textures/characters/vaultManSheet.png");
	mGameData->getTextures().load("resources/textures/vehicles/boat.png");
	mGameData->getTextures().load("resources/textures/characters/vaultMan.png");
	mGameData->getTextures().load("resources/textures/map/city.png");
}

void GameState::makeSceneTree()
{
    makeMap();
    makeBoat();
    makeNPC();
    makePlayer();
	makeStaticObjectToCamera();
    setCamera();
}

void GameState::makePlayer()
{
    std::unique_ptr<World::Entity::Player> player(new World::Entity::Player(mGameData));
    player->getSprite().setTexture(mGameData->getTextures().get("resources/textures/characters/vaultManSheet.png"));
    player->setPosition(sf::Vector2f(1900, 5240));
    mRoot.addChild(std::move(player));
}

void GameState::makeBoat()
{
    std::unique_ptr<World::Entity::Character> boat(new World::Entity::Character(mGameData, "boat"));
    boat->getSprite().setTexture(mGameData->getTextures().get("resources/textures/vehicles/boat.png"));
    boat->setPosition(sf::Vector2f(1500, 5700));

    mRoot.addChild(std::move(boat));
}

void GameState::makeNPC()
{
    std::unique_ptr<World::Entity::Character> npc(new World::Entity::Character(mGameData, "npc"));
    npc->getSprite().setTexture(mGameData->getTextures().get("resources/textures/characters/vaultMan.png"));
    npc->setPosition(sf::Vector2f(1650, 5760));

    mRoot.getChild("boat").addChild(std::move(npc));
}

void GameState::makeMap()
{
    std::unique_ptr<World::Entity::Map> 
		city(new World::Entity::Map(mGameData, "cityMap", mGameData->getTextures().get("resources/textures/map/city.png"), 2));
    mRoot.addChild(std::move(city));
}

void PopHead::States::GameState::makeStaticObjectToCamera()
{
	auto object = std::make_unique<World::Entity::StaticObjectToCamera>(mGameData);
	mRoot.addChild(std::move(object));
}

void GameState::setCamera()
{
    const sf::Vector2f playerPosition(1900, 5220);
    mGameData->getRenderer().getCamera().setCenter(playerPosition);
}

void GameState::input()
{
    mRoot.input();

	if (INPUT_isKeyJustPressed(sf::Keyboard::Space))
		shouldCameraShake = true;
}

void GameState::update(sf::Time delta)
{
    mRoot.update(delta);

	if (shouldCameraShake)
		cameraShake();

	cameraMovement(delta);

	boatMovement(delta);
}

void GameState::cameraShake()
{
	constexpr float cameraShakeStrength = 10.f;
	mGameData->getRenderer().startShaking(cameraShakeStrength);
	shouldCameraShake = false;
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
