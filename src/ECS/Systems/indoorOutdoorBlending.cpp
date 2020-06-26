#include "pch.hpp"
#include "indoorOutdoorBlending.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"

namespace ph::system {

using namespace component;

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

static float correctBrightness(float brightness)
{
	if(brightness < 0.05f) brightness = 0.05f;
	else if(brightness > 0.95f) brightness = 1.f;
	return brightness;
}

static float correctAlpha(float alpha)
{
	if(alpha < 0.05f) alpha = 0.f;
	else if(alpha > 0.95f) alpha = 1.f;
	return alpha;
}

void IndoorOutdoorBlending::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<Player, BodyRect>().each([&]
	(auto, auto playerBody)
	{
		bool playerIntersectsArea = false;

		auto setEnviromentColors = [&](float brightness, float alpha)
		{
			mRegistry.view<OutdoorBlend>().each([=]
			(OutdoorBlend& ob)
			{
				ob.brightness = correctBrightness(brightness); 
			});

			mRegistry.view<IndoorBlend>().each([=]
			(IndoorBlend& ib)
			{
				ib.alpha = correctAlpha(alpha); 
			});	
		};

		mRegistry.view<IndoorOutdoorBlendArea, InsideSimRegion, BodyRect>().each([&]
		(auto blendArea, auto, auto blendAreaBody)
		{
			if(intersect(playerBody, blendAreaBody))
			{
				playerIntersectsArea = true;

				auto outdoor = getOutdoorFactor(blendAreaBody, playerBody.center(), blendArea.exit); 
				mPlayerOutdoor = correctAlpha(outdoor); 
				setEnviromentColors(outdoor, 1.f - outdoor);
			}
		});
		if(playerIntersectsArea) return;

		mRegistry.view<IndoorArea, InsideSimRegion, BodyRect>().each([&]
		(auto, auto, auto indoorAreaBody)
		{
			if(intersect(playerBody, indoorAreaBody))
			{
				playerIntersectsArea = true;
				mPlayerOutdoor = 0.f;
				setEnviromentColors(0.f, 1.f);
			}
		});
		if(playerIntersectsArea) return;

		mRegistry.view<OutdoorArea, InsideSimRegion, BodyRect>().each([&]
		(auto, auto, auto outdoorAreaBody)
		{
			if(intersect(playerBody, outdoorAreaBody))
			{
				playerIntersectsArea = true;
				mPlayerOutdoor = 1.f;
				setEnviromentColors(1.f, 0.f);
			}
		});
		if(playerIntersectsArea) return;
	});

	mRegistry.view<IndoorOutdoorBlend, InsideSimRegion, BodyRect>().each([&]
	(auto& object, auto, auto objectBody)
	{
		mRegistry.view<IndoorOutdoorBlendArea, InsideSimRegion, BodyRect>().each([&]
		(auto blendArea, auto, auto blendAreaBody)
		{
			if(intersect(objectBody, blendAreaBody))
				object.outdoor = correctAlpha(getOutdoorFactor(blendAreaBody, objectBody.center(), blendArea.exit));
		});

		mRegistry.view<IndoorArea, BodyRect>().each([&]
		(auto, auto indoorAreaBody)
		{
			if(intersect(objectBody, indoorAreaBody))
				object.outdoor = 0.f; 
		});

		mRegistry.view<OutdoorArea, BodyRect>().each([&]
		(auto, auto outdoorAreaBody)
		{
			if(intersect(objectBody, outdoorAreaBody))
				object.outdoor = 1.f; 
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

