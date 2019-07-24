#include "gameData.hpp"
#include "GameObjects/DrawableGameObjects/melee.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"

namespace ph {

Swoosh::Swoosh(const GameObject& opponentsNode, const sf::Vector2f direction, const sf::Vector2f position,
	const unsigned damage, const unsigned range)
	:mEnemiesNode(opponentsNode)
	,mDirection(direction)
	,mDamage(damage)
	,mRange(range)
{
	setMeeleWeaponHitArea(direction, position);
	auto* characterWhoWasHit = getCharacterWhoWasShot();
	if (characterWhoWasHit == nullptr)
		return;
	characterWhoWasHit->takeDamage(mDamage);
}


auto Swoosh::getCharacterWhoWasShot() -> Character*
{
	for (auto& enemy : mEnemiesNode.getChildren()) {
		auto& e = dynamic_cast<Character&>(*enemy);
		if (wasEnemyHit(e))
			return &e;
	}
	return nullptr;
}

bool Swoosh::wasEnemyHit(Character& character)
{
	const auto& sprite = character.getSprite();
	const sf::FloatRect hitbox = sprite.getGlobalBounds();
	return Math::isPointInsideRect(mHitArea[2].position , hitbox);
}

void Swoosh::setMeeleWeaponHitArea(const sf::Vector2f attackDirection, const sf::Vector2f startPosition)
{
	mHitArea[0].position = startPosition;
	mHitArea[1].position = startPosition + (attackDirection * mRange);
	mHitArea[2].position = startPosition + (attackDirection * mRange);
	mHitArea[3].position = startPosition + (attackDirection * mRange);

	if (attackDirection == sf::Vector2f(1, 0))
	{
		mHitArea[1].position += {-5.f, 10.f};
		mHitArea[3].position += {-5.f, -10.f};
	}
	else if (attackDirection == sf::Vector2f(-1, 0))
	{
		mHitArea[1].position += {5.f, -10.f};
		mHitArea[3].position += {5.f, 10.f};
	}
	else if (attackDirection == sf::Vector2f(0, 1))
	{
		mHitArea[1].position += {-10.f, -5.f};
		mHitArea[3].position += {10.f, -5.f};
	}
	else if (attackDirection == sf::Vector2f(0, -1))
	{
		mHitArea[1].position += {10.f, 5.f};
		mHitArea[3].position += {-10.f, 5.f};
	}
	else if (attackDirection == sf::Vector2f(0.7f, -0.7f))
	{
		mHitArea[1].position += {0.f, 12.f};
		mHitArea[3].position += {-12.f, 0.f};
	}
	else if (attackDirection == sf::Vector2f(-0.7f, -0.7f))
	{
		mHitArea[1].position += {12.f, 0.f};
		mHitArea[3].position += {0.f, 12.f};
	}
	else if (attackDirection == sf::Vector2f(0.7f, 0.7f))
	{
		mHitArea[1].position += {0.f, -12.f};
		mHitArea[3].position += {-12.f, 0.f};
	}
	else if (attackDirection == sf::Vector2f(-0.7f, 0.7f))
	{
		mHitArea[1].position += {0.f, -12.f};
		mHitArea[3].position += {12.f, 0.f};
	}
}

MeleeWeapon::MeleeWeapon(GameData* const gameData, const float damage, const float range)
	:DrawableGameObject(gameData, "sword", LayerID::kinematicEntities)
	,mDamage(damage)
	,mRange(range)
{
}

void MeleeWeapon::attack(const sf::Vector2f attackDirection)
{
	mGameData->getSoundPlayer().playAmbientSound("sounds/swordAttack.wav");
	auto& player = getParent();
	auto& root = player.getParent();
	auto& enemies = root.getChild("enemy_container");
	setMeleeWeaponPositionToRightHand(attackDirection);
	Swoosh swoosh(enemies, attackDirection, mPosition, mDamage, mRange);
	initializeAttackGraphics(swoosh);
	mTimeFromLastAttack.restart();
}

void MeleeWeapon::setMeleeWeaponPositionToRightHand(const sf::Vector2f attackDirection)
{
	if (attackDirection == sf::Vector2f(1, 0))
		mPosition += {20, 20};
	else if (attackDirection == sf::Vector2f(-1, 0))
		mPosition += {5, 15};
	else if (attackDirection == sf::Vector2f(0, 1))
		mPosition += {3, 20};
	else if (attackDirection == sf::Vector2f(0, -1))
		mPosition += {15, 15};
	else if (attackDirection == sf::Vector2f(0.7f, -0.7f))
		mPosition += {20, 3};
	else if (attackDirection == sf::Vector2f(-0.7f, -0.7f))
		mPosition += {3, 3};
	else if (attackDirection == sf::Vector2f(0.7f, 0.7f))
		mPosition += {10, 20};
	else if (attackDirection == sf::Vector2f(-0.7f, 0.7f))
		mPosition += {0, 10};
	else
		PH_EXCEPTION("Direction vector like this shouldn't exist.");
}

void MeleeWeapon::initializeAttackGraphics(const Swoosh& swoosh)
{
	mHitGraphics = swoosh.getHitArea();
	for (auto& hitPoint : mHitGraphics)
		hitPoint.color = sf::Color(255, 0, 0, 100);
}

void MeleeWeapon::resetAttackGraphics()
{
	for (auto& areaPoint : mHitGraphics)
		areaPoint.position = { 0, 0 };
}

void MeleeWeapon::update(const sf::Time delta)
{
	if (mTimeFromLastAttack.getElapsedTime().asSeconds() > .05f)
		resetAttackGraphics();
}

void MeleeWeapon::draw(sf::RenderTarget& target, sf::RenderStates) const
{

	target.draw(mHitGraphics.data(), 4, sf::TrianglesFan);
}

}