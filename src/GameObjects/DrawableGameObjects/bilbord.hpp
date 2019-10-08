#pragma once 

#include "GameObjects/gameObject.hpp"
#include "Renderer/renderer.hpp"

namespace ph { 
 
class Bilbord : public GameObject
{
public:
	Bilbord(const ph::Texture& stayingBilbordTexture, const ph::Texture& lyingBilbordTexture, const bool isLying);

	void drawCurrent(sf::Transform) override;

	void fallOver();

private:
	std::shared_ptr<VertexArray> mVertexArray;
	std::shared_ptr<Shader> mShader;
	const ph::Texture& mStayingBilbordTexture;
	const ph::Texture& mLyingBilbordTexture;
	bool mIsLying;
};

} 
