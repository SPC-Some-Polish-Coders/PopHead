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
	std::shared_ptr<VertexArray> mVertexArray;
	std::shared_ptr<Shader> mShader;
	const ph::Texture& mTexture;
};

}