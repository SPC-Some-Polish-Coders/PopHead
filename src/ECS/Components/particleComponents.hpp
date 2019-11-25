#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>

class Texture;

namespace ph {

struct Particle
{
	sf::Vector2f position;
	float lifetime = 0.f;
};

namespace component {

struct ParticleEmitter
{
	std::vector<Particle> particles;

	Texture* parTexture = nullptr;
	sf::Vector2f spawnPositionOffset = {0.f, 0.f};
	sf::Vector2f randomSpawnAreaSize = {0.f, 0.f};
	sf::Vector2f parInitialVelocity = {0.f, 1.f};
	sf::Vector2i parSize = {1, 1};
	sf::Color parStartColor = sf::Color::White;
	sf::Color parEndColor = sf::Color::White;
	unsigned amountOfParticles = 1;
	float parWholeLifetime = 1.f;
	float parSpeedScale = 1.f;
	bool isEmitting = true;

	bool wasInitialized = false;
};

// TODO: Particle spawn area size
// TODO: Randomness
// TODO: Collisions with kinematic and static collision bodies

}}
