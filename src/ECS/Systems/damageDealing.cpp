#include "damageDealing.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace {
	constexpr float colorChangeTime = 0.14f;
	constexpr float particlesTime = 0.4f;
}

namespace ph::system {

	void DamageDealing::update(float seconds)
	{
		dealDamage();
		playDamageAnimation(seconds);
	}

	void DamageDealing::dealDamage() const
	{
		auto view = mRegistry.view<component::DamageTag, component::Health>();
		for (auto entity : view)
		{
			auto& [damageTag, health] = view.get<component::DamageTag, component::Health>(entity);
			health.healthPoints -= damageTag.amountOfDamage;
			mRegistry.remove<component::DamageTag>(entity);
			mRegistry.assign_or_replace<component::DamageAnimation>(entity, colorChangeTime, particlesTime);
		}
	}

	void DamageDealing::playDamageAnimation(float seconds) const
	{
		// TODO: Make character blast just after it was hit

		auto view = mRegistry.view<component::DamageAnimation, component::Color, component::Health>();
		for(auto entity : view)
		{
			auto& [damageAnimation, color, health] = view.get<component::DamageAnimation, component::Color, component::Health>(entity);
			
			damageAnimation.timeToEndColorChange -= seconds;
			damageAnimation.particlesTime -= seconds;
			
			if(damageAnimation.timeToEndColorChange <= 0.f)
				color.color = sf::Color::White;
			else
				color.color = sf::Color(255, 0, 0);

			if(damageAnimation.particlesTime <= 0.f) {
				mRegistry.remove<component::ParticleEmitter>(entity);
				mRegistry.remove<component::DamageAnimation>(entity);
			}
			else if(!mRegistry.has<component::ParticleEmitter>(entity)) {
				//component::ParticleEmitter spiritParEmitter;
				//spiritParEmitter.oneShot = true;
				//spiritParEmitter.amountOfParticles = 50;
				//spiritParEmitter.parInitialVelocity = {0.f, -1.8f};
				//spiritParEmitter.parSize = {3.f, 3.f};
				//spiritParEmitter.parStartColor = sf::Color::White;
				//spiritParEmitter.parEndColor = sf::Color::Black;
				//spiritParEmitter.parWholeLifetime = 0.5f;
				//spiritParEmitter.spawnPositionOffset = {0.f, 0.f};
				//spiritParEmitter.randomSpawnAreaSize = {13.f, 5.f};
				//mRegistry.assign<component::ParticleEmitter>(entity, spiritParEmitter);

				component::ParticleEmitter bloodParEmitter;

				bloodParEmitter.oneShot = true;
				bloodParEmitter.parInitialVelocity = {-1.f, -1.f};
				bloodParEmitter.parInitialVelocityRandom = {1.f, 1.f};
				bloodParEmitter.parSize = {4.f, 4.f};
				bloodParEmitter.parStartColor = sf::Color::Red;
				bloodParEmitter.parEndColor = sf::Color(255, 0, 0, 100);
				bloodParEmitter.spawnPositionOffset = {8.f, 8.f};

				if(health.healthPoints <= 0.f) {
					bloodParEmitter.parWholeLifetime = 0.6f;
					bloodParEmitter.parSize = {11.f, 11.f};
					bloodParEmitter.amountOfParticles = 100;
				}
				else if(health.healthPoints < health.maxHealthPoints * 0.2) {
					bloodParEmitter.parWholeLifetime = 0.35f;
					bloodParEmitter.parSize = {8.f, 8.f};
					bloodParEmitter.amountOfParticles = 50;
				}
				else if(health.healthPoints < health.maxHealthPoints * 0.4) {
					bloodParEmitter.parWholeLifetime = 0.30f;
					bloodParEmitter.parSize = {6.f, 6.f};
					bloodParEmitter.amountOfParticles = 40;
				}
				else if(health.healthPoints < health.maxHealthPoints * 0.6) {
					bloodParEmitter.parWholeLifetime = 0.28f;
					bloodParEmitter.amountOfParticles = 30;
				}
				else if(health.healthPoints < health.maxHealthPoints * 0.8) {
					bloodParEmitter.parWholeLifetime = 0.26f;
					bloodParEmitter.amountOfParticles = 25;
				}
				else {
					bloodParEmitter.parWholeLifetime = 0.24f;
					bloodParEmitter.amountOfParticles = 15;
				}

				mRegistry.assign<component::ParticleEmitter>(entity, bloodParEmitter);
			}
		}
	}
}
