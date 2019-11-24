#include "glEnums.hpp"
#include "GL/glew.h"
#include "Logs/logs.hpp"

namespace ph {

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
