#pragma once

#include <memory>
#include <vector>
#include <deque>
#include <SFML/System.hpp>

#include "scene.hpp"

namespace ph {

class GameData;

using ScenePtr = std::unique_ptr<Scene>;

class SceneMachine
{
public:
    SceneMachine();

    void pushScene(const std::string& sceneSourceCodeFilePath);
    void replaceScene(const std::string& sceneSourceCodeFilePath);
    void popScene();
    void clearScenes();
    
	void changingScenesProcess();
private:
	void pushAction();
	void replaceAction();
	void popAction();
	void clearAction();

public:
    void input();
    void update(sf::Time delta);

	Scene& getTopScene() { return *(mActiveScenes.back().get()); }

    auto getScenesAmount() const -> unsigned int {return mActiveScenes.size();}
    bool getHideInSceneNr(unsigned int nrOfScene) const; /// 0 is top
    bool getPauseInSceneNr(unsigned int nrOfScene) const;
    void setHideInSceneNr(unsigned int nrOfScene, bool hide);
    void setPauseInSceneNr(unsigned int nrOfScene, bool pause);

    void setGameData( ph::GameData* const gameData ){mGameData = gameData;}

private:
    std::vector<ScenePtr> mActiveScenes;
    std::deque<ScenePtr> mPendingScenes;

    GameData* mGameData;

    bool mIsPushing;
    bool mIsReplacing;
    bool mIsPopping;
    bool mIsClearing;
};

}
