#include "lineRenderer.hpp"
#include "Renderer/Shaders/shaderLibary.hpp"
#include "Renderer/openglErrors.hpp"
#include "Utilities/profiling.hpp"
#include "Utilities/vector4.hpp"
#include "Utilities/cast.hpp"
#include <GL/glew.h>

namespace ph {

void LineRenderer::init()
{
	PH_PROFILE_FUNCTION();

	auto& sl = ShaderLibrary::getInstance();
	sl.loadFromFile("line", "resources/shaders/line.vs.glsl", "resources/shaders/line.fs.glsl");
	mLineShader = sl.get("line");

	GLCheck( glEnable(GL_LINE_SMOOTH) );
	GLCheck( glHint(GL_LINE_SMOOTH_HINT, GL_NICEST) );

	GLCheck( glGenVertexArrays(1, &mLineVAO) );
	GLCheck( glBindVertexArray(mLineVAO) );

	GLCheck( glGenBuffers(1, &mLineVBO) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mLineVBO) );
	GLCheck( glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW) );

	GLCheck( glEnableVertexAttribArray(0) );
	GLCheck( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0) );
	GLCheck( glEnableVertexAttribArray(1) );
	GLCheck( glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float))) );
}

void LineRenderer::shutDown()
{
	PH_PROFILE_FUNCTION();

	GLCheck( glDeleteVertexArrays(1, &mLineVAO) );
	GLCheck( glDeleteBuffers(1, &mLineVBO) );
}

void LineRenderer::setDebugNumbersToZero()
{
	PH_PROFILE_FUNCTION();

	mNumberOfDrawCalls = 0;
}

void LineRenderer::drawLine(const sf::Color& colorA, const sf::Color& colorB,
                            const sf::Vector2f posA, const sf::Vector2f posB, float thickness)
{
	PH_PROFILE_FUNCTION();

	mLineShader->bind();
	mLineShader->setUniformMatrix4x4("viewProjectionMatrix", mViewProjectionMatrix);

	auto colA = Cast::toNormalizedColorVector4f(colorA);
	auto colB = Cast::toNormalizedColorVector4f(colorB);

	float vertexData[] = {
		posA.x, posA.y, colA.x, colA.y, colA.z, colA.w,
		posB.x, posB.y, colB.x, colB.y, colB.z, colB.w
	};

	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mLineVBO) );
	GLCheck( glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * 6 * sizeof(float), vertexData) );

	GLCheck( glBindVertexArray(mLineVAO) );
	
	GLCheck( glLineWidth(thickness) );

	GLCheck( glDrawArrays(GL_LINES, 0, 2) );

	++mNumberOfDrawCalls;
}

}