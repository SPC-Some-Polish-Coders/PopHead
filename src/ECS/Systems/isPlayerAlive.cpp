#include "isPlayerAlive.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

	void IsPlayerAlive::update(float dt)
	{
		auto playerView = mRegistry.view<component::Player, component::Health>();
		for (const auto& player : playerView)
		{
			const auto& playerHealth = playerView.get<component::Health>(player);
			mIsAlive = playerHealth.healthPoints > 0;
		}
	}

	bool IsPlayerAlive::isAlive()
	{
		return mIsAlive;
	}

}
