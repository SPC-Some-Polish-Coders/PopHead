#pragma once

#include "Scenes/cutScene.hpp"

namespace ph {

class StartGameCutScene : public CutScene
{
public:
	StartGameCutScene(GameObject& root);
	void update(const sf::Time delta) override;
};

}