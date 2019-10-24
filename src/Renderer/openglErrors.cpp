#include "openglErrors.hpp"
#include "GL/glew.h"

namespace ph {
namespace OpenGLErrors {

void clearErrors()
{
	while(glGetError() != GL_NO_ERROR);
}

bool logErrors(const char* functionSignature, const char* fileName, unsigned numberOfLine)
{
	GLenum errorCode = glGetError();
	if(errorCode == GL_NO_ERROR)
		return true;

	std::string error;
	std::string description;
	switch(errorCode) {
	case GL_INVALID_ENUM:
		error = "GL_INVALID_ENUM";
		description = "Enumeration parameter is not a legal enumeration for that function.";
	case GL_INVALID_VALUE:
		error = "GL_INVALID_VALUE";
		description = "Value parameter is not a legal value for that function.";
	case GL_INVALID_OPERATION:
		error = "GL_INVALID_OPERATION";
		description = "the set of state for a command is not legal for the parameters given to that command.\n"
			"It is also given for commands where combinations of parameters define what the legal parameters are.";
	case GL_STACK_OVERFLOW:
		error = "GL_STACK_OVERFLOW";
		description = "stack pushing operation cannot be done because it would overflow the limit of that stack's size.";
	case GL_STACK_UNDERFLOW:
		error = "GL_STACK_UNDERFLOW";
		description = "Stack popping operation cannot be done because the stack is already at its lowest point.";
	case GL_OUT_OF_MEMORY:
		error = "GL_OUT_OF_MEMORY";
		description = "performing an operation that can allocate memory, and the memory cannot be allocated.\n"
			"The results of OpenGL functions that return this error are undefined; it is allowable for partial operations to happen.";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		error = "GL_INVALID_FRAMEBUFFER_OPERATION";
		description = "You do something that would attempt to read from or write/render to a framebuffer that is not complete.";
	}

	// TODO_ren: Make it log info using our logs module instead of std::cout

	std::cout << "OpenGL Error nr " << errorCode << ": " << error << "\n" <<
		"description: " << description << "\n" <<
		"function signature: " << functionSignature << "\n" <<
		"file: " << fileName << "\n" <<
		"line: " << numberOfLine << std::endl;

	return false;
}

}}
