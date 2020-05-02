#include "pch.hpp"
#include "areasDebug.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Renderer/renderer.hpp"

extern bool debugWindowOpen;

namespace ph::system {

static bool collisionDebug = false;
static bool velocityChangingAreaDebug = false;
static bool pushingAreaDebug = false;
static bool lightWallsDebug = false;

void AreasDebug::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(debugWindowOpen && ImGui::BeginTabItem("game debug visualization"))
	{
		ImGui::Checkbox("collisions", &collisionDebug);
		ImGui::Checkbox("velocity changing areas", &velocityChangingAreaDebug);
		ImGui::Checkbox("pushing areas", &pushingAreaDebug);
		ImGui::Checkbox("light walls", &lightWallsDebug);
		ImGui::EndTabItem();
	}

	if(collisionDebug)
	{
		// render static collision bodies as dark red rectangle
		auto staticBodies = mRegistry.view<component::StaticCollisionBody, component::BodyRect>();
		staticBodies.each([](const component::StaticCollisionBody, const component::BodyRect& body) 
		{
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(130, 0, 0, 140), nullptr,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
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
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});

		// render static circle bodies as dark red circle
		auto staticCircleBodies = mRegistry.view<component::StaticCollisionBody, component::BodyCircle>();
		staticCircleBodies.each([](const component::StaticCollisionBody, const component::BodyCircle& body)
		{
			Renderer::submitCircle(sf::Color(130, 0, 0, 140), body.center - sf::Vector2f(body.radius, body.radius), 
				body.radius, 50, ProjectionType::gameWorld, false);
		});

		// render kinematic circle bodies as blue circle
		auto kinematicCircleBodies = mRegistry.view<component::KinematicCollisionBody, component::BodyCircle>();
		kinematicCircleBodies.each([](const component::KinematicCollisionBody, const component::BodyCircle& body)
		{
			Renderer::submitCircle(sf::Color(45, 100, 150, 140), body.center - sf::Vector2f(body.radius, body.radius),
				body.radius, 50, ProjectionType::gameWorld, false);
		});
	}

	if(velocityChangingAreaDebug)
	{
		// render velocity changing areas as orange rectangle
		auto velocityChangingAreas = mRegistry.view<component::AreaVelocityChangingEffect, component::BodyRect>();
		velocityChangingAreas.each([](const component::AreaVelocityChangingEffect, const component::BodyRect& body) {
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(255, 165, 0, 140), nullptr,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});
	}

	if(pushingAreaDebug)
	{
		// render pushing areas as yellow rectangle
		auto velocityChangingAreas = mRegistry.view<component::PushingArea, component::BodyRect>();
		velocityChangingAreas.each([](const component::PushingArea, const component::BodyRect& body) {
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(255, 255, 0, 140), nullptr,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});
	}

	if(lightWallsDebug)
	{
		// render light walls as blue rectangle
		auto lightWalls = mRegistry.view<component::LightWall, component::BodyRect>();
		lightWalls.each([](const component::LightWall, const component::BodyRect body) {
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(40, 40, 225, 140), nullptr,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});

		// render chunk light walls as light blue rectangle
		auto chunks = mRegistry.view<component::RenderChunk>();
		chunks.each([](const component::RenderChunk& chunk) {
			for(const auto wall : chunk.lightWalls) {
				Renderer::submitQuad(nullptr, nullptr, &sf::Color(60, 60, 255, 140), nullptr,
					wall.getTopLeft(), wall.getSize(), 50, 0.f, {}, ProjectionType::gameWorld, false);
			}
		});	
	}
}

}
