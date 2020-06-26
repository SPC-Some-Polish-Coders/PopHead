#include "pch.hpp"
#include "levers.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"
#include "ECS/entityUtil.hpp"
#include "Utilities/joystickMacros.hpp"

namespace ph::system {

using namespace component;

void Levers::update(float dt)
{
	mRegistry.view<Lever, InsideSimRegion, TextureRect, BodyRect>().each([&]
	(auto& lever, auto, auto& leverTextureRect, auto leverBody)
	{
		lever.wasJustSwitched = false;
	});
}

void Levers::onEvent(sf::Event e)
{
	if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::E)
		handleUsedLevers();
	else if(e.type == sf::Event::JoystickButtonPressed && e.joystickButton.button == PH_JOYSTICK_Y)
		handleUsedLevers();
}

void Levers::handleUsedLevers() const
{
	if(isPlayerAlive())
	{
		auto playerBody = getPlayerBody();
		mRegistry.view<Lever, InsideSimRegion, TextureRect, BodyRect>().each([&]
		(auto& lever, auto, auto& leverTextureRect, auto leverBody)
		{
			if(lever.turnOffAfterSwitch && lever.active) return;

			if(intersect(leverBody, playerBody))
			{
				lever.wasJustSwitched = true;
				lever.active = !lever.active;
				leverTextureRect = lever.active ? IntRect(9, 0, 7, 15) : IntRect(0, 0, 7, 15);
			}
		});
	}
}

}

