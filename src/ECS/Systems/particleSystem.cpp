#include "pch.hpp"
#include "particleSystem.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Utilities/random.hpp"
#include "Renderer/renderer.hpp"

namespace ph::system {

using namespace component;

static void updateParticleEmitter(float dt, ParticleEmitter& emi, const BodyRect& body,
                                  float brightness, float alpha, bool pause)
{
	if(!emi.isEmitting) return;

	if(!pause)
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
			auto addParticle = [](ParticleEmitter& emi, const BodyRect& body) 
			{
				Particle particle;

				particle.position = body.pos + emi.spawnPositionOffset;

				if(emi.randomSpawnAreaSize != Vec2())
					particle.position += Random::generateVector({0.f, 0.f}, emi.randomSpawnAreaSize);

				if(emi.parInitialVelocity == emi.parInitialVelocityRandom)
					particle.velocity = emi.parInitialVelocity;
				else
					particle.velocity = Random::generateVector(emi.parInitialVelocity, emi.parInitialVelocityRandom);

				emi.particles.emplace_back(particle);
			};

			if(emi.oneShot || Cast<float>(emi.amountOfParticles) > emi.parWholeLifetime * 60.f)
			{
				float nrOfParticlesPerFrame = Cast<float>(emi.amountOfParticles / Cast<u32>(emi.parWholeLifetime * 60.f));
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
			color.r = (Cast<u8>((Cast<float>(emi.parStartColor.r) * startColorMultiplier + Cast<float>(emi.parEndColor.r) * endColorMultiplier) * brightness));
			color.g = (Cast<u8>((Cast<float>(emi.parStartColor.g) * startColorMultiplier + Cast<float>(emi.parEndColor.g) * endColorMultiplier) * brightness));
			color.b = (Cast<u8>((Cast<float>(emi.parStartColor.b) * startColorMultiplier + Cast<float>(emi.parEndColor.b) * endColorMultiplier) * brightness));
			color.a = (Cast<u8>((Cast<float>(emi.parStartColor.a) * startColorMultiplier + Cast<float>(emi.parEndColor.a) * endColorMultiplier) * alpha));
		}

		// submit particle to renderer
		if(emi.parTexture)
			Renderer::submitQuad(emi.parTexture, Null, &color, Null, particle.position, emi.parSize, emi.parZ, 0.f, {});
		else if(emi.parSize.x != emi.parSize.y)
			Renderer::submitQuad(Null, Null, &color, Null, particle.position, emi.parSize, emi.parZ, 0.f, {});
		else
			Renderer::submitPoint(particle.position, color, emi.parSize.x);
	}
}

void PatricleSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	auto updateMultiParticleEmitter = [&]
	(MultiParticleEmitter& multiEmi, BodyRect body, float brightness, float alpha)
	{
		// update particle emitters 
		for(auto& particleEmitter : multiEmi.particleEmitters)
			updateParticleEmitter(dt, particleEmitter, body, brightness, alpha, sPause);

		// erase dead particle emitters from multi particle emitter
		for(auto it = multiEmi.particleEmitters.begin(); it != multiEmi.particleEmitters.end();) 
		{
			if(it->oneShot && it->particles.empty())
				it = multiEmi.particleEmitters.erase(it);
			else
				++it;
		}
	};

	// update multi particle emitters with indoor outdoor blend
	mRegistry.view<MultiParticleEmitter, InsideSimRegion, BodyRect, IndoorOutdoorBlend>().each([&]
	(auto& multiEmi, auto, auto body, auto io)
	{
		updateMultiParticleEmitter(multiEmi, body, io.brightness, io.alpha);
	});

	// update multi particle emitters without indoor outdoor blend
	mRegistry.view<MultiParticleEmitter, InsideSimRegion, BodyRect, IndoorOutdoorBlend>().each([&]
	(auto& multiEmi, auto, auto body, auto io)
	{
		updateMultiParticleEmitter(multiEmi, body, 1, 1);
	});

	// update single particle emitters with indoor outdoor blend
	mRegistry.view<ParticleEmitter, InsideSimRegion, BodyRect, IndoorOutdoorBlend>().each([&]
	(auto& emi, auto, auto body, auto io)
	{
		updateParticleEmitter(dt, emi, body, io.brightness, io.alpha, sPause);
	});

	// update single particle emitters without indoor outdoor blend
	mRegistry.view<ParticleEmitter, InsideSimRegion, BodyRect>(entt::exclude<IndoorOutdoorBlend>).each([&]
	(auto& emi, auto, auto body)
	{
		updateParticleEmitter(dt, emi, body, 1, 1, sPause);
	});
}

}
