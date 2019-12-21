#include "collisionDebug.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

void CollisionDebug::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(!sIsCollisionDebugActive)
		return;

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

}
