#pragma once

namespace ph {

enum class DrawMode
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

unsigned toGLEnum(DrawMode);

}