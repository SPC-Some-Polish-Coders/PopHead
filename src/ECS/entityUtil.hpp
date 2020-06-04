#include "ECS/Components/physicsComponents.hpp"
#include "aliases.hpp"
#include <entt/entt.hpp>

namespace ph {

void initEntityUtil(entt::registry*);

bool isPlayerOnScene();
entt::entity getPlayerEntity();
component::BodyRect getPlayerBody();
Vec2 getPlayerCenterPos();
Vec2 getPlayerPos();
Vec2 getPlayerFaceDirection();

}
