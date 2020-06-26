#include "pch.hpp"
#include "weatherSystem.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"
#include "ECS/entityUtil.hpp"
#include "Renderer/renderer.hpp"

namespace ph::system {

using namespace component;

static WeatherType currentWeatherType = WeatherType::Sunny;

WeatherSystem::WeatherSystem(entt::registry& registry)
	:System(registry)
{
	currentWeatherType = WeatherType::Sunny;
}

void WeatherSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	bool weatherWasChanged = false;

	mRegistry.view<Player, BodyRect>().each([&]
	(auto, auto playerBody)
	{
		mRegistry.view<WeatherArea, InsideSimRegion, WeatherType, BodyRect>().each([&]
		(auto, auto, auto weatherAreaType, auto weatherAreaBody)
		{
			if(intersect(weatherAreaBody, playerBody))
			{
				if(currentWeatherType != weatherAreaType)
				{
					weatherWasChanged = true;
					currentWeatherType = weatherAreaType;
				}
			}
		});
	});

	auto destroyWeatherEntity = [this] 
	{
		auto view = mRegistry.view<Weather>();
		mRegistry.destroy(view.begin(), view.end());
	};

	if(weatherWasChanged)
	{
		switch(currentWeatherType)
		{
			case WeatherType::Sunny: 
			{
				destroyWeatherEntity();
				Renderer::setAmbientLightColor(sf::Color(255, 245, 225));
			} break;

			case WeatherType::Cave:
			{
				destroyWeatherEntity();
				Renderer::setAmbientLightColor(sf::Color(2, 1, 1));
			} break;

			case WeatherType::DrizzleRain: 
			case WeatherType::NormalRain: 
			case WeatherType::HeavyRain: 
			{
				// initialize rain
				destroyWeatherEntity();

				Vec2 rainInitVel = {};
				Vec2 rainInitVelRandom = {};
				Vec2 rainAcceleration = {};
				u32 rainDropsAmount = {};

				auto createRain = [&]
				{
					auto entity = mRegistry.create();

					mRegistry.assign<Weather>(entity);
					mRegistry.assign<BodyRect>(entity);
					auto& emi = mRegistry.assign<ParticleEmitter>(entity);
					
					emi.spawnPositionOffset = {-390.f, -250.f};
					emi.randomSpawnAreaSize = {800.f, 20.f};
					emi.parInitialVelocity = rainInitVel;
					emi.parInitialVelocityRandom = rainInitVelRandom; 
					emi.parAcceleration = rainAcceleration;
					emi.parSize = {2.f, 4.f};
					emi.parStartColor = sf::Color(0, 0, 255, 230);
					emi.parEndColor = sf::Color(0, 0, 255, 230);
					emi.amountOfParticles = rainDropsAmount;
					emi.parWholeLifetime = 1.f;
					emi.parZ = 70;
				};

				switch(currentWeatherType)
				{
					case WeatherType::DrizzleRain: 
					{
						Renderer::setAmbientLightColor(sf::Color(75, 75, 75));
						rainInitVel = {0.f, 300.f};
						rainInitVelRandom = {0.f, 400.f};
						rainAcceleration = {0.f, 1.f};
						rainDropsAmount = 20;
						createRain();
					} break;

					case WeatherType::NormalRain: 
					{
						Renderer::setAmbientLightColor(sf::Color(20, 20, 30));
						rainInitVel = {0.f, 300.f};
						rainInitVelRandom = {0.f, 500.f};
						rainAcceleration = {0.f, 1.5f};
						rainDropsAmount = 85;
						createRain();
					} break;

					case WeatherType::HeavyRain: 
					{
						Renderer::setAmbientLightColor(sf::Color(9, 9, 16));
						rainInitVel = {0.f, 400.f};
						rainInitVelRandom = {0.f, 500.f};
						rainAcceleration = {0.f, 2.f};
						rainDropsAmount = 150;
						createRain();
					} break;
				}
			} break;
		}
	}

	if(currentWeatherType == WeatherType::DrizzleRain ||
	   currentWeatherType == WeatherType::NormalRain ||
	   currentWeatherType == WeatherType::HeavyRain)
	{
		// set weather position to player position
		mRegistry.view<Weather, BodyRect>().each([]
		(auto, auto& weatherBody) 
		{
			if(isPlayerAlive())
				weatherBody.pos = getPlayerPos();
		});	
	}
}

}
