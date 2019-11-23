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
		Renderer::submitLine(sf::Color(255, 51, 0), sf::Color::White, lastingShotDetails.startingShotPos, lastingShotDetails.endingShotPos, 1.f);
		});
}

}