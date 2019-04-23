#include "States/GameState/gameState.hpp"

#include "World/Entity/Objects/Characters/player.hpp"
#include "World/Entity/Objects/map.hpp"

#include "Base/gameData.hpp"

#include "Utilities/math.hpp"
#include "Utilities/random.hpp"

using PopHead::States::GameState;

GameState::GameState(PopHead::Base::GameData* const gameData)
:State{gameData}
{
    mPlayerTexture.loadFromFile("resources/textures/characters/vaultManSheet.png");
    std::unique_ptr<World::Entity::Player> player(new World::Entity::Player(gameData));
    player->getSprite().setTexture(mPlayerTexture);
    mRoot.addChild(std::move(player));

    mMapTexture.loadFromFile("resources/textures/map/city.png");
    std::unique_ptr<World::Entity::Map> city(new World::Entity::Map(gameData, "cityMap", mMapTexture, 2));
    mRoot.addChild(std::move(city));
}

void GameState::input()
{
    mRoot.input();
	if (mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Key::Space)) {
		const float cameraShakeStrength = 10.f;
		mGameData->getRenderer().setCameraShakeStrength(cameraShakeStrength);
	}
}

void GameState::update(sf::Time delta)
{
    mRoot.update(delta);

	const float cameraMotionSpeed = 4.f;
	const sf::FloatRect characterBounds = dynamic_cast<World::Entity::Character&>(mRoot.getChild("player")).getSprite().getGlobalBounds();
	mGameData->getRenderer().moveCamera(Utilities::Math::getCenter(characterBounds), cameraMotionSpeed * delta.asSeconds());

	const float cameraShakeStrengthLoss = 10.f;
	mGameData->getRenderer().shakeCamera(cameraShakeStrengthLoss * delta.asSeconds());
}
