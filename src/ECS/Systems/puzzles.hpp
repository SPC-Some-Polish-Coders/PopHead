#pragma once

#include "ECS/system.hpp"
#include "ECS/entitiesTemplateStorage.hpp"

namespace ph::system {

class Puzzles : public System
{
public:
	Puzzles(entt::registry&, EntitiesTemplateStorage&);

	void update(float dt) override;

private:
	EntitiesTemplateStorage& mEntitiesTemplateStorage;
};

}
