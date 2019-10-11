#include "glEnums.hpp"
#include "GL/glew.h"
#include "Logs/logs.hpp"

namespace ph {

unsigned toGLEnum(DrawMode drawMode)
{
	switch(drawMode)
	{
		case ph::DrawMode::Points: return GL_POINTS;
		case ph::DrawMode::LineStrip: return GL_LINE_STRIP;
		case ph::DrawMode::LineLoop: return GL_LINE_LOOP;
		case ph::DrawMode::Lines: return GL_LINES;
		case ph::DrawMode::LineStripAdjacency: return GL_LINE_STRIP_ADJACENCY;
		case ph::DrawMode::LinesAdjacency: return GL_LINES_ADJACENCY;
		case ph::DrawMode::TriangleStrip: return GL_TRIANGLE_STRIP;
		case ph::DrawMode::TriangleFan:	return GL_TRIANGLE_FAN;
		case ph::DrawMode::Triangles: return GL_TRIANGLES;
		case ph::DrawMode::TriangleStripAdjacency: GL_TRIANGLE_STRIP_ADJACENCY;
		case ph::DrawMode::TrianglesAdjacency: return GL_TRIANGLES_ADJACENCY;
		default:
			PH_UNEXPECTED_SITUATION("This draw mode is not supported!");
	}
}

}