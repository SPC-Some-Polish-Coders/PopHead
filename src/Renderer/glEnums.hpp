#pragma once

namespace ph {

enum class DrawPrimitive
{
	Points,
	LineStrip,
	LineLoop,
	Lines,
	LineStripAdjacency,
	LinesAdjacency,
	TriangleStrip,
	TriangleFan,
	Triangles,
	TriangleStripAdjacency,
	TrianglesAdjacency
};

enum class DataUsage
{
	streamDraw,
	staticDraw,
	dynamicDraw,
};

unsigned toGLEnum(DrawPrimitive);
unsigned toGLEnum(DataUsage);

}