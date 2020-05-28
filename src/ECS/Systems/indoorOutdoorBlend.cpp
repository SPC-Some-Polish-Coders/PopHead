#include "pch.hpp"
#include "indoorOutdoorBlend.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

using component::IndoorOutdoorBlendArea;

static float getOutdoorFactor(const FloatRect& blendArea, Vec2 objectPos, IndoorOutdoorBlendArea::ExitSide exit)
{
	float res;

	switch(exit)
	{
		case IndoorOutdoorBlendArea::Left:
		{
			float playerFromRight = blendArea.x + blendArea.w - objectPos.x;
			res = playerFromRight / blendArea.w;
		} break;

		case IndoorOutdoorBlendArea::Right:
		{
			float playerFromLeft = objectPos.x - blendArea.x;
			res = playerFromLeft / blendArea.w; 
		} break;

		case IndoorOutdoorBlendArea::Top:
		{
			float playerFromBottom = blendArea.y + blendArea.h - objectPos.y;
			res = playerFromBottom / blendArea.h;
		} break;

		case IndoorOutdoorBlendArea::Down:
		{
			float playerFromTop = objectPos.y - blendArea.y;
			res = playerFromTop / blendArea.h;
		} break;
	}

	return res;
}

float correctDarkness(float brightness)
{
	if(brightness < 0.05f) brightness = 0.05f;
	else if(brightness > 0.95f) brightness = 1.f;
	return brightness;
}

float correctAlpha(float alpha)
{
	if(alpha < 0.05f) alpha = 0.f;
	else if(alpha > 0.95f) alpha = 1.f;
	return alpha;
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
				auto outdoor = getOutdoorFactor(blendAreaBody, playerBody.center(), blendArea.exit); 

				mPlayerOutdoor = correctAlpha(outdoor); 

				mRegistry.view<component::OutdoorBlend>().each([=]
				(component::OutdoorBlend& ob)
				{
					ob.brightness = correctDarkness(outdoor); 
				});

				mRegistry.view<component::IndoorBlend>().each([=]
				(component::IndoorBlend& ib)
				{
					ib.alpha = correctAlpha(1.f - outdoor); 
				});	
			}
		});
	});

	mRegistry.view<component::IndoorOutdoorBlend, component::BodyRect>().each([&]
	(component::IndoorOutdoorBlend& object, component::BodyRect objectBody)
	{
		blendAreas.each([&]
		(component::IndoorOutdoorBlendArea blendArea, const component::BodyRect& blendAreaBody)
		{
			if(intersect(objectBody, blendAreaBody))
			{
				object.outdoor = correctAlpha(getOutdoorFactor(blendAreaBody, objectBody.center(), blendArea.exit));
			}
		});

		bool isPlayerInsideBlendArea = mPlayerOutdoor > 0.f && mPlayerOutdoor < 1.f;
		bool isObjectInsideBlendArea = object.outdoor > 0.f && object.outdoor < 1.f;

		if((object.outdoor == 0.f && mPlayerOutdoor == 0.f) ||
		   (object.outdoor == 1.f && mPlayerOutdoor == 1.f))
		{
			object.brightness = 1.f;
			object.alpha = 1.f; 
		}
		else if(object.outdoor == 1.f && mPlayerOutdoor == 0.f) 
		{
			object.brightness = 0.05f;
			object.alpha = 1.f;
		}
		else if(object.outdoor == 0.f && mPlayerOutdoor == 1.f)
		{
			object.brightness = 1.f;
			object.alpha = 0.f;
		}
		else if(isObjectInsideBlendArea && mPlayerOutdoor == 0.f)
		{
			object.brightness = 1.f - object.outdoor;
			object.alpha = 1.f;
		}
		else if(isObjectInsideBlendArea && mPlayerOutdoor == 1.f)
		{
			object.brightness = 1.f; 
			object.alpha = object.outdoor;
		}
		else if(object.outdoor == 0.f && isPlayerInsideBlendArea)
		{
			object.brightness = 1.f; 
			object.alpha = 1.f - mPlayerOutdoor;
		}
		else if(object.outdoor == 1.f && isPlayerInsideBlendArea)
		{
			object.brightness = mPlayerOutdoor;
			object.alpha = 1.f;
		}
		else if(isObjectInsideBlendArea && isPlayerInsideBlendArea)
		{
			if(object.outdoor > mPlayerOutdoor)
			{
				object.brightness = object.outdoor;
				object.alpha = 1.f;
			}
			else
			{
				object.brightness = 1.f;
				object.alpha = 1.f - object.outdoor;
			}
		}
		else
		{
			PH_BREAKPOINT();	
		}
	});
}

}

