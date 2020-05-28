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
		mRegistry.view<component::StaticCollisionBody, component::BodyRect>(entt::exclude<component::BodyCircle>).each([]
		(auto, const auto& body) 
		{
			Renderer::submitQuad(Null, Null, &sf::Color(130, 0, 0, 140), Null,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});

		// render multi static collision bodies as bright red rectangle or bright red circles
		mRegistry.view<component::MultiStaticCollisionBody>().each([]
		(const auto& multiCollisionBody) 
		{
			for(auto& bodyRect : multiCollisionBody.rects)
			{
				Renderer::submitQuad(Null, Null, &sf::Color(255, 0, 0, 140), Null,
					bodyRect.pos, bodyRect.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			}

			for(auto& circle : multiCollisionBody.circles)
			{
				Renderer::submitCircle(sf::Color(255, 0, 0, 140), circle.offset - Vec2(circle.radius, circle.radius),
					circle.radius, 50, ProjectionType::gameWorld, false);
			}
		});

		// render kinematic bodies as blue rectangle
		mRegistry.view<component::KinematicCollisionBody, component::BodyRect>(entt::exclude<component::BodyCircle>).each([]
		(auto, const auto& body)
		{
			Renderer::submitQuad(Null, Null, &sf::Color(45, 100, 150, 140), Null,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});

		// render static circle bodies as dark red circle
		mRegistry.view<component::StaticCollisionBody, component::BodyRect, component::BodyCircle>().each([]
		(auto, const auto& rect, const auto& circle)
		{
			Renderer::submitQuad(Null, Null, &sf::Color(130, 0, 0, 140), Null,
				rect.pos, rect.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			Renderer::submitCircle(sf::Color(130, 0, 0, 200), rect.pos + circle.offset - Vec2(circle.radius, circle.radius),
				circle.radius, 50, ProjectionType::gameWorld, false);
		});

		// render kinematic circle bodies as blue circle
		mRegistry.view<component::KinematicCollisionBody, component::BodyRect, component::BodyCircle>().each([]
		(auto, const auto& rect, const auto& circle)
		{
			Renderer::submitQuad(Null, Null, &sf::Color(45, 100, 150, 40), Null,
				rect.pos, rect.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			Renderer::submitCircle(sf::Color(45, 100, 150, 140), rect.pos + circle.offset - Vec2(circle.radius, circle.radius),
				circle.radius, 50, ProjectionType::gameWorld, false);
		});
	}

	if(velocityChangingArea)
	{
		// render velocity changing areas as orange rectangle
		mRegistry.view<component::AreaVelocityChangingEffect, component::BodyRect>().each([]
		(auto, const auto& body) 
		{
			Renderer::submitQuad(Null, Null, &sf::Color(255, 165, 0, 140), Null,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});
	}

	if(pushingArea)
	{
		// render pushing areas as yellow rectangle
		mRegistry.view<component::PushingArea, component::BodyRect>().each([]
		(auto, const auto& body) 
		{
			Renderer::submitQuad(Null, Null, &sf::Color(255, 255, 0, 140), Null,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});
	}

	if(lightWalls)
	{
		// render light walls as blue rectangle
		mRegistry.view<component::LightWall, component::BodyRect>().each([]
		(auto, const auto& body) 
		{
			Renderer::submitQuad(Null, Null, &sf::Color(40, 40, 225, 140), Null,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});

		// render chunk light walls as light blue rectangle
		mRegistry.view<component::RenderChunk>().each([]
		(const auto& chunk) 
		{
			for(const auto wall : chunk.lightWalls) 
			{
				Renderer::submitQuad(Null, Null, &sf::Color(60, 60, 255, 140), Null,
					wall.pos, wall.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			}
		});	
	}

	if(indoorOutdoorBlend)
	{
		// render indoor outdoor blend areas as orange rectangle
		mRegistry.view<component::IndoorOutdoorBlendArea, component::BodyRect>().each([]
		(auto, const auto& body) 
		{
			Renderer::submitQuad(Null, Null, &sf::Color(255, 165, 0, 140), Null,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
		});
	}

	if(collisionDenialAreas || lightWallDenialAreas || collisionAndLightWallDenialAreas)
	{
		// denial areas debug
		mRegistry.view<component::DenialArea, component::BodyRect>().each([]
		(auto denial, const auto& body)
		{
			using component::DenialArea;
			if(denial.type == DenialArea::Collision && collisionDenialAreas)
			{
				// render collision denial areas as dark red rectangle 
				Renderer::submitQuad(Null, Null, &sf::Color(150, 10, 0, 140), Null,
					body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			}
			else if(denial.type == DenialArea::LightWall && lightWallDenialAreas)
			{
				// render collision denial areas as dark yellow rectangle 
				Renderer::submitQuad(Null, Null, &sf::Color(100, 100, 0, 140), Null,
					body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			}
			else if(denial.type == DenialArea::All && collisionAndLightWallDenialAreas)
			{
				// render collision and light wall denial areas as black rectangle 
				Renderer::submitQuad(Null, Null, &sf::Color(0, 0, 0, 140), Null,
					body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			}
		});
	}

	if(hintArea)
	{
		
		mRegistry.view<component::Hint, component::BodyRect>().each([]
		(const auto& hint, const auto& body)
		{
			// render hint area as lime green rectangle
			Renderer::submitQuad(Null, Null, &sf::Color(192, 255, 0, 140), Null,
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
			Renderer::submitQuad(Null, Null, &sf::Color(130, 0, 150, 140), Null,
				body.pos, body.size, 50, 0.f, {}, ProjectionType::gameWorld, false);

			if(cameraRoomCenter)
			{
				FloatRect centerArea = body;
				centerArea.x += camRoom.edgeAreaSize * body.w;
				centerArea.y += camRoom.edgeAreaSize * body.h;
				centerArea.w -= camRoom.edgeAreaSize * body.w * 2.f;
				centerArea.h -= camRoom.edgeAreaSize * body.h * 2.f;

				Renderer::submitQuad(Null, Null, &sf::Color(255, 0, 0, 140), Null,
					centerArea.pos, centerArea.size, 50, 0.f, {}, ProjectionType::gameWorld, false);
			}
		});
	}

	if(puzzleGridRoads)
	{
		mRegistry.view<component::PuzzleGridRoadChunk, component::PuzzleGridPos>().each([&]
		(auto entity, const auto& roadChunk, auto chunkRelativeGridPos)
		{
			for(i32 y = 0; y < 12; ++y)
			{
				for(i32 x = 0; x < 12; ++x)
				{
					if(roadChunk.tiles[y][x])
					{
						// render road tile as green rectangle
						auto gridPos = Cast<Vec2>(chunkRelativeGridPos + Vec2i(x, y));
						auto worldPos = gridPos * 16.f;

						Renderer::submitQuad(Null, Null, &sf::Color(0, 200, 0, 140), Null,
							worldPos, {16.f, 16.f}, 50, 0.f, {}, ProjectionType::gameWorld, false);

						if(puzzleGridRoadsPos)
						{
							// render puzzle grid pos, x up, y down 
							char posText[50];
							sprintf(posText, "%i", (i32)gridPos.x);
							Renderer::submitTextWorldHD(posText, "LiberationMono-Bold.ttf", worldPos, 6, sf::Color::Black, 45);
							sprintf(posText, "%i", (i32)gridPos.y);
							worldPos.y += 4.f;
							Renderer::submitTextWorldHD(posText, "LiberationMono-Bold.ttf", worldPos, 6, sf::Color::Black, 45);
						}

						if(puzzleGridRoadsChunks)
						{
							// render puzzle grid road chukns as rectangles with random color
							if(auto* body = mRegistry.try_get<component::BodyRect>(entity))
							{
								auto color = mRegistry.get<component::DebugColor>(entity);
								Renderer::submitQuad(Null, Null, &color, Null,
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
