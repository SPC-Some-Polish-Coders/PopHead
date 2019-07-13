#pragma once

#include "scene.hpp"
#include <SFML/System.hpp>
#include <memory>

namespace ph {

class GameData;

class SceneMachine
{
public:
    SceneMachine();

    void replaceScene(const std::string& sceneSourceCodeFilePath);
    void popScene();
    
	void changingScenesProcess();
private:
	void replaceAction();
	void popAction();

public:
    void input();
    void update(sf::Time delta);

	Scene& getScene() { return *mScene.get(); }
    void setGameData( ph::GameData* const gameData ){mGameData = gameData;}

private:
    std::unique_ptr<Scene> mScene;
    std::string mSceneToMakeSourceCodeFilepath;
    GameData* mGameData;
    bool mIsReplacing;
    bool mIsPopping;
};

}