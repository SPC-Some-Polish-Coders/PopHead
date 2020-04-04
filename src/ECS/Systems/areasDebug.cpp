#include "areasDebug.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

void AreasDebug::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sIsCollisionDebugActive)
	{
		// render static collision bodies as dark red rectangle
		auto staticBodies = mRegistry.view<component::StaticCollisionBody, component::BodyRect>();
		staticBodies.each([](const component::StaticCollisionBody, const component::BodyRect body) 
		{
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(130, 0, 0, 140), nullptr,
				body.rect.getTopLeft(), body.rect.getSize(), 50, 0.f, {}, ProjectionType::gameWorld, false);
		});

		// render multi static collision bodies as bright red rectangle
		auto multiStaticBodies = mRegistry.view<component::MultiStaticCollisionBody>();
		multiStaticBodies.each([](const component::MultiStaticCollisionBody& multiCollisionBody) 
		{
			for(auto& bodyRect : multiCollisionBody.rects)
			{
				Renderer::submitQuad(nullptr, nullptr, &sf::Color(255, 0, 0, 140), nullptr,
					bodyRect.getTopLeft(), bodyRect.getSize(), 50, 0.f, {}, ProjectionType::gameWorld, false);
			}
		});

		// render kinematic bodies as blue rectangle
		auto kinematicBodies = mRegistry.view<component::KinematicCollisionBody, component::BodyRect>();
		kinematicBodies.each([](const component::KinematicCollisionBody, const component::BodyRect& body)
		{
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(45, 100, 150, 140), nullptr,
				body.rect.getTopLeft(), body.rect.getSize(), 50, 0.f, {}, ProjectionType::gameWorld, false);
		});
	}

	if(sIsVelocityChangingAreaDebugActive)
	{
		// render velocity changing areas as orange rectangle
		auto velocityChangingAreas = mRegistry.view<component::AreaVelocityChangingEffect, component::BodyRect>();
		velocityChangingAreas.each([](const component::AreaVelocityChangingEffect, const component::BodyRect& body) {
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(255, 165, 0, 140), nullptr,
				body.rect.getTopLeft(), body.rect.getSize(), 50, 0.f, {}, ProjectionType::gameWorld, false);
		});
	}

	if(sIsPushingAreaDebugActive)
	{
		// render pushing areas as yellow rectangle
		auto velocityChangingAreas = mRegistry.view<component::PushingArea, component::BodyRect>();
		velocityChangingAreas.each([](const component::PushingArea, const component::BodyRect& body) {
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(255, 255, 0, 140), nullptr,
				body.rect.getTopLeft(), body.rect.getSize(), 50, 0.f, {}, ProjectionType::gameWorld, false);
		});
	}

	if(sIsLightWallsAreaDebugActive)
	{
		// render light walls as blue rectangle
		auto lightWalls = mRegistry.view<component::LightWall, component::BodyRect>();
		lightWalls.each([](const component::LightWall, const component::BodyRect body) {
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(40, 40, 225, 140), nullptr,
				body.rect.getTopLeft(), body.rect.getSize(), 50, 0.f, {}, ProjectionType::gameWorld, false);
		});

		// render chunk light walls as light blue rectangle
		auto chunks = mRegistry.view<component::RenderChunk>();
		chunks.each([](const component::RenderChunk& chunk) {
			for(const auto wall : chunk.lightWalls)
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(60, 60, 255, 140), nullptr,
				wall.getTopLeft(), wall.getSize(), 50, 0.f, {}, ProjectionType::gameWorld, false);
		});	
	}
}

}
