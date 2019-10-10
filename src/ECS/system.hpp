#pragma once

#include <entt/entity/registry.hpp>

namespace ph {

class System
{
public:
	explicit System(entt::registry& registry);

	virtual void update(float seconds) = 0;

protected:
	entt::registry& mRegistry;
};

}
