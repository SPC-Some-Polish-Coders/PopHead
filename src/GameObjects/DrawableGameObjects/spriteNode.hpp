#pragma once

#include "GameObjects/gameObject.hpp"
#include "Renderer/renderer.hpp"

namespace ph {

class SpriteNode : public GameObject
{
public:
	SpriteNode(const Texture&);

	void drawCurrent(sf::Transform) override;
private:
	Sprite mSprite;
	inline static int serialNumber = 0;
};

}