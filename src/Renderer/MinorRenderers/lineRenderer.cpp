#include "pch.hpp"
#include "lineRenderer.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/API/openglErrors.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"
#include "Utilities/vector4.hpp"
#include "Utilities/cast.hpp"

namespace ph::LineRenderer {

struct DebugInfo
{
	size_t drawnLines = 0;
	size_t drawCalls = 0; 
};
static DebugInfo debugInfo;

struct LineVertexData
{
	Vec4 color;
	Vec2 pos;
};

struct LineBatch
{
	std::vector<LineVertexData> vertexData;
	float thickness;
};

static std::vector<LineBatch> lineBatches;

static Shader lineShader;
static const FloatRect* screenBounds;
static u32 lineVAO;
static u32 lineVBO;

static u32 drawCalls;

void init()
{
	lineShader.init(shader::lineSrc());
	lineShader.initUniformBlock("SharedData", 0);

	GLCheck( glEnable(GL_LINE_SMOOTH) );
	GLCheck( glHint(GL_LINE_SMOOTH_HINT, GL_NICEST) );

	GLCheck( glGenVertexArrays(1, &lineVAO) );
	GLCheck( glBindVertexArray(lineVAO) );

	GLCheck( glGenBuffers(1, &lineVBO) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, lineVBO) );

	GLCheck( glEnableVertexAttribArray(0) );
	GLCheck( glEnableVertexAttribArray(1) );
	GLCheck( glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), Null) );
	GLCheck( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float))) );
}

void shutDown()
{
	GLCheck( glDeleteVertexArrays(1, &lineVAO) );
	GLCheck( glDeleteBuffers(1, &lineVBO) );
}

void submitLine(sf::Color colorA, sf::Color colorB, Vec2 posA, Vec2 posB, float thickness)
{
	auto colA = toNormalizedColorVec4(colorA);
	auto colB = toNormalizedColorVec4(colorB);

	bool batchExists = false;
	for(auto& batch : lineBatches)
	{
		if(batch.thickness == thickness)
		{
			batch.vertexData.emplace_back(LineVertexData{colA, posA});
			batch.vertexData.emplace_back(LineVertexData{colB, posB});
			batchExists = true;
		}
	}

	if(!batchExists)
	{
		auto& batch = lineBatches.emplace_back();
		batch.thickness = thickness;
		batch.vertexData.emplace_back(LineVertexData{colA, posA});
		batch.vertexData.emplace_back(LineVertexData{colB, posB});
	}
}

void flush()
{
	debugInfo.drawnLines = 0;
	debugInfo.drawCalls = lineBatches.size();

	for(auto& batch : lineBatches)
	{
		debugInfo.drawnLines += batch.vertexData.size();

		GLCheck( glBindBuffer(GL_ARRAY_BUFFER, lineVBO) );
		GLCheck( glBufferData(GL_ARRAY_BUFFER, batch.vertexData.size() * sizeof(LineVertexData), batch.vertexData.data(), GL_STATIC_DRAW) );

		GLCheck( glBindVertexArray(lineVAO) );
		lineShader.bind();
		GLCheck( glLineWidth(batch.thickness * (360.f / screenBounds->h)) );

		GLCheck( glDrawArrays(GL_LINES, 0, Cast<u32>(batch.vertexData.size())) );
	}

	lineBatches.clear();
}

void submitDebug()
{
	if(ImGui::BeginTabItem("line renderer"))
	{
		ImGui::Text("drawn lines: %u", debugInfo.drawnLines);
		ImGui::Text("draw calls: %u", debugInfo.drawCalls);
		ImGui::EndTabItem();
	}
}

void setScreenBoundsPtr(const FloatRect* sb) 
{ 
	screenBounds = sb; 
}

}

