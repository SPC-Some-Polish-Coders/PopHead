#pragma once

#include "GameObjects/gameObject.hpp"
#include "gameData.hpp"

namespace ph {

class GameData;

class Item : public GameObject
{
public:
	Item(GameData* const gameData, const std::string& name);

	virtual void onUse();
	virtual void onPickUp();
	virtual void onDrop();

	virtual void drawWhileOnTheGround(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	virtual void updateWhileOnTheGround(const sf::Time time) = 0;

	void setPosition(const sf::Vector2f& position);

	void updateCurrent(const sf::Time) override;
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setInteractable(const bool interactable) { mInteractable = interactable; }
	bool getInteractable() const { return mInteractable; }
	void setInInventory(const bool inInv) { mInInventory = inInv; }
	bool getInInventory() const { return mInInventory; }

protected:
	auto getGroundSprite() const -> const sf::Sprite& { return mGroundSprite; }
	auto getInventorySprite() const -> const sf::Sprite& { return mInventorySprite; }
	void setGroundTexture(sf::Texture& groundTexture) { mGroundSprite.setTexture(groundTexture); }
	void setInventoryTexture(sf::Texture& inventoryTexture) { mInventorySprite.setTexture(inventoryTexture); }

private:
	GameData* const mGameData;
	sf::Sprite mGroundSprite;
	sf::Sprite mInventorySprite;
	bool mInteractable;
	bool mInInventory;
};

}