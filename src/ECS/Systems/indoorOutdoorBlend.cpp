#include "pch.hpp"
#include "indoorOutdoorBlend.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

using component::IndoorOutdoorBlendArea;

struct BlendColor { float outdoorDarkness, indoorAlpha; };

static BlendColor getIndoorOutdoorBlendColor(const FloatRect& blendArea, sf::Vector2f objectPos, IndoorOutdoorBlendArea::ExitSide exit)
{
	BlendColor res;

	float playerFromLeft = objectPos.x - blendArea.x;
	float playerFromRight = blendArea.x + blendArea.w - objectPos.x;
	float playerFromTop = objectPos.y - blendArea.y;
	float playerFromBottom = blendArea.y + blendArea.h - objectPos.y;
	switch(exit)
	{
		case IndoorOutdoorBlendArea::Left:
		{
			res.indoorAlpha = playerFromLeft / blendArea.w;
			res.outdoorDarkness = playerFromRight / blendArea.w;
		} break;

		case IndoorOutdoorBlendArea::Right:
		{
			res.indoorAlpha = playerFromRight / blendArea.w;
			res.outdoorDarkness = playerFromLeft / blendArea.w; 
		} break;

		case IndoorOutdoorBlendArea::Top:
		{
			res.indoorAlpha = playerFromTop / blendArea.h; 
			res.outdoorDarkness = playerFromBottom / blendArea.h;
		} break;

		case IndoorOutdoorBlendArea::Down:
		{
			res.indoorAlpha = playerFromBottom / blendArea.h;
			res.outdoorDarkness = playerFromTop / blendArea.h;
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

	if(sPause)
		return;

	auto blendAreas = mRegistry.view<component::IndoorOutdoorBlendArea, component::BodyRect>();

	mRegistry.view<component::Player, component::BodyRect>().each([&]
	(const component::Player, const component::BodyRect& playerBody)
	{
		blendAreas.each([this, &playerBody]
		(component::IndoorOutdoorBlendArea blendArea, const component::BodyRect& blendAreaBody)
		{
			if(intersect(playerBody, blendAreaBody))
			{
				auto bc = getIndoorOutdoorBlendColor(blendAreaBody, playerBody.center(), blendArea.exit); 

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
			if(intersect(objectBody, blendAreaBody))
			{
				auto bc = getIndoorOutdoorBlendColor(blendAreaBody, objectBody.center(), blendArea.exit);
				objectBlend.outdoor = bc.outdoorDarkness;
			}
		});

		objectBlend.outdoorDarkness = mPlayerOutdoor * objectBlend.outdoor;
	});
}

}

