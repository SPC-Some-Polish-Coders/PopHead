#include "pch.hpp"
#include "lineRenderer.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/API/openglErrors.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"
#include "Utilities/vector4.hpp"
#include "Utilities/cast.hpp"

namespace ph {

static u32 drawCalls;

void LineRenderer::init()
{
	mLineShader.init(shader::lineSrc());
	mLineShader.initUniformBlock("SharedData", 0);

	GLCheck( glEnable(GL_LINE_SMOOTH) );
	GLCheck( glHint(GL_LINE_SMOOTH_HINT, GL_NICEST) );

	GLCheck( glGenVertexArrays(1, &mLineVAO) );
	GLCheck( glBindVertexArray(mLineVAO) );

	GLCheck( glGenBuffers(1, &mLineVBO) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mLineVBO) );
	GLCheck( glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), Null, GL_DYNAMIC_DRAW) );

	GLCheck( glEnableVertexAttribArray(0) );
	GLCheck( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0) );
	GLCheck( glEnableVertexAttribArray(1) );
	GLCheck( glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float))) );
}

void LineRenderer::shutDown()
{
	GLCheck( glDeleteVertexArrays(1, &mLineVAO) );
	GLCheck( glDeleteBuffers(1, &mLineVBO) );
}

void LineRenderer::drawLine(const sf::Color& colorA, const sf::Color& colorB,
                            const Vec2 posA, const Vec2 posB, float thickness)
{
	auto colA = toNormalizedColorVec4(colorA);
	auto colB = toNormalizedColorVec4(colorB);
	float vertexData[] = {
		posA.x, posA.y, colA.x, colA.y, colA.z, colA.w,
		posB.x, posB.y, colB.x, colB.y, colB.z, colB.w
	};
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mLineVBO) );
	GLCheck( glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * 6 * sizeof(float), vertexData) );

	GLCheck( glBindVertexArray(mLineVAO) );
	mLineShader.bind();
	
	GLCheck( glLineWidth(thickness * (360.f / mScreenBounds->h)) );

	GLCheck( glDrawArrays(GL_LINES, 0, 2) );

	++drawCalls;
}

void LineRenderer::submitDebug()
{
	if(ImGui::BeginTabItem("line renderer"))
	{
		ImGui::Text("draw calls: %u", drawCalls);
		ImGui::EndTabItem();
	}
	drawCalls = 0;
}

}
