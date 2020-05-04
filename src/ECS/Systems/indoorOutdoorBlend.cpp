#include "pch.hpp"
#include "indoorOutdoorBlend.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

void IndoorOutdoorBlend::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause)
		return;

	auto players = mRegistry.view<component::Player, component::BodyRect>();
	auto blendAreas = mRegistry.view<component::IndoorOutdoorBlendArea, component::BodyRect>();
	players.each([this, &blendAreas]
	(const component::Player, const component::BodyRect& playerBody)
	{
		blendAreas.each([this, &playerBody]
		(const component::IndoorOutdoorBlendArea& blendArea, const component::BodyRect& blendAreaBody)
		{
			if(playerBody.rect.doPositiveRectsIntersect(blendAreaBody.rect))
			{
				float indoorAlpha, outdoorDarkness;

				float playerFromLeft = playerBody.x - blendAreaBody.x;
				float playerFromRight = blendAreaBody.x + blendAreaBody.width - playerBody.x;
				float playerFromTop = playerBody.y - blendAreaBody.y;
				float playerFromBottom = blendAreaBody.y + blendAreaBody.height - playerBody.y;
				switch(blendArea.exit)
				{
					using component::IndoorOutdoorBlendArea;

					case IndoorOutdoorBlendArea::Left:
					{
						indoorAlpha = playerFromLeft / blendAreaBody.width;
						outdoorDarkness = playerFromRight / blendAreaBody.width;
					} break;

					case IndoorOutdoorBlendArea::Right:
					{
						indoorAlpha = playerFromRight / blendAreaBody.width;
						outdoorDarkness = playerFromLeft / blendAreaBody.width; 
					} break;

					case IndoorOutdoorBlendArea::Top:
					{
						indoorAlpha = playerFromTop / blendAreaBody.height; 
						outdoorDarkness = playerFromBottom / blendAreaBody.height;
					} break;

					case IndoorOutdoorBlendArea::Down:
					{
						indoorAlpha = playerFromBottom / blendAreaBody.height;
						outdoorDarkness = playerFromTop / blendAreaBody.height;
					} break;
				}

				if(indoorAlpha < 0.05f)
				{
					indoorAlpha = 0.f;
				}
				else if(indoorAlpha > 0.95f)
				{
					indoorAlpha = 1.f;
				}

				if(outdoorDarkness < 0.05f)
				{
					outdoorDarkness = 0.05f;
				}
				else if(outdoorDarkness > 0.95f)
				{
					outdoorDarkness = 1.f;
				}

				/*
				if(ImGui::BeginTabItem("indoor outdoor"))
				{
					ImGui::SliderFloat("outdoor darkness", &outdoorDarkness, 0.f, 1.f);
					ImGui::EndTabItem();
				}*/

				auto toColorVal = [](float c) { return static_cast<unsigned char>(c * 255.f); };
				
				auto chunks = mRegistry.view<component::RenderChunk>();
				chunks.each([&](component::RenderChunk& chunk)
				{
					if(chunk.outdoor)
					{
						unsigned char c = toColorVal(outdoorDarkness); 
						chunk.color = sf::Color(c, c, c, 255);
					}
					else
					{
						chunk.color.a = toColorVal(indoorAlpha);
					}
				});

				auto groundChunks = mRegistry.view<component::GroundRenderChunk>(); 
				groundChunks.each([&](component::GroundRenderChunk& groundChunk)
				{
					if(groundChunk.outdoor)
					{
						unsigned char c = toColorVal(outdoorDarkness); 
						groundChunk.color = sf::Color(c, c, c, 255);
					}
					else
					{
						groundChunk.color.a = toColorVal(indoorAlpha);
					}
				});
			}
		});
	});
}

}
