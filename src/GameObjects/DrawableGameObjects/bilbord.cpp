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
	auto vbo = VertexBufferHolder::getGlobalInstance().getRectangleVertexBuffer("bilbord", stayingBilbordTexture.getWidth(), stayingBilbordTexture.getHeight());
	auto ibo = IndexBufferHolder::getGlobalInstance().getRectangleIndexBuffer("rectangle");
	mVertexArray.setVertexBuffer(vbo, VertexBufferLayout::position2_texCoords2);
	mVertexArray.setIndexBuffer(ibo);
}

void Bilbord::drawCurrent(sf::Transform transform)
{
	mIsLying ? mLyingBilbordTexture.bind() : mStayingBilbordTexture.bind();
	Renderer::submit(mVertexArray, transform);
}

void Bilbord::fallOver()
{
	mIsLying = true;
}

}  
