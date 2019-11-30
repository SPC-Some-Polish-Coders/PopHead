#pragma once

#include "lastingShots.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Renderer/renderer.hpp"

namespace ph::system {

void LastingShots::update(float seconds)
{
	const auto lastingShotsView = mRegistry.view<component::LastingShot>();

	lastingShotsView.each([](const component::LastingShot& lastingShotDetails) {
		Renderer::submitLine(sf::Color(230, 160, 0), sf::Color(250, 250, 200), lastingShotDetails.startingShotPos, lastingShotDetails.endingShotPos, 1.f);
	});
}

}