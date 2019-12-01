#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <Renderer/texture.hpp>
#include <vector>

namespace ph {

struct Particle
{
	sf::Vector2f position;
	sf::Vector2f velocity;
	float lifetime = 0.f;
};

namespace component {

struct ParticleEmitter
{
	std::vector<Particle> particles;

	const Texture* parTexture = nullptr;
	sf::Vector2f spawnPositionOffset = {0.f, 0.f};
	sf::Vector2f randomSpawnAreaSize = {0.f, 0.f};
	sf::Vector2f parInitialVelocity = {0.f, 1.f};
	sf::Vector2f parAcceleration = {0.f, 0.f};
	sf::Vector2f parSize = {1.f, 1.f};
	sf::Color parStartColor = sf::Color::White;
	sf::Color parEndColor = sf::Color::White;
	unsigned amountOfParticles = 1;
	unsigned amountOfAlreadySpawnParticles = 0;
	float parWholeLifetime = 1.f;
	bool oneShot = false;
	bool isEmitting = true;
	bool wasInitialized = false;
};

}}
