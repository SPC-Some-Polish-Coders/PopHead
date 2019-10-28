#pragma once

#include "GameObjects/gameObject.hpp"
#include "Renderer/renderer.hpp"
#include "gameData.hpp"

namespace ph {

class GameData;

class Item : public GameObject
{
public:
	Item(GameData* const gameData, const std::string& name, const Texture& groundTexture);

	virtual void onUse();
	virtual void onPickUp();
	virtual void onDrop();

	virtual void updateWhileOnTheGround(const sf::Time time) = 0;

	void setPosition(const sf::Vector2f& position);

	void updateCurrent(const sf::Time) override;
	void drawCurrent(sf::Transform) override;

	void setInteractable(const bool interactable) { mInteractable = interactable; }
	bool getInteractable() const { return mInteractable; }
	void setInInventory(const bool inInv) { mInInventory = inInv; }
	bool getInInventory() const { return mInInventory; }

private:
	const Texture& mTexture;
	GameData* const mGameData;
	bool mInteractable;
	bool mInInventory;
};

}