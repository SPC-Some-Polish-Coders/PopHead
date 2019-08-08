#include "npc.hpp"

#include "Resources/collisionRectData.hpp"
#include "gameData.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"

namespace ph {

namespace
{
	const std::string name = "npc";
	const Animation animation;
	constexpr unsigned int movementSpeed = 100;
	constexpr unsigned int hp = 100;
	constexpr unsigned int maxHp = 100;
	const sf::FloatRect posAndSize(
		0,
		0,
		CollisionRectData::PLAYER_WIDTH,
		CollisionRectData::PLAYER_HEIGHT
	);
	constexpr float mass = 25;
}

Npc::Npc(GameData* gameData)
	:Character(gameData, name, animation, movementSpeed, hp, maxHp, posAndSize, mass)
{
	getSprite().setTexture(mGameData->getTextures().get("textures/characters/vaultMan.png"));
}

void Npc::input()
{
}

void Npc::update(sf::Time delta)
{
	setPosition(mCollisionBody.getPosition());
}

}