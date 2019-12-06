#pragma once

#include "ECS/system.hpp"

namespace ph {
	class AIManager;	
}

namespace ph::system {

	class ZombieSystem : public System 
	{
	public:
		ZombieSystem(entt::registry&, const AIManager*);

		void update(float dt) override;

	private:
		const AIManager* mAIManager;
	};
}
