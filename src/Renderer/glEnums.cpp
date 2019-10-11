#include "glEnums.hpp"
#include "GL/glew.h"
#include "Logs/logs.hpp"

namespace ph {

unsigned toGLEnum(DrawPrimitive drawMode)
{
	switch(drawMode)
	{
		case ph::DrawPrimitive::Points: return GL_POINTS;
		case ph::DrawPrimitive::LineStrip: return GL_LINE_STRIP;
		case ph::DrawPrimitive::LineLoop: return GL_LINE_LOOP;
		case ph::DrawPrimitive::Lines: return GL_LINES;
		case ph::DrawPrimitive::LineStripAdjacency: return GL_LINE_STRIP_ADJACENCY;
		case ph::DrawPrimitive::LinesAdjacency: return GL_LINES_ADJACENCY;
		case ph::DrawPrimitive::TriangleStrip: return GL_TRIANGLE_STRIP;
		case ph::DrawPrimitive::TriangleFan:	return GL_TRIANGLE_FAN;
		case ph::DrawPrimitive::Triangles: return GL_TRIANGLES;
		case ph::DrawPrimitive::TriangleStripAdjacency: GL_TRIANGLE_STRIP_ADJACENCY;
		case ph::DrawPrimitive::TrianglesAdjacency: return GL_TRIANGLES_ADJACENCY;
		default:
			PH_UNEXPECTED_SITUATION("This draw mode is not supported!");
	}
}

}