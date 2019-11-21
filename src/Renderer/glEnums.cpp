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

unsigned toGLEnum(DataUsage allocationMode)
{
	switch(allocationMode)
	{
		case ph::DataUsage::streamDraw: return GL_STREAM_DRAW;
		case ph::DataUsage::staticDraw: return GL_STATIC_DRAW;
		case ph::DataUsage::dynamicDraw: return GL_DYNAMIC_DRAW;
		default:
			PH_UNEXPECTED_SITUATION("This allocation mode is not supported!");
	}
}

}