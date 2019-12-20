#include "collisionDebug.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Renderer/renderer.hpp"

namespace ph::system {

void CollisionDebug::update(float dt)
{
	if(!sIsCollisionDebugActive)
		return;

	auto staticBodies = mRegistry.view<component::StaticCollisionBody, component::BodyRect>();
	staticBodies.each([](const component::StaticCollisionBody, const component::BodyRect body) 
	{
		Renderer::submitQuad(nullptr, nullptr, &sf::Color(200, 0, 0, 140), nullptr,
			body.rect.getTopLeft(), body.rect.getSize(), 50, 0.f, {}, false);
	});

	auto kinematicBodies = mRegistry.view<component::KinematicCollisionBody, component::BodyRect>();
	kinematicBodies.each([](const component::KinematicCollisionBody, const component::BodyRect& body)
	{
		Renderer::submitQuad(nullptr, nullptr, &sf::Color(45, 100, 150, 140), nullptr,
			body.rect.getTopLeft(), body.rect.getSize(), 50, 0.f, {}, false);
	});
}

}
