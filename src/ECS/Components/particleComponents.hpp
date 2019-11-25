#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>

class Texture;

namespace ph {

struct Particle
{
	sf::Vector2f position;
	float lifetime;
};

namespace component {

struct ParticleEmitter
{
	std::vector<Particle> particles;

	Texture* parTexture = nullptr;
	sf::Color parColor = sf::Color::White;
	sf::Vector2f offset = {0.f, 0.f};
	sf::Vector2f parInitialVelocity = {0.f, 1.f};
	sf::Vector2i parSize = {1, 1};
	unsigned amountOfParticles = 1;
	float parWholeLifetime = 1.f;
	float parSpeedScale = 1.f;
	bool isEmitting = true;

	bool wasInitialized = false;
};

// TODO: Color addition per frame
// TODO: Particle spawn area size
// TODO: Randomness

}}
