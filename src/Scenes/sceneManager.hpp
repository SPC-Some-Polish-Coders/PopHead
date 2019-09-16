#pragma once

#include "scene.hpp"
#include <SFML/System.hpp>
#include <memory>

namespace ph {

class GameData;

class SceneManager
{
public:
    SceneManager();

    void replaceScene(const std::string& sceneSourceCodeFilePath);
    void replaceScene(const std::string& sceneSourceCodeFilePath, const sf::Vector2f& playerPosition);
    void popScene();
    
	void changingScenesProcess();

	bool hasPlayerPosition() const;
	const sf::Vector2f& getPlayerPosition() const;

private:
	void replaceAction();
	void popAction();

public:
	void handleEvent(const sf::Event&);
    void update(sf::Time delta);

	Scene& getScene() { return *mScene.get(); }
    void setGameData( ph::GameData* const gameData ){mGameData = gameData;}

	std::string getCurrentMapName() const { return mFileOfSceneToMake; }

private:
    std::unique_ptr<Scene> mScene;
    std::string mFileOfSceneToMake;
    GameData* mGameData;
    bool mIsReplacing;
    bool mIsPopping;
	
	bool mHasPlayerPosition;
	sf::Vector2f mPlayerPosition;
};

}