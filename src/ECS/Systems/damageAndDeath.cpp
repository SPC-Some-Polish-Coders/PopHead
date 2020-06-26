#include "pch.hpp"
#include "damageAndDeath.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"
#include "ECS/entityUtil.hpp"
#include "Scenes/save.hpp"
#include "GUI/gui.hpp"
#include "AI/aiManager.hpp"

namespace ph::system {

DamageAndDeath::DamageAndDeath(entt::registry& registry, AIManager& aiManager)
	:System(registry)
	,mAIManager(aiManager)
{
}

void DamageAndDeath::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	dealDamage();
	makeDamageJuice(dt);
	makeCharactersDie();
	updateDeadCharacters(dt);
}

using namespace component;

void DamageAndDeath::dealDamage() const
{
	mRegistry.view<DamageTag, Health>().each([&]
	(auto entity, auto& damageTag, auto& health)
	{
		health.healthPoints -= damageTag.amountOfDamage;
		if(damageTag.particles)
			mRegistry.assign_or_replace<DamageAnimation>(entity, 0.14f);
		mRegistry.remove<DamageTag>(entity);
	});
}

void DamageAndDeath::makeDamageJuice(float dt) const
{
	mRegistry.view<DamageAnimation, RenderQuad, Health, MultiParticleEmitter, InsideSimRegion>().each([&]
	(auto entity, auto& damageAnimation, auto& renderQuad, auto health, auto& multiParticleEmitter, auto)
	{
		damageAnimation.timeToEndColorChange -= dt;
		
		if(damageAnimation.timeToEndColorChange <= 0.f) 
		{
			renderQuad.color = sf::Color::White;
			mRegistry.remove<DamageAnimation>(entity);
		}
		else if(!damageAnimation.animationStarted) 
		{
			damageAnimation.animationStarted = true;
			renderQuad.color = sf::Color(255, 0, 0);

			ParticleEmitter bloodParEmitter;

			bloodParEmitter.oneShot = true;
			bloodParEmitter.parInitialVelocity = {-60.f, -60.f};
			bloodParEmitter.parInitialVelocityRandom = {60.f, 60.f};
			bloodParEmitter.parSize = {4.f, 4.f};
			bloodParEmitter.parStartColor = sf::Color::Red;
			bloodParEmitter.parEndColor = sf::Color(255, 0, 0, 100);
			bloodParEmitter.spawnPositionOffset = {8.f, 8.f};

			if(health.healthPoints <= 0.f) 
			{
				bloodParEmitter.parWholeLifetime = 0.55f;
				bloodParEmitter.parSize = {10.f, 10.f};
				bloodParEmitter.amountOfParticles = 100;
			}
			else if(health.healthPoints < health.maxHealthPoints * 0.2) 
			{
				bloodParEmitter.parWholeLifetime = 0.35f;
				bloodParEmitter.parSize = {8.f, 8.f};
				bloodParEmitter.amountOfParticles = 50;
			}
			else if(health.healthPoints < health.maxHealthPoints * 0.4) 
			{
				bloodParEmitter.parWholeLifetime = 0.30f;
				bloodParEmitter.parSize = {6.f, 6.f};
				bloodParEmitter.amountOfParticles = 40;
			}
			else if(health.healthPoints < health.maxHealthPoints * 0.6) 
			{
				bloodParEmitter.parWholeLifetime = 0.28f;
				bloodParEmitter.amountOfParticles = 30;
			}
			else if(health.healthPoints < health.maxHealthPoints * 0.8) 
			{
				bloodParEmitter.parWholeLifetime = 0.26f;
				bloodParEmitter.amountOfParticles = 25;
			}
			else 
			{
				bloodParEmitter.parWholeLifetime = 0.24f;
				bloodParEmitter.amountOfParticles = 15;
			}
			
			multiParticleEmitter.particleEmitters.emplace_back(bloodParEmitter);
		}
	});
}

void DamageAndDeath::makeCharactersDie() 
{
	mRegistry.view<Health, InsideSimRegion>().each([&]
	(auto entity, auto health, auto)
	{
		if(health.healthPoints <= 0)
		{
			if(mRegistry.has<Player>(entity))
			{
				auto& playerBody = getPlayerBodyRef();
				playerBody.size = Vec2(20.f); 
				loadGameSave(&mRegistry);
			}
			else
			{
				PH_ASSERT_UNEXPECTED_SITUATION(mRegistry.has<RenderQuad>(entity), "Hurt enemy must have RenderQuad!");
				PH_ASSERT_UNEXPECTED_SITUATION(mRegistry.has<AnimationData>(entity), "Hurt enemy must have AnimationData!");

				mRegistry.assign<DeadCharacter>(entity, 10.f);
				mRegistry.remove<Health>(entity);
				mRegistry.reset<Killable>(entity);
				mRegistry.reset<KinematicCollisionBody>(entity);
				mRegistry.reset<Damage>(entity);
				if(mRegistry.has<Kinematics>(entity)) 
				{
					auto& kin = mRegistry.get<Kinematics>(entity);
					kin.acceleration = Vec2();
					kin.vel *= 0.35f;
				}

				auto& z = mRegistry.get<RenderQuad>(entity).z;
				if(mLastDeadBodyZ == 101)
					mLastDeadBodyZ = 170;
				z = --mLastDeadBodyZ;

				auto& animation = mRegistry.get<AnimationData>(entity);
				animation.currentStateName = "dead";
			}
		}
	});
}

void DamageAndDeath::updateDeadCharacters(float dt)
{
	u32 nrOfDeadCharacters = 0;
	mRegistry.view<DeadCharacter, RenderQuad, InsideSimRegion>().each([&]
	(auto entity, auto& deadCharacter, auto& renderQuad, auto)
	{
		++nrOfDeadCharacters;
		
		// fade out
		deadCharacter.timeToFadeOut -= dt;
		if(deadCharacter.timeToFadeOut < 0.f)
			mRegistry.assign<TaggedToDestroy>(entity);
		auto alpha = Cast<u8>(deadCharacter.timeToFadeOut * 25.5f);
		renderQuad.color = sf::Color(255, 255, 255, alpha);

		// update z component
		deadCharacter.timeFromDeath += dt;
	});

	if(nrOfDeadCharacters == 0)
		mLastDeadBodyZ = 170;
}

}
