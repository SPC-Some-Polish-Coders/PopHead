#include "damageAndDeath.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "GUI/gui.hpp"
#include "Logs/logs.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

	DamageAndDeath::DamageAndDeath(entt::registry& registry, GUI& gui)
		:System(registry)
		,mGui(gui)
	{
	}

	void DamageAndDeath::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		dealDamage();
		makeDamageJuice(dt);
		makeCharactersDie();
		makeCorpsesFadeOut(dt);
	}

	void DamageAndDeath::dealDamage() const
	{
		auto view = mRegistry.view<component::DamageTag, component::Health>();
		for (auto entity : view)
		{
			auto& [damageTag, health] = view.get<component::DamageTag, component::Health>(entity);
			health.healthPoints -= damageTag.amountOfDamage;
			mRegistry.remove<component::DamageTag>(entity);
			mRegistry.assign_or_replace<component::DamageAnimation>(entity, 0.14f);
		}
	}

	void DamageAndDeath::makeDamageJuice(float dt) const
	{
		auto view = mRegistry.view<component::DamageAnimation, component::RenderQuad, component::Health, component::MultiParticleEmitter>();
		for(auto entity : view)
		{
			auto& [damageAnimation, renderQuad, multiParticleEmitter] =
				view.get<component::DamageAnimation, component::RenderQuad, component::MultiParticleEmitter>(entity);
			const auto& health = view.get<component::Health>(entity);
			
			damageAnimation.timeToEndColorChange -= dt;
			
			if(damageAnimation.timeToEndColorChange <= 0.f) {
				renderQuad.color = sf::Color::White;
				mRegistry.remove<component::DamageAnimation>(entity);
			}
			else if(!damageAnimation.animationStarted) {
				damageAnimation.animationStarted = true;
				renderQuad.color = sf::Color(255, 0, 0);

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

	void DamageAndDeath::makeCharactersDie() const
	{
		auto view = mRegistry.view<component::Health>();
		for(auto entity : view)
		{
			const auto& health = view.get(entity);
			if(health.healthPoints <= 0)
			{
				PH_ASSERT_UNEXPECTED_SITUATION(mRegistry.has<component::RenderQuad>(entity), "Hurt enemy must have RenderQuad!");
				PH_ASSERT_UNEXPECTED_SITUATION(mRegistry.has<component::AnimationData>(entity), "Hurt enemy must have AnimationData!");

				mRegistry.assign<component::TimeToFadeOut>(entity);
				mRegistry.remove<component::Health>(entity);
				mRegistry.reset<component::Killable>(entity);
				mRegistry.reset<component::KinematicCollisionBody>(entity);
				mRegistry.reset<component::Damage>(entity);
				if(mRegistry.has<component::PushingForces>(entity)) {
					auto& pushingVelocity = mRegistry.get<component::PushingForces>(entity);
					pushingVelocity.vel = pushingVelocity.vel * 0.35f;
				}

				bool isPlayer = mRegistry.has<component::Player>(entity);
				auto& z = mRegistry.get<component::RenderQuad>(entity).z;
				z = isPlayer ? 96 : 97;
				if(isPlayer) {
					auto deathCameraEntity = mRegistry.create();
					mRegistry.remove<component::GunAttacker>(entity);
					mRegistry.remove<component::FaceDirection>(entity);
					component::Camera camera;
					camera.camera = mRegistry.get<component::Camera>(entity).camera;
					camera.priority = 2;
					mRegistry.assign<component::Camera>(deathCameraEntity, camera);
					mGui.showInterface("gameOverScreen");
				}

				auto& animation = mRegistry.get<component::AnimationData>(entity);
				animation.currentStateName = "dead";
			}
		}
	}

	void DamageAndDeath::makeCorpsesFadeOut(float dt) const
	{
		auto view = mRegistry.view<component::TimeToFadeOut, component::RenderQuad>();
		for(auto entity : view)
		{
			auto& [timeToFadeOut, renderQuad] = view.get<component::TimeToFadeOut, component::RenderQuad>(entity);
			timeToFadeOut.seconds += dt;
			if(timeToFadeOut.seconds > 10.f)
				mRegistry.assign<component::TaggedToDestroy>(entity);
			auto alpha = static_cast<unsigned char>(255.f - (timeToFadeOut.seconds * 25.5f));
			renderQuad.color = sf::Color(255, 255, 255, alpha);
		}
	}
}
