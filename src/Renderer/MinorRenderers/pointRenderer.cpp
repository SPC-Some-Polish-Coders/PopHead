#include "pch.hpp"
#include "pointRenderer.hpp"
#include "Utilities/cast.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/API/openglErrors.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"

namespace ph {

void PointRenderer::init()
{
	mPointsShader.init(shader::pointsSrc());
	mPointsShader.initUniformBlock("SharedData", 0);
	glEnable(GL_PROGRAM_POINT_SIZE);

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(PointVertexData), (void*)offsetof(PointVertexData, color));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PointVertexData), (void*)offsetof(PointVertexData, position));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(PointVertexData), (void*)offsetof(PointVertexData, size));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(PointVertexData), (void*)offsetof(PointVertexData, z));

	mSubmitedPointsVertexData.reserve(100);
}

void PointRenderer::shutDown()
{
	glDeleteBuffers(1, &mVBO);
	glDeleteVertexArrays(1, &mVAO);
	mPointsShader.remove();
}

void PointRenderer::submitPoint(Vec2 pos, const sf::Color& color, float z, float size)
{
	if(!isInsideScreen(pos, size))
		return;

	PointVertexData point;
	point.color = toNormalizedColorVec4(color);
	point.position = pos;
	point.size = size * (360.f / mScreenBounds->h);
	point.z = z;
	mSubmitedPointsVertexData.emplace_back(point);
}

void PointRenderer::flush()
{
	if(mSubmitedPointsVertexData.empty())
		return;

	mPointsShader.bind();
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PointVertexData) * mSubmitedPointsVertexData.size(), mSubmitedPointsVertexData.data(), GL_STATIC_DRAW);

	glDrawArrays(GL_POINTS, 0, Cast<GLsizei>(mSubmitedPointsVertexData.size()));

	mSubmitedPointsVertexData.clear();
}

bool PointRenderer::isInsideScreen(Vec2 pos, float size)
{
	if(size == 1.f)
	{
		return mScreenBounds->contains(pos);
	}
	else 
	{
		float halfSize = size / 2;
		FloatRect pointRect(pos.x - halfSize, pos.y - halfSize, size, size);
		return intersect(*mScreenBounds, pointRect);
	}
}

void PointRenderer::submitDebug()
{
	if(ImGui::BeginTabItem("Point renderer"))
	{
		ImGui::Text("drawn points %u", mSubmitedPointsVertexData.size());
		ImGui::Text("draw calls: 1 (it is always 1)");
		ImGui::EndTabItem();
	}
}

}
