#include "pch.hpp"
#include "areasDebug.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/debugComponents.hpp"
#include "Renderer/renderer.hpp"

extern bool debugWindowOpen;

namespace ph::system {

static bool 
enableAreaDebug = true,
collision = false,
velocityChangingArea = false,
pushingArea = false,
lightWalls = false,
indoorOutdoorBlend = false,
collisionDenialAreas = false,
lightWallDenialAreas = false,
collisionAndLightWallDenialAreas = false,
hintArea = false, hintAreaDetail = true,
cameraRoom = false, cameraRoomCenter = true,
puzzleGridRoads = false, puzzleGridRoadsPos = true, puzzleGridRoadsChunks = false;

void AreasDebug::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(debugWindowOpen && ImGui::BeginTabItem("debug visualization"))
	{
		ImGui::Checkbox("enable area debug", &enableAreaDebug);
		ImGui::Checkbox("collisions", &collision);
		ImGui::Checkbox("velocity changing areas", &velocityChangingArea);
		ImGui::Checkbox("pushing areas", &pushingArea);
		ImGui::Checkbox("light walls", &lightWalls);
		ImGui::Checkbox("indoor outdoor blend areas", &indoorOutdoorBlend);
		ImGui::Checkbox("collision denial areas", &collisionDenialAreas);
		ImGui::Checkbox("light wall denial areas", &lightWallDenialAreas);
		ImGui::Checkbox("collision and light wall denial areas", &collisionAndLightWallDenialAreas);
		ImGui::Checkbox("hint area", &hintArea);
		if(hintArea)
			ImGui::Checkbox("hint area detail", &hintAreaDetail);
		ImGui::Checkbox("camera room", &cameraRoom);
		if(cameraRoom)
			ImGui::Checkbox("camera room center", &cameraRoomCenter);
		ImGui::Checkbox("puzzle grid roads", &puzzleGridRoads);
		if(puzzleGridRoads)
		{
			ImGui::Checkbox("puzzle grid roads pos", &puzzleGridRoadsPos);
			ImGui::Checkbox("puzzle grid roads chunks", &puzzleGridRoadsChunks);
		}
		ImGui::EndTabItem();
	}

	if(!enableAreaDebug)
		return;

	if(collision)
	{
		// render static collision bodies as dark red rectangle
		auto staticBodies = mRegistry.view<component::StaticCollisionBody, component::BodyRect>();
		staticBodies.each([](const component::StaticCollisionBody, const component::BodyRect body) 
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
					bodyRect.pos, bodyRect.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			}
		});

		// render kinematic bodies as blue rectangle
		auto kinematicBodies = mRegistry.view<component::KinematicCollisionBody, component::BodyRect>();
		kinematicBodies.each([](const component::KinematicCollisionBody, const component::BodyRect& body)
		{
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(45, 100, 150, 140), nullptr,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});
	}

	if(velocityChangingArea)
	{
		// render velocity changing areas as orange rectangle
		auto velocityChangingAreas = mRegistry.view<component::AreaVelocityChangingEffect, component::BodyRect>();
		velocityChangingAreas.each([](const component::AreaVelocityChangingEffect, const component::BodyRect& body) {
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(255, 165, 0, 140), nullptr,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});
	}

	if(pushingArea)
	{
		// render pushing areas as yellow rectangle
		auto velocityChangingAreas = mRegistry.view<component::PushingArea, component::BodyRect>();
		velocityChangingAreas.each([](const component::PushingArea, const component::BodyRect& body) {
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(255, 255, 0, 140), nullptr,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});
	}

	if(lightWalls)
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
					wall.pos, wall.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			}
		});	
	}

	if(indoorOutdoorBlend)
	{
		// render indoor outdoor blend areas as orange rectangle
		auto areas = mRegistry.view<component::IndoorOutdoorBlendArea, component::BodyRect>();
		areas.each([](const component::IndoorOutdoorBlendArea, const component::BodyRect& body) {
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(255, 165, 0, 140), nullptr,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});
	}

	if(collisionDenialAreas || lightWallDenialAreas || collisionAndLightWallDenialAreas)
	{
		// denial areas debug
		auto areas = mRegistry.view<component::DenialArea, component::BodyRect>(); 
		areas.each([](const component::DenialArea denial, const component::BodyRect& body)
		{
			using component::DenialArea;
			if(denial.type == DenialArea::Collision && collisionDenialAreas)
			{
				// render collision denial areas as dark red rectangle 
				Renderer::submitQuad(nullptr, nullptr, &sf::Color(150, 10, 0, 140), nullptr,
					body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			}
			else if(denial.type == DenialArea::LightWall && lightWallDenialAreas)
			{
				// render collision denial areas as dark yellow rectangle 
				Renderer::submitQuad(nullptr, nullptr, &sf::Color(100, 100, 0, 140), nullptr,
					body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			}
			else if(denial.type == DenialArea::All && collisionAndLightWallDenialAreas)
			{
				// render collision and light wall denial areas as black rectangle 
				Renderer::submitQuad(nullptr, nullptr, &sf::Color(0, 0, 0, 140), nullptr,
					body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			}
		});
	}

	if(hintArea)
	{
		auto areas = mRegistry.view<component::Hint, component::BodyRect>();
		areas.each([](const component::Hint& hint, const component::BodyRect& body)
		{
			// render hint area as lime green rectangle
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(192, 255, 0, 140), nullptr,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);

			if(hintAreaDetail)
			{
				auto pos = body.pos;

				std::string debugText = "hintName: ";
				debugText += hint.hintName;
				Renderer::submitText(debugText.c_str(), "LiberationMono-Bold.ttf", pos, 10, sf::Color::Black,
				                     45, ProjectionType::gameWorld, false);
				pos.y += 10.f;
				debugText = "keyboard content: ";
				debugText += hint.keyboardContent;
				Renderer::submitText(debugText.c_str(), "LiberationMono-Bold.ttf", pos, 10, sf::Color::Black,
				                     45, ProjectionType::gameWorld, false);
				pos.y += 10.f;
				debugText = "joystick content: ";
				debugText += hint.joystickContent;
				Renderer::submitText(debugText.c_str(), "LiberationMono-Bold.ttf", pos, 10, sf::Color::Black,
				                     45, ProjectionType::gameWorld, false);
			}
		});
	}

	if(cameraRoom)
	{
		mRegistry.view<component::CameraRoom, component::BodyRect>().each([&]
		(auto camRoom, const auto& body)
		{
			// render camera rooms as violet rectangle
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(130, 0, 150, 140), nullptr,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);

			if(cameraRoomCenter)
			{
				FloatRect centerArea = body;
				centerArea.x += camRoom.edgeAreaSize * body.w;
				centerArea.y += camRoom.edgeAreaSize * body.h;
				centerArea.w -= camRoom.edgeAreaSize * body.w * 2.f;
				centerArea.h -= camRoom.edgeAreaSize * body.h * 2.f;

				Renderer::submitQuad(nullptr, nullptr, &sf::Color(255, 0, 0, 140), nullptr,
					centerArea.pos, centerArea.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			}
		});
	}

	if(puzzleGridRoads)
	{
		mRegistry.view<component::PuzzleGridRoadChunk, component::PuzzleGridPos>().each([&]
		(auto entity, const auto& roadChunk, auto chunkRelativeGridPos)
		{
			for(int y = 0; y < 12; ++y)
			{
				for(int x = 0; x < 12; ++x)
				{
					if(roadChunk.tiles[y][x])
					{
						// render road tile as green rectangle
						auto gridPos = static_cast<sf::Vector2f>(chunkRelativeGridPos + sf::Vector2i(x, y));
						auto worldPos = gridPos * 16.f;

						Renderer::submitQuad(nullptr, nullptr, &sf::Color(0, 200, 0, 140), nullptr,
							worldPos, {16.f, 16.f}, 50, 0.f, {}, ProjectionType::gameWorld, false);

						if(puzzleGridRoadsPos)
						{
							// render puzzle grid pos, x up, y down 
							char posText[50];
							sprintf(posText, "%i", (int)gridPos.x);
							Renderer::submitTextWorldHD(posText, "LiberationMono-Bold.ttf", worldPos, 6, sf::Color::Black, 45);
							sprintf(posText, "%i", (int)gridPos.y);
							worldPos.y += 4.f;
							Renderer::submitTextWorldHD(posText, "LiberationMono-Bold.ttf", worldPos, 6, sf::Color::Black, 45);
						}

						if(puzzleGridRoadsChunks)
						{
							// render puzzle grid road chukns as rectangles with random color
							if(auto* body = mRegistry.try_get<component::BodyRect>(entity))
							{
								auto color = mRegistry.get<component::DebugColor>(entity);
								Renderer::submitQuad(nullptr, nullptr, &color, nullptr,
									body->pos, body->size, 50, 0.f, {}, ProjectionType::gameWorld, false);
							}
						}
					}
				}
			}
		});
	}
}

}
