#include "damageDealing.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Utilities/debug.hpp"

namespace {
	constexpr float colorChangeTime = 0.14f;
}

namespace ph::system {

	void DamageDealing::update(float seconds)
	{
		dealDamage();
		makeDamageJuice(seconds);
	}

	void DamageDealing::dealDamage() const
	{
		auto view = mRegistry.view<component::DamageTag, component::Health>();
		for (auto entity : view)
		{
			auto& [damageTag, health] = view.get<component::DamageTag, component::Health>(entity);
			health.healthPoints -= damageTag.amountOfDamage;
			mRegistry.remove<component::DamageTag>(entity);
			mRegistry.assign_or_replace<component::DamageAnimation>(entity, colorChangeTime);
		}
	}

	void DamageDealing::makeDamageJuice(float seconds) const
	{
		// NOTE: Juice is game design term

		// TODO: Make this assert work
		/*PH_ASSERT(mRegistry.has<component::MultiParticleEmitter>(entity),
			"This Entity should have MultiParticleEmitter in order to emit damage particles!");*/

		auto view = mRegistry.view<component::DamageAnimation, component::Color, component::Health, component::MultiParticleEmitter>();
		for(auto entity : view)
		{
			auto& [damageAnimation, color, health, multiParticleEmitter] =
				view.get<component::DamageAnimation, component::Color, component::Health, component::MultiParticleEmitter>(entity);
			
			damageAnimation.timeToEndColorChange -= seconds;
			
			if(damageAnimation.timeToEndColorChange <= 0.f) {
				color.color = sf::Color::White;
				mRegistry.remove<component::DamageAnimation>(entity);
			}
			else if(!damageAnimation.wasAnimationStarted) {
				damageAnimation.wasAnimationStarted = true;
				color.color = sf::Color(255, 0, 0);

				component::ParticleEmitter bloodParEmitter;

				bloodParEmitter.oneShot = true;
				bloodParEmitter.parInitialVelocity = {-1.f, -1.f};
				bloodParEmitter.parInitialVelocityRandom = {1.f, 1.f};
				bloodParEmitter.parSize = {4.f, 4.f};
				bloodParEmitter.parStartColor = sf::Color::Red;
				bloodParEmitter.parEndColor = sf::Color(255, 0, 0, 100);
				bloodParEmitter.spawnPositionOffset = {8.f, 8.f};

				if(health.healthPoints <= 0.f) {
					bloodParEmitter.parWholeLifetime = 0.55f;
					bloodParEmitter.parSize = {10.f, 10.f};
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
				
				multiParticleEmitter.particleEmitters.emplace_back(bloodParEmitter);
			}
		}
	}
}


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
