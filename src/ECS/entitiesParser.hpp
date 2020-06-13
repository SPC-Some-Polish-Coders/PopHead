#pragma once

#include "entitiesTemplateStorage.hpp"

namespace ph {

void parseEntities(const std::string& filePath, EntitiesTemplateStorage&,
                   entt::registry& gameRegistry);

}
