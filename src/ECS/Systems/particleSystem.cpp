#include "pch.hpp"
#include "particleSystem.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/random.hpp"
#include "Renderer/renderer.hpp"

namespace ph::system {

void PatricleSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	updateSingleParticleEmitters(dt);
	updateMultiParticleEmitters(dt);
}

void PatricleSystem::updateSingleParticleEmitters(float dt) const
{
	mRegistry.view<component::ParticleEmitter, component::BodyRect>().each([dt, this]
	(auto& emi, const auto& body)
	{
		updateParticleEmitter(dt, emi, body);
	});
}

void PatricleSystem::updateMultiParticleEmitters(float dt) const
{
	mRegistry.view<component::MultiParticleEmitter, component::BodyRect>().each([dt, this]
	(auto& multiEmi, const auto& body)
	{
		// update particle emitters 
		for(auto& particleEmitter : multiEmi.particleEmitters)
			updateParticleEmitter(dt, particleEmitter, body);

		// erase dead particle emitters from multi particle emitter
		for(auto it = multiEmi.particleEmitters.begin(); it != multiEmi.particleEmitters.end();) 
		{
			if(it->oneShot && it->particles.empty())
				it = multiEmi.particleEmitters.erase(it);
			else
				++it;
		}
	});
}

void PatricleSystem::updateParticleEmitter(float dt, component::ParticleEmitter& emi, const component::BodyRect& body) const
{
	// exit if is not emitting
	if(!emi.isEmitting)
		return;

	if(!sPause)
	{
		// alocate particles
		if(!emi.wasInitialized) 
		{
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

				particle.position = body.pos + emi.spawnPositionOffset;

				if(emi.randomSpawnAreaSize != Vec2(0.f, 0.f))
					particle.position += Random::generateVector({0.f, 0.f}, emi.randomSpawnAreaSize);

				if(emi.parInitialVelocity == emi.parInitialVelocityRandom)
					particle.velocity = emi.parInitialVelocity;
				else
					particle.velocity = Random::generateVector(emi.parInitialVelocity, emi.parInitialVelocityRandom);
					
				emi.particles.emplace_back(particle);
			};

			if(emi.oneShot || Cast<float>(emi.amountOfParticles) > emi.parWholeLifetime * 60.f)
			{
				float nrOfParticlesPerFrame = float(emi.amountOfParticles / u32(emi.parWholeLifetime * 60.f));
				u32 nrOfParticlesAddedInThisFrame = 0;
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

		// update particles
		for(auto& particle : emi.particles)
		{
			particle.lifetime += dt;
			particle.velocity += emi.parAcceleration * dt;
			particle.position += particle.velocity * dt;
		}
	}

	for(auto& particle : emi.particles)
	{
		// compute current particle color
		sf::Color color = emi.parStartColor;
		if(emi.parStartColor != emi.parEndColor) 
		{
			float startColorMultiplier = (emi.parWholeLifetime - particle.lifetime) / emi.parWholeLifetime;
			float endColorMultiplier = particle.lifetime / emi.parWholeLifetime;
			color.r = u8(float(emi.parStartColor.r) * startColorMultiplier + float(emi.parEndColor.r) * endColorMultiplier);
			color.g = u8(float(emi.parStartColor.g) * startColorMultiplier + float(emi.parEndColor.g) * endColorMultiplier);
			color.b = u8(float(emi.parStartColor.b) * startColorMultiplier + float(emi.parEndColor.b) * endColorMultiplier);
			color.a = u8(float(emi.parStartColor.a) * startColorMultiplier + float(emi.parEndColor.a) * endColorMultiplier);
		}

		// submit particle to renderer
		if(emi.parTexture)
			Renderer::submitQuad(emi.parTexture, Null, &color, Null, particle.position, emi.parSize, emi.parZ, 0.f, {});
		else if(emi.parSize.x != emi.parSize.y)
			Renderer::submitQuad(Null, Null, &color, Null, particle.position, emi.parSize, emi.parZ, 0.f, {});
		else
			Renderer::submitPoint(particle.position, color, emi.parZ, emi.parSize.x);
	}
}

}
