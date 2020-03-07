#pragma once

#include "scene.hpp"
#include "ECS/entitiesTemplateStorage.hpp"
#include "Utilities/threadPool.hpp"
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

namespace ph {

class Texture;

class SceneManager
{
public:
    SceneManager();

    void replaceScene(const std::string& sceneSourceCodeFilePath);
    void replaceScene(const std::string& sceneSourceCodeFilePath, const sf::Vector2f& playerPosition);
    void popScene();
    
	void changingScenesProcess();

	bool hasPlayerPositionForNextScene() const;
	const sf::Vector2f& getPlayerPositionForNextScene() const;

private:
	void replaceAction();
	void popAction();

public:
	void handleEvent(sf::Event);
    void update(float dt);

	Scene& getScene() { return *mScene.get(); }
	void init(AIManager*);

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
	sf::Vector2f mPlayerPositionForNextScene;
    bool mIsReplacing;
    bool mIsPopping;
	bool mHasPlayerPositionForNextScene;
};

}
