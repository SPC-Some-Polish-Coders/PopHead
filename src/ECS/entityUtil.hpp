#include "ECS/Components/physicsComponents.hpp"
#include "aliases.hpp"
#include <entt/entt.hpp>

namespace ph {

void initEntityUtil(entt::registry*);

bool isPlayerOnScene();
bool isPlayerAlive();
entt::entity getPlayerEntity();
component::BodyRect getPlayerBody();
component::BodyRect& getPlayerBodyRef();
Vec2 getPlayerCenterPos();
Vec2 getPlayerPos();
Vec2 getPlayerFaceDirection();

}
