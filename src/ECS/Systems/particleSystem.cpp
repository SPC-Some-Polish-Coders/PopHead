#include "particleSystem.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/random.hpp"
#include "Renderer/renderer.hpp"

namespace ph::system {

void PatricleSystem::update(float seconds)
{
	auto view = mRegistry.view<component::ParticleEmitter, component::BodyRect>();
	view.each([seconds](component::ParticleEmitter& emi, const component::BodyRect& body)
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
		if(!emi.particles.empty() && emi.particles.front().lifetime >= emi.parWholeLifetime)
			emi.particles.erase(emi.particles.begin());

		// add particles
		if((emi.particles.size() < emi.amountOfParticles) &&
		   (emi.particles.empty() || emi.particles.back().lifetime > emi.parWholeLifetime / emi.amountOfParticles))
		{
			Particle particle;
			particle.position = body.rect.getTopLeft() + emi.spawnPositionOffset;
			if(emi.randomSpawnAreaSize != sf::Vector2f(0.f, 0.f)) {
				const sf::Vector2f randomOffset(
					Random::generateNumber(0.f, emi.randomSpawnAreaSize.x),
					Random::generateNumber(0.f, emi.randomSpawnAreaSize.y)
				);
				particle.position += randomOffset;
			}
			emi.particles.emplace_back(particle);
		}

		for(auto& particle : emi.particles)
		{
			// update particle
			particle.lifetime += seconds;
			particle.position += emi.parInitialVelocity * seconds; // TODO: Add acceleration

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
			if(!emi.parTexture && emi.parSize.x == emi.parSize.y)
				Renderer::submitPoint(particle.position, color, 0.f, emi.parSize.x);
		}
	});
}

}
