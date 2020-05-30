#pragma once

#include "scene.hpp"
#include "ECS/entitiesTemplateStorage.hpp"
#include "Utilities/threadPool.hpp"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <memory>

namespace ph {

class Texture;

class SceneManager
{
public:
    SceneManager();

    void replaceScene(const std::string& sceneSourceCodeFilePath);
    void replaceScene(const std::string& sceneSourceCodeFilePath, Vec2 playerPosition);
    void popScene();
    
	void changingScenesProcess();

	bool hasPlayerPositionForNextScene() const;
	const Vec2& getPlayerPositionForNextScene() const;

private:
	void replaceAction();
	void popAction();

public:
	void handleEvent(sf::Event);
    void update(float dt);

	Scene& getScene() { return *mScene.get(); }
	void init(AIManager*, sf::Window*);

	std::string getCurrentSceneFilePath() const { return mCurrentSceneFilePath; }

private:
	EntitiesTemplateStorage mEntitiesTemplateStorage;
	ThreadPool mThreadPool;
    std::unique_ptr<Scene> mScene;
	PlayerStatus mLastPlayerStatus;
	std::string mFilePathOfSceneToMake;
	std::string mCurrentSceneFilePath;
	Texture* mTilesetTexture;
	AIManager* mAIManager;
	sf::Window* mWindow;
	Vec2 mPlayerPositionForNextScene;
    bool mIsReplacing;
    bool mIsPopping;
	bool mHasPlayerPositionForNextScene;
};

}
