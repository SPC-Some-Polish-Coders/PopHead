#include "bilbord.hpp"
#include "Logs/logs.hpp"

namespace ph { 
 
Bilbord::Bilbord(const ph::Texture& stayingBilbordTexture, const ph::Texture& lyingBilbordTexture, const bool isLying)
	:GameObject("bilbord")
	,mVertexArray()
	,mStayingBilbordTexture(stayingBilbordTexture)
	,mLyingBilbordTexture(lyingBilbordTexture)
	,mIsLying(isLying)
{
	auto vbo = VertexBufferHolder::getInstance().getRectangleVertexBuffer("bilbord", stayingBilbordTexture.getWidth(), stayingBilbordTexture.getHeight(), false);
	auto ibo = IndexBufferHolder::getInstance().getRectangleIndexBuffer();
	mVertexArray.setVertexBuffer(vbo, VertexBufferLayout::position2_texCoords2);
	mVertexArray.setIndexBuffer(ibo);
}

void Bilbord::drawCurrent(sf::Transform transform)
{
	mIsLying ? mLyingBilbordTexture.bind() : mStayingBilbordTexture.bind();
	Renderer::submit(mVertexArray, transform, mStayingBilbordTexture.getSize());
}

void Bilbord::fallOver()
{
	mIsLying = true;
}

}  
