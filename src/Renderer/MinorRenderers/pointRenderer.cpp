#include "pointRenderer.hpp"
#include "Utilities/cast.hpp"
#include "Renderer/Shaders/shaderLibary.hpp"
#include "Renderer/openglErrors.hpp"
#include <GL/glew.h>

namespace ph {

void PointRenderer::init()
{
	auto& sl = ShaderLibrary::getInstance();
	sl.loadFromFile("points", "resources/shaders/points.vs.glsl", "resources/shaders/points.fs.glsl");
	mPointsShader = sl.get("points");

	glEnable(GL_PROGRAM_POINT_SIZE);

	GLCheck( unsigned uniformBlockIndex = glGetUniformBlockIndex(mPointsShader->getID(), "SharedData") );
	GLCheck( glUniformBlockBinding(mPointsShader->getID(), uniformBlockIndex, 0) );

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(PointVertexData), (void*)offsetof(PointVertexData, color));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PointVertexData), (void*)offsetof(PointVertexData, position));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(PointVertexData), (void*)offsetof(PointVertexData, size));

	mSubmitedPointsVertexData.reserve(100);
}

void PointRenderer::shutDown()
{
	glDeleteBuffers(1, &mVBO);
	glDeleteVertexArrays(1, &mVAO);
}

void PointRenderer::submitPoint(sf::Vector2f position, const sf::Color& color, float size)
{
	PointVertexData point;
	point.color = Cast::toNormalizedColorVector4f(color);
	point.position = position;
	point.size = size;
	mSubmitedPointsVertexData.emplace_back(point);
}

void PointRenderer::flush()
{
	mPointsShader->bind();
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PointVertexData) * mSubmitedPointsVertexData.size(), mSubmitedPointsVertexData.data(), GL_STATIC_DRAW);

	glDrawArrays(GL_POINTS, 0, mSubmitedPointsVertexData.size());

	mSubmitedPointsVertexData.clear();
}

}
