#pragma once

#include "ECS/system.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Window.hpp>

namespace ph::system {

class EntitiesDebugger : public System
{
public:
	EntitiesDebugger(entt::registry&, sf::Window*);

	void update(float dt) override;

private:
	sf::Window* mWindow;
	entt::entity mSelected = entt::null;
};

}
