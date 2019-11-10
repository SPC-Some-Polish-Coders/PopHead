#include "isPlayerAlive.hpp"
#include "ECS/Components/charactersComponents.hpp"


namespace ph::system {

	void IsPlayerAlive::update(float seconds)
	{
		auto playerView = mRegistry.view<component::Player, component::Health>();
		for (const auto& player : playerView)
		{
			const auto& playerHealth = playerView.get<component::Health>(player);
			mIsAlive = (playerHealth.healthPoints <= 0 ? false : true);		
		}
	}

	bool IsPlayerAlive::isAlive()
	{
		return mIsAlive;
	}

}
