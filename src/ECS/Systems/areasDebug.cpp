#include "areasDebug.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
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
				body.rect.getTopLeft(), body.rect.getSize(), 50, 0.f, {});
		});

		// render multi static collision bodies as bright red rectangle
		auto multiStaticBodies = mRegistry.view<component::MultiStaticCollisionBody>();
		multiStaticBodies.each([](const component::MultiStaticCollisionBody& multiCollisionBody) 
		{
			for(auto& bodyRect : multiCollisionBody.rects)
			{
				Renderer::submitQuad(nullptr, nullptr, &sf::Color(255, 0, 0, 140), nullptr,
					bodyRect.getTopLeft(), bodyRect.getSize(), 50, 0.f, {});
			}
		});

		// render kinematic bodies as blue rectangle
		auto kinematicBodies = mRegistry.view<component::KinematicCollisionBody, component::BodyRect>();
		kinematicBodies.each([](const component::KinematicCollisionBody, const component::BodyRect& body)
		{
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(45, 100, 150, 140), nullptr,
				body.rect.getTopLeft(), body.rect.getSize(), 50, 0.f, {});
		});
	}

	if(sIsVelocityChangingAreaDebugActive)
	{
		// render velocity changing areas as orange rectangle
		auto velocityChangingAreas = mRegistry.view<component::AreaVelocityChangingEffect, component::BodyRect>();
		velocityChangingAreas.each([](const component::AreaVelocityChangingEffect, const component::BodyRect& body) {
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(255, 165, 0, 140), nullptr,
				body.rect.getTopLeft(), body.rect.getSize(), 50, 0.f, {});
		});
	}

	if(sIsPushingAreaDebugActive)
	{
		// render pushing areas as yellow rectangle
		auto velocityChangingAreas = mRegistry.view<component::PushingArea, component::BodyRect>();
		velocityChangingAreas.each([](const component::PushingArea, const component::BodyRect& body) {
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(255, 255, 0, 140), nullptr,
				body.rect.getTopLeft(), body.rect.getSize(), 50, 0.f, {});
		});
	}
}

}
