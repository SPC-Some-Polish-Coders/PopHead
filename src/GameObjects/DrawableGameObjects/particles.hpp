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
	Particles(Renderer&, const sf::Vector2f);

	void update(const sf::Time delta) override;
	void draw(sf::RenderTarget& target, sf::RenderStates) const override;

private:
	void initializeParticles();
	void killParticles();

private:
	sf::Time mLifetime;
	sf::Vector2f mPosition;
	const unsigned mNumberOfParticles;
	std::vector<Particle> mParticles;
	sf::VertexArray mVertecies;
};

}
