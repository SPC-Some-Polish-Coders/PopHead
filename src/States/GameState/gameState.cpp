#include "States/GameState/gameState.hpp"

#include "World/Entity/Objects/Characters/player.hpp"
#include "World/Entity/Objects/map.hpp"
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
    mPlayerTexture.loadFromFile("resources/textures/characters/vaultManSheet.png");
    mMapTexture.loadFromFile("resources/textures/map/city.png");
}

void GameState::makeSceneTree()
{
    makePlayer();
    makeMap();
}

void GameState::makePlayer()
{
    std::unique_ptr<World::Entity::Player> player(new World::Entity::Player(mGameData));
    player->getSprite().setTexture(mPlayerTexture);
    mRoot.addChild(std::move(player));
}

void GameState::makeMap()
{
    std::unique_ptr<World::Entity::Map> city(new World::Entity::Map(mGameData, "cityMap", mMapTexture, 2));
    mRoot.addChild(std::move(city));
}

void GameState::input()
{
    mRoot.input();
	if (mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Key::Space)) {
		constexpr float cameraShakeStrength = 10.f;
		mGameData->getRenderer().startShaking(cameraShakeStrength);
	}
}

void GameState::update(sf::Time delta)
{
    mRoot.update(delta);

	constexpr float cameraMotionSpeed = 4.f;
	const sf::FloatRect characterBounds = dynamic_cast<World::Entity::Character&>(mRoot.getChild("player")).getSprite().getGlobalBounds();
	mGameData->getRenderer().moveCamera(Utilities::Math::getCenter(characterBounds), cameraMotionSpeed * delta.asSeconds());
}
