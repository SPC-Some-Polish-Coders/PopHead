#include "pch.hpp"
#include "shader.hpp"
#include "openglErrors.hpp"

namespace ph {

void Shader::init(ShaderSource& ss)
{
	mID = glCreateProgram();
	i32 vertexShader = compileShaderAndGetId(ss.vertexShader, GL_VERTEX_SHADER);
	i32 fragmentShader = compileShaderAndGetId(ss.fragmentShader, GL_FRAGMENT_SHADER);

	GLCheck( glAttachShader(mID, vertexShader) );
	GLCheck( glAttachShader(mID, fragmentShader) );
	GLCheck( glLinkProgram(mID) );
	checkLinkingErrors();

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

i32 Shader::compileShaderAndGetId(const char* sourceCode, u32 shaderType)
{
	u32 shaderId = glCreateShader(shaderType);
	GLCheck( glShaderSource(shaderId, 1, &sourceCode, Null) );
	GLCheck( glCompileShader(shaderId) );
	checkCompilationErrors(shaderId, shaderType);
	return shaderId;
}

void Shader::checkCompilationErrors(u32 shaderId, u32 shaderType)
{
	#ifndef PH_DISTRIBUTION
	i32 success;
	char infoLog[200];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if(!success) 
	{
		glGetShaderInfoLog(shaderId, sizeof(infoLog), NULL, infoLog);
		std::string type = shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
		std::cout << type + " shader compilation failed:\n" + infoLog << std::endl;
		PH_EXIT_GAME(type + " shader compilation failed:\n" + infoLog);
	}
	#endif
}

void Shader::checkLinkingErrors()
{
	#ifndef PH_DISTRIBUTION
	i32 success;
	char infoLog[200];
	glGetProgramiv(mID, GL_LINK_STATUS, &success);
	if(!success) 
	{
		glGetProgramInfoLog(mID, sizeof(infoLog), NULL, infoLog);
		PH_EXIT_GAME("Shader linking error:\n" + std::string(infoLog));
	}
	#endif
}

void Shader::remove()
{
	glDeleteProgram(mID);
}

void Shader::bind() const
{
	GLCheck( glUseProgram(mID) );
}

void Shader::initUniformBlock(const char* uniformBlockName, u32 uniformBlockBinding)
{
	bind();
	GLCheck( u32 uniformBlockIndex = glGetUniformBlockIndex(mID, uniformBlockName) );
	GLCheck( glUniformBlockBinding(mID, uniformBlockIndex, uniformBlockBinding) );
}

void Shader::setUniformBool(const char* name, bool value) const
{
	GLCheck( glUniform1i(getUniformLocation(name), Cast<i32>(value)) );
}

void Shader::setUniformI32(const char* name, i32 value) const
{
	GLCheck( glUniform1i(getUniformLocation(name), value) );
}

void Shader::setUniformU32(const char* name, u32 value) const
{
	GLCheck( glUniform1ui(getUniformLocation(name), value) );
}

void Shader::setUniformFloat(const char* name, float value) const
{
	GLCheck( glUniform1f(getUniformLocation(name), value) );
}

void Shader::setUniformVec2(const char* name, Vec2 value) const
{
	GLCheck( glUniform2f(getUniformLocation(name), value.x, value.y) );
}

void Shader::setUniformVec4(const char* name, Vec4 v) const
{
	glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
}

void Shader::setUniformVec4Color(const char* name, sf::Color color) const
{
	setUniformVec4(name, castToNormalizedColorVec4(color));
}

void Shader::setUniformVec4Rect(const char* name, const FloatRect& r) const
{
	GLCheck( glUniform4f(getUniformLocation(name), r.x, r.y, r.w, r.h) );
}

void Shader::setUniformMatrix4x4(const char* name, float* transform) const
{
	GLCheck( glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, transform) );
}

void Shader::setUniformFloatArray(const char* name, i32 count, float* data) const
{
	GLCheck( glUniform1fv(getUniformLocation(name), count, data) );
}

void Shader::setUniformI32Array(const char* name, i32 count, i32* data) const
{
	GLCheck( glUniform1iv(getUniformLocation(name), count, data) );
}

i32 Shader::getUniformLocation(const char* name) const
{
	if(mUniformsLocationCache.find(name) != mUniformsLocationCache.end())
		return mUniformsLocationCache[name];

	GLCheck( i32 location = glGetUniformLocation(mID, name) );
	mUniformsLocationCache[name] = location;
	return location;
}

}

