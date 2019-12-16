#pragma once

#include "ECS/system.hpp"

namespace ph {

class GUI;

namespace system {

class GameplayUI : public System
{
public:
	GameplayUI(entt::registry&, GUI&);

	void update(float dt) override;

private:
	GUI& mGui;
};

}}
