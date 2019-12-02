#include "damageDealing.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace {
	constexpr float colorChangeTime = 0.14f;
	constexpr float wholeDamageAnimationTime = 0.5f;
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
			mRegistry.assign_or_replace<component::DamageAnimation>(entity, colorChangeTime, wholeDamageAnimationTime);
		}
	}

	void DamageDealing::playDamageAnimation(float seconds) const
	{
		// TODO: Make character blast just after it was hit

		auto view = mRegistry.view<component::DamageAnimation, component::Color>();
		for(auto entity : view)
		{
			auto& [damageAnimation, color] = view.get<component::DamageAnimation, component::Color>(entity);
			
			damageAnimation.timeToEndColorChange -= seconds;
			damageAnimation.wholeDamageAnimationTime -= seconds;
			
			if(damageAnimation.timeToEndColorChange <= 0.f)
				color.color = sf::Color::White;
			else
				color.color = sf::Color(255, 0, 0);

			if(damageAnimation.wholeDamageAnimationTime <= 0.f) {
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
				bloodParEmitter.amountOfParticles = 20;
				bloodParEmitter.parInitialVelocity = {-1.f, -1.f};
				bloodParEmitter.parInitialVelocityRandom = {1.f, 1.f};
				bloodParEmitter.parSize = {3.f, 3.f};
				bloodParEmitter.parStartColor = sf::Color::Red;
				bloodParEmitter.parEndColor = sf::Color::Red;
				bloodParEmitter.parWholeLifetime = 0.2f;
				bloodParEmitter.spawnPositionOffset = {8.f, 8.f};
				mRegistry.assign<component::ParticleEmitter>(entity, bloodParEmitter);
			}
		}
	}
}
