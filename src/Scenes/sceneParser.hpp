#pragma once

#include "ECS/entitiesTemplateStorage.hpp"
#include "ECS/systemsQueue.hpp"
#include "Resources/resourceHolder.hpp"
#include "cutSceneManager.hpp"
#include "Scenes/sceneManager.hpp"
#include "GUI/gui.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "AI/aiManager.hpp"
#include <entt/entt.hpp>
#include <string>

namespace ph{

void parseScene(CutSceneManager&, EntitiesTemplateStorage&, entt::registry&,
                const std::string& sceneFileName, TextureHolder&, SystemsQueue&, AIManager&, SceneManager&);

}

