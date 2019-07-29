#pragma once

#include "GameObjects/drawableGameObject.hpp"

namespace ph {

struct Particle
{
	sf::Vector2f velocity;
};

class Particles : public DrawableGameObject
{
public:
	Particles(GameData* const, const sf::Vector2f);

	void update(const sf::Time delta) override;
	void draw(sf::RenderTarget& target, sf::RenderStates) const override;

private:
	void initializeParticles();
	void killParticles();

private:
	const unsigned mNumberOfParticles;

	sf::Time mLifetime;
	sf::Vector2f mPosition;
	sf::VertexArray mVertecies;
	std::vector<Particle> mParticles;

};


}
