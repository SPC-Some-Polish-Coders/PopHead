#include "pointRenderer.hpp"
#include "Utilities/cast.hpp"
#include "Utilities/profiling.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/API/openglErrors.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"
#include <GL/glew.h>

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

void PointRenderer::resetDebugNumbers()
{
	mNrOfDrawnPoints = 0;
	mNrOfDrawCalls = 0;
}

void PointRenderer::submitPoint(sf::Vector2f position, const sf::Color& color, float z, float size)
{
	if(!isInsideScreen(position, size))
		return;

	PointVertexData point;
	point.color = Cast::toNormalizedColorVector4f(color);
	point.position = position;
	point.size = size * (360.f / mScreenBounds->height);
	point.z = z;
	mSubmitedPointsVertexData.emplace_back(point);

	if(mIsDebugCountingActive)
		++mNrOfDrawnPoints;
}

void PointRenderer::flush()
{
	PH_PROFILE_FUNCTION();

	if(mSubmitedPointsVertexData.empty())
		return;

	mPointsShader.bind();
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PointVertexData) * mSubmitedPointsVertexData.size(), mSubmitedPointsVertexData.data(), GL_STATIC_DRAW);

	glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(mSubmitedPointsVertexData.size()));

	mSubmitedPointsVertexData.clear();

	if(mIsDebugCountingActive)
		++mNrOfDrawCalls;
}

bool PointRenderer::isInsideScreen(sf::Vector2f position, float size)
{
	if(size == 1.f)
		return mScreenBounds->contains(position);
	else {
		const float halfSize = size / 2;
		FloatRect pointRect(position.x - halfSize, position.y - halfSize, size, size);
		return mScreenBounds->doPositiveRectsIntersect(pointRect);
	}
}

}
