#pragma once

#include "GL/glew.h"
#include <iostream>

namespace ph {

namespace OpenGLErrors
{
#ifndef PH_DISTRIBUTION
#ifdef _MSC_VER
#define ASSERT(x) if(!(x)) __debugbreak();
#else
#define ASSERT(x) x; 
#endif

#define GLCheck(x) \
		OpenGLErrors::clearErrors(); \
		x; \
		ASSERT(OpenGLErrors::logErrors(#x, __FILE__, __LINE__));

void clearErrors();
bool logErrors(const char* functionSignature, const char* fileName, unsigned numberOfLine);

#else
#define ASSERT(x) x;
#define GLCheck(x) x;
#endif
}

}
