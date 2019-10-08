#include "bilbord.hpp"
#include "Logs/logs.hpp"
#include "Renderer/bufferHolders.hpp"
#include "Renderer/renderer.hpp"

namespace ph { 
 
Bilbord::Bilbord(const ph::Texture& stayingBilbordTexture, const ph::Texture& lyingBilbordTexture, const bool isLying)
	:GameObject("bilbord")
	,mVertexArray(std::make_shared<VertexArray>())
	,mShader(std::make_unique<Shader>())
	,mStayingBilbordTexture(stayingBilbordTexture)
	,mLyingBilbordTexture(lyingBilbordTexture)
	,mIsLying(isLying)
{
	auto vbo = VertexBufferHolder::getGlobalInstance().getRectangleVertexBuffer("car", stayingBilbordTexture.getWidth(), stayingBilbordTexture.getHeight());
	auto ibo = IndexBufferHolder::getGlobalInstance().getRectangleIndexBuffer("rectangle");
	mVertexArray->setVertexBuffer(vbo, VertexBufferLayout::position2_texCoords2);
	mVertexArray->setIndexBuffer(ibo);

	mShader->loadFromFile("resources/shaders/basic.vs.glsl", "resources/shaders/basic.fs.glsl");
}

void Bilbord::drawCurrent(sf::Transform transform)
{
	mIsLying ? mLyingBilbordTexture.bind() : mStayingBilbordTexture.bind();
	Renderer::submit(mVertexArray, mShader, transform);
}

void Bilbord::fallOver()
{
	mIsLying = true;
}

}  
