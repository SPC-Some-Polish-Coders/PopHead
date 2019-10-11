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

unsigned toGLEnum(DrawPrimitive);

}