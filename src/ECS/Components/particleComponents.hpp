#pragma once

#include "Renderer/API/texture.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>

namespace ph {

struct Particle
{
	Vec2 position;
	Vec2 velocity;
	float lifetime = 0.f;
};

namespace component {

struct ParticleEmitter
{
	std::vector<Particle> particles;

	Texture* parTexture = Null;
	Vec2 spawnPositionOffset = {0.f, 0.f};
	Vec2 randomSpawnAreaSize = {0.f, 0.f};
	Vec2 parInitialVelocity = {0.f, 1.f};
	Vec2 parInitialVelocityRandom = {0.f, 1.f};
	Vec2 parAcceleration = {0.f, 0.f};
	Vec2 parSize = {1.f, 1.f};
	sf::Color parStartColor = sf::Color::White;
	sf::Color parEndColor = sf::Color::White;
	u32 amountOfParticles = 1;
	u32 amountOfAlreadySpawnParticles = 0;
	float parWholeLifetime = 1.f;
	u8 parZ = 90;
	bool oneShot = false;
	bool isEmitting = true;
	bool wasInitialized = false;
};

struct MultiParticleEmitter
{
	std::vector<ParticleEmitter> particleEmitters;
};

}}
