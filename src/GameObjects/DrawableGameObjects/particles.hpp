#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class Renderer;

struct Particle
{
	sf::Vector2f velocity;
};

class Particles : public GameObject
{
public:
	Particles(Renderer&, const sf::Vector2f);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates) const override;

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
