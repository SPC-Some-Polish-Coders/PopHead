#include "gun.hpp"
#include "gameData.hpp"

namespace ph {

Bullet::Bullet(const Entity& enemiesNode, const sf::Vector2f direction, const sf::Vector2f startPosition,
               const unsigned  damage, const unsigned range)
	:mDirection(direction)
	,mStartPosition(startPosition)
	,mEnemiesNode(enemiesNode)
	,mTraveledDistance(1)
	,mRange(range)
	,mDamage(damage)
{
	auto* characterWhoWasShot = getCharacterWhoWasShot();
	if(characterWhoWasShot == nullptr)
		return;
	makeSpriteOfShot();
	characterWhoWasShot->takeDamage(mDamage);
}

auto Bullet::getCharacterWhoWasShot() -> Character*
{
	while(isBulletStillInItsRange()) {
		for(auto& enemy : mEnemiesNode.getChildren()) {
			auto& e = dynamic_cast<Character&>(*enemy);
			if(wasEnemyShot(e))
				return &e;
		}
		++mTraveledDistance;
	}
	return nullptr;
}

bool Bullet::isBulletStillInItsRange()
{
	return mTraveledDistance < mRange;
}

bool Bullet::wasEnemyShot(Character& character)
{
	const auto& sprite = character.getSprite();
	const sf::FloatRect hitbox = sprite.getGlobalBounds();
	const sf::Vector2f currentPosition = mStartPosition + (mDirection	* static_cast<float>(mTraveledDistance));
	return hitbox.contains(currentPosition);
	// TODO: Make and use our own contains function
}

void Bullet::makeSpriteOfShot()
{
}

Gun::Gun(GameData* const gameData, const float damage)
	:Object(gameData, "gun", LayerID::kinematicEntities)
	,mDamage(damage)
{
}

void Gun::shoot(const ShotDirection shotDirection) const
{
	mGameData->getSoundPlayer().playAmbientSound("sounds/barretaShot.wav");
	auto& player = getParent();
	auto& root = player.getParent();
	auto& enemies = root.getChild("enemy_container");
	const sf::Vector2f shotDirectionVector = getShotDirectionVector(shotDirection);
	Bullet(enemies, shotDirectionVector, mPosition, 50, 1000);
}

auto Gun::getShotDirectionVector(const ShotDirection shotDirection) const -> const sf::Vector2f
{
	switch(shotDirection)
	{
		case ShotDirection::east:
			return sf::Vector2f(1, 0);
		case ShotDirection::west:
			return sf::Vector2f(-1, 0);
		case ShotDirection::south:
			return sf::Vector2f(0, 1);
		case ShotDirection::north:
			return sf::Vector2f(0, -1);
		case ShotDirection::northEast:
			return sf::Vector2f(1, -1);
		case ShotDirection::northWest:
			return sf::Vector2f(-1, -1);
		case ShotDirection::southEast:
			return sf::Vector2f(1, 1);
		case ShotDirection::southWest:
			return sf::Vector2f(-1, 1);
	}
}

void Gun::draw(sf::RenderTarget&, const sf::RenderStates) const
{
}

}