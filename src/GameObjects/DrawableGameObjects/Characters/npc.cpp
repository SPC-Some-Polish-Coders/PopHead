#include "npc.hpp"
#include "Resources/collisionRectData.hpp"
#include "gameData.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "Resources/spriteSheetData.hpp"

namespace ph {

namespace
{
	constexpr unsigned int movementSpeed = 55;
	constexpr unsigned int hp = 50;
	constexpr unsigned int maxHp = 50;
	const sf::FloatRect posAndSize(
		0,
		0,
		CollisionRectData::HUMAN_WIDTH,
		CollisionRectData::HUMAN_HEIGHT
	);
	constexpr float mass = 25;

	const Animation animation(
		std::array<std::string, 8>{
			"down", "right", "left", "rightUp", "leftUp", "up", "crawlingUp", "dead"
		},
		std::array<sf::IntRect, 8>{
			sf::IntRect(0, 0 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 1 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 2 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 3 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 4 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 5 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 6 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT),
			sf::IntRect(0, 7 * SpriteSheetData::HUMAN_HEIGHT, SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT)
		},
		{
			2, 2, 2, 2, 2, 2, 2, 1
		},
		sf::seconds(0.30f)
	);
}

Npc::Npc(GameData* gameData, const std::string& name)
	:Character(gameData, name, animation, movementSpeed, hp, maxHp, posAndSize, mass)
{
	getSprite().setTexture(mGameData->getTextures().get("textures/characters/playerFullAnimation.png"));
	mAnimation.animate(mSprite);
}

void Npc::update(sf::Time delta)
{
	setPosition(mCollisionBody.getPosition());
	mAnimation.animate(mSprite, delta);
}

}
