#pragma once

#include "ECS/system.hpp"

namespace ph {
	class SceneManager;
}

namespace ph::system {

class Entrances : public System
{
public:
	using System::System;
	
	Entrances(entt::registry& registry, SceneManager& sceneManager);

	void update(float seconds) override;

private:
	SceneManager& mSceneManager;
};

}