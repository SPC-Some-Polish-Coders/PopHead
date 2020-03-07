#include "weather.hpp"
#include "Utilities/profiling.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Renderer/renderer.hpp"

namespace ph::system {

Weather::Weather(entt::registry& registry)
	:System(registry)
{
	sMode = Sunny;
	sRain = Rain();
}

void Weather::update(float dt)
{
	PH_PROFILE_FUNCTION(0);

	auto destroyWeatherEntity = [this] {
		auto view = mRegistry.view<component::Weather>();
		mRegistry.destroy(view.begin(), view.end());
	};

	switch(sMode)
	{
		case Sunny: {
			if(!sRain.needsInitialization)
			{
				destroyWeatherEntity();
				sRain.needsInitialization = true;
				Renderer::setAmbientLightColor(sf::Color(255, 245, 225));
			}
		} break;

		case Rainy: {

			// initialize rain
			if(sRain.needsInitialization) 
			{
				sRain.needsInitialization = false;
				destroyWeatherEntity();

				sf::Vector2f rainInitVel = {};
				sf::Vector2f rainInitVelRandom = {};
				sf::Vector2f rainAcceleration = {};
				unsigned rainDropsAmount = {};

				auto createRain = [&]
				{
					auto entity = mRegistry.create();
					mRegistry.assign<component::Weather>(entity);
					mRegistry.assign<component::BodyRect>(entity);
					auto& emi = mRegistry.assign<component::ParticleEmitter>(entity);
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

				switch(sRain.type)
				{
					case Rain::Drizzle: {
						Renderer::setAmbientLightColor(sf::Color(210, 200, 200));
						rainInitVel = {0.f, 300.f};
						rainInitVelRandom = {0.f, 400.f};
						rainAcceleration = {0.f, 1.f};
						rainDropsAmount = 20;
						createRain();
					} break;

					case Rain::Normal: {
						Renderer::setAmbientLightColor(sf::Color(100, 95, 95));
						rainInitVel = {0.f, 300.f};
						rainInitVelRandom = {0.f, 500.f};
						rainAcceleration = {0.f, 1.5f};
						rainDropsAmount = 85;
						createRain();
					} break;

					case Rain::Heavy: {
						Renderer::setAmbientLightColor(sf::Color(50, 40, 40));
						rainInitVel = {0.f, 400.f};
						rainInitVelRandom = {0.f, 500.f};
						rainAcceleration = {0.f, 2.f};
						rainDropsAmount = 150;
						createRain();
					} break;
				}
			}

			// get player position
			sf::Vector2f playerPos;
			auto players = mRegistry.view<component::Player, component::BodyRect>();
			players.each([&playerPos](const component::Player, const component::BodyRect& body) {
				playerPos = body.rect.getTopLeft();
			});

			// set weather position to player position
			auto weatherView = mRegistry.view<component::Weather, component::BodyRect>();
			weatherView.each([playerPos](const component::Weather, component::BodyRect& weatherBody) {
				weatherBody.rect.setPosition(playerPos);
			});	

		} break;
	}
}

void Weather::setRainType(Rain::Type type)
{
	if(type != sRain.type) {
		sRain.type = type;
		sRain.needsInitialization = true;	
	}
}

}
