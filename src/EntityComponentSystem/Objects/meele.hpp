#pragma once

#include "EntityComponentSystem/object.hpp"

namespace ph{

class Sword : public Object
{
public:
	Sword(GameData* const, const float damage, const float range);

	void update(const sf::Time delta) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void hit(const sf::Vector2f hitDirection);

private:
	void initializeHit();
	void resetHit();

private:
	sf::VertexArray mHitGraphic;
	sf::Clock mTimeFromLastHit;
	const float mDamage;
	const float mRange;

};

}