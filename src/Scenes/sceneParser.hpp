#pragma once

#include "ECS/entitiesTemplateStorage.hpp"
#include "ECS/systemsQueue.hpp"
#include "Resources/resourceHolder.hpp"
#include "cutSceneManager.hpp"
#include "GUI/gui.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "AI/aiManager.hpp"
#include "gameData.hpp"
#include <entt/entt.hpp>
#include <string>

namespace ph{

void parseScene(GameData* const, CutSceneManager&, EntitiesTemplateStorage&, entt::registry&,
                const std::string& sceneFileName, TextureHolder&, SystemsQueue&, GUI&, MusicPlayer&, AIManager&);

}

