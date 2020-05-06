#include "pch.hpp"
#include "indoorOutdoorBlend.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

extern bool debugWindowOpen;

namespace ph::system {

using component::IndoorOutdoorBlendArea;

struct BlendColor { float outdoorDarkness, indoorAlpha; };

static BlendColor getIndoorOutdoorBlendColor(const FloatRect& blendArea, sf::Vector2f objectPos, IndoorOutdoorBlendArea::ExitSide exit)
{
	BlendColor res;

	float playerFromLeft = objectPos.x - blendArea.left;
	float playerFromRight = blendArea.left + blendArea.width - objectPos.x;
	float playerFromTop = objectPos.y - blendArea.top;
	float playerFromBottom = blendArea.top + blendArea.height - objectPos.y;
	switch(exit)
	{
		case IndoorOutdoorBlendArea::Left:
		{
			res.indoorAlpha = playerFromLeft / blendArea.width;
			res.outdoorDarkness = playerFromRight / blendArea.width;
		} break;

		case IndoorOutdoorBlendArea::Right:
		{
			res.indoorAlpha = playerFromRight / blendArea.width;
			res.outdoorDarkness = playerFromLeft / blendArea.width; 
		} break;

		case IndoorOutdoorBlendArea::Top:
		{
			res.indoorAlpha = playerFromTop / blendArea.height; 
			res.outdoorDarkness = playerFromBottom / blendArea.height;
		} break;

		case IndoorOutdoorBlendArea::Down:
		{
			res.indoorAlpha = playerFromBottom / blendArea.height;
			res.outdoorDarkness = playerFromTop / blendArea.height;
		} break;
	}

	if(res.indoorAlpha < 0.05f) res.indoorAlpha = 0.f;
	else if(res.indoorAlpha > 0.95f) res.indoorAlpha = 1.f;

	if(res.outdoorDarkness < 0.05f) res.outdoorDarkness = 0.05f;
	else if(res.outdoorDarkness > 0.95f) res.outdoorDarkness = 1.f;

	return res;
}

void IndoorOutdoorBlend::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(debugWindowOpen && ImGui::BeginTabItem("IndoorOutdoorBlend"))
	{
		ImGui::Text("mPlayerOutdoor: %f", mPlayerOutdoor);
		ImGui::EndTabItem();		
	}

	if(sPause)
		return;

	auto blendAreas = mRegistry.view<component::IndoorOutdoorBlendArea, component::BodyRect>();

	mRegistry.view<component::Player, component::BodyRect>().each([&]
	(const component::Player, const component::BodyRect& playerBody)
	{
		blendAreas.each([this, &playerBody]
		(component::IndoorOutdoorBlendArea blendArea, const component::BodyRect& blendAreaBody)
		{
			if(playerBody.rect.doPositiveRectsIntersect(blendAreaBody.rect))
			{
				auto bc = getIndoorOutdoorBlendColor(blendAreaBody.rect, playerBody.rect.getCenter(), blendArea.exit); 

				mPlayerOutdoor = bc.outdoorDarkness;

				mRegistry.view<component::OutdoorBlend>().each([=]
				(component::OutdoorBlend& ob)
				{
					ob.darkness = bc.outdoorDarkness;
				});

				mRegistry.view<component::IndoorBlend>().each([=]
				(component::IndoorBlend& ib)
				{
					ib.alpha = bc.indoorAlpha;
				});	
			}
		});
	});

	mRegistry.view<component::IndoorOutdoorBlend, component::BodyRect>().each([&]
	(component::IndoorOutdoorBlend& objectBlend, component::BodyRect objectBody)
	{
		blendAreas.each([&]
		(component::IndoorOutdoorBlendArea blendArea, const component::BodyRect& blendAreaBody)
		{
			if(FloatRect::doPositiveRectsIntersect(objectBody.rect, blendAreaBody.rect))
			{
				auto bc = getIndoorOutdoorBlendColor(blendAreaBody.rect, objectBody.rect.getCenter(), blendArea.exit);
				objectBlend.outdoor = bc.outdoorDarkness;
			}
		});

		objectBlend.outdoorDarkness = mPlayerOutdoor * objectBlend.outdoor;
	});
}

}

