#include "particleSystem.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/random.hpp"
#include "Renderer/renderer.hpp"
#include <cmath>

namespace ph::system {

void PatricleSystem::update(float dt)
{
	updateSingleParticleEmitters(dt);
	updateMultiParticleEmitters(dt);
}

void PatricleSystem::updateSingleParticleEmitters(const float dt) const
{
	auto view = mRegistry.view<component::ParticleEmitter, component::BodyRect>();
	view.each([dt, this](component::ParticleEmitter& emi, const component::BodyRect& body)
	{
		updateParticleEmitter(dt, emi, body);
	});
}

void PatricleSystem::updateMultiParticleEmitters(const float dt) const
{
	auto view = mRegistry.view<component::MultiParticleEmitter, component::BodyRect>();
	view.each([dt, this](component::MultiParticleEmitter& multiEmi, const component::BodyRect& body)
	{
		// update particle emitters 
		for(auto& particleEmitter : multiEmi.particleEmitters)
			updateParticleEmitter(dt, particleEmitter, body);

		// erase dead particle emitters from multi particle emitter
		for(auto it = multiEmi.particleEmitters.begin(); it != multiEmi.particleEmitters.end();) {
			if(it->oneShot && it->particles.empty())
				it = multiEmi.particleEmitters.erase(it);
			else
				++it;
		}
	});
}

void PatricleSystem::updateParticleEmitter(const float dt, component::ParticleEmitter& emi, const component::BodyRect& body) const
{
	// exit if is not emitting
	if(!emi.isEmitting)
		return;

	// alocate particles
	if(!emi.wasInitialized) {
		emi.particles.reserve(emi.amountOfParticles);
		emi.wasInitialized = true;
	}

	// erase particles
	while(!emi.particles.empty() && emi.particles.front().lifetime >= emi.parWholeLifetime)
		emi.particles.erase(emi.particles.begin());

	// add particles
	if(!emi.oneShot || emi.amountOfAlreadySpawnParticles < emi.amountOfParticles)
	{
		auto addParticle = [](component::ParticleEmitter& emi, const component::BodyRect& body) 
		{
			Particle particle;

			particle.position = body.rect.getTopLeft() + emi.spawnPositionOffset;

			if(emi.randomSpawnAreaSize != sf::Vector2f(0.f, 0.f))
				particle.position += Random::generateVector({0.f, 0.f}, emi.randomSpawnAreaSize);

			if(emi.parInitialVelocity == emi.parInitialVelocityRandom)
				particle.velocity = emi.parInitialVelocity;
			else
				particle.velocity = Random::generateVector(emi.parInitialVelocity, emi.parInitialVelocityRandom);
				
			emi.particles.emplace_back(particle);
		};

		if(emi.oneShot || static_cast<float>(emi.amountOfParticles) > emi.parWholeLifetime * 60.f)
		{
			float nrOfParticlesPerFrame = float(emi.amountOfParticles / unsigned(emi.parWholeLifetime * 60.f));
			unsigned nrOfParticlesAddedInThisFrame = 0;
			while((emi.particles.size() < emi.amountOfParticles) && 
				    (emi.oneShot || nrOfParticlesAddedInThisFrame < nrOfParticlesPerFrame))
			{
				++nrOfParticlesAddedInThisFrame;
				addParticle(emi, body);
			}
			emi.amountOfAlreadySpawnParticles += nrOfParticlesAddedInThisFrame;
		}
		else if((emi.particles.size() < emi.amountOfParticles) && 
			(emi.particles.empty() || emi.particles.back().lifetime > emi.parWholeLifetime / emi.amountOfParticles))
		{
			addParticle(emi, body);
			++emi.amountOfAlreadySpawnParticles;
		}
	}

	for(auto& particle : emi.particles)
	{
		// update particle
		particle.lifetime += dt;
		particle.velocity += emi.parAcceleration * dt;
		particle.position += particle.velocity;

		// compute current particle color
		sf::Color color = emi.parStartColor;
		if(emi.parStartColor != emi.parEndColor) {
			const float startColorMultiplier = (emi.parWholeLifetime - particle.lifetime) / emi.parWholeLifetime;
			const float endColorMultiplier = particle.lifetime / emi.parWholeLifetime;
			color.r = unsigned char(float(emi.parStartColor.r) * startColorMultiplier + float(emi.parEndColor.r) * endColorMultiplier);
			color.g = unsigned char(float(emi.parStartColor.g) * startColorMultiplier + float(emi.parEndColor.g) * endColorMultiplier);
			color.b = unsigned char(float(emi.parStartColor.b) * startColorMultiplier + float(emi.parEndColor.b) * endColorMultiplier);
			color.a = unsigned char(float(emi.parStartColor.a) * startColorMultiplier + float(emi.parEndColor.a) * endColorMultiplier);
		}

		// submit particle to renderer
		if(emi.parTexture)
			Renderer::submitQuad(emi.parTexture, nullptr, &color, nullptr, particle.position, emi.parSize, 0, 0.f, {}, false);
		else if(emi.parSize.x != emi.parSize.y)
			Renderer::submitQuad(nullptr, nullptr, &color, nullptr, particle.position, emi.parSize, 0, 0.f, {}, false);
		else
			Renderer::submitPoint(particle.position, color, 0, emi.parSize.x);
	}
}

}
