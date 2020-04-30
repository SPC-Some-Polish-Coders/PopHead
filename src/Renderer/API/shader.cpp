#include "pch.hpp"
#include "shader.hpp"
#include "openglErrors.hpp"

namespace ph {

void Shader::init(ShaderSource& ss)
{
	mID = glCreateProgram();
	int vertexShader = compileShaderAndGetId(ss.vertexShader, GL_VERTEX_SHADER);
	int fragmentShader = compileShaderAndGetId(ss.fragmentShader, GL_FRAGMENT_SHADER);

	GLCheck( glAttachShader(mID, vertexShader) );
	GLCheck( glAttachShader(mID, fragmentShader) );
	GLCheck( glLinkProgram(mID) );
	checkLinkingErrors();

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

int Shader::compileShaderAndGetId(const char* sourceCode, const unsigned shaderType)
{
	unsigned shaderId = glCreateShader(shaderType);
	GLCheck( glShaderSource(shaderId, 1, &sourceCode, nullptr) );
	GLCheck( glCompileShader(shaderId) );
	checkCompilationErrors(shaderId, shaderType);
	return shaderId;
}

void Shader::checkCompilationErrors(const unsigned shaderId, const unsigned shaderType)
{
	#ifndef PH_DISTRIBUTION
	int success;
	char infoLog[200];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if(!success) {
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
	int success;
	char infoLog[200];
	glGetProgramiv(mID, GL_LINK_STATUS, &success);
	if(!success) {
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

void Shader::initUniformBlock(const char* uniformBlockName, unsigned uniformBlockBinding)
{
	bind();
	GLCheck( unsigned uniformBlockIndex = glGetUniformBlockIndex(mID, uniformBlockName) );
	GLCheck( glUniformBlockBinding(mID, uniformBlockIndex, uniformBlockBinding) );
}

void Shader::setUniformBool(const char* name, const bool value) const
{
	GLCheck( glUniform1i(getUniformLocation(name), static_cast<int>(value)) );
}

void Shader::setUniformInt(const char* name, const int value) const
{
	GLCheck( glUniform1i(getUniformLocation(name), value) );
}

void Shader::setUniformUnsignedInt(const char* name, const unsigned value) const
{
	GLCheck( glUniform1ui(getUniformLocation(name), value) );
}

void Shader::setUniformFloat(const char* name, const float value) const
{
	GLCheck( glUniform1f(getUniformLocation(name), value) );
}

void Shader::setUniformVector2(const char* name, const sf::Vector2f value) const
{
	GLCheck( glUniform2f(getUniformLocation(name), value.x, value.y) );
}

void Shader::setUniformVector2(const char* name, const float x, const float y) const
{
	GLCheck( glUniform2f(getUniformLocation(name), x, y) );
}

void Shader::setUniformVector3(const char* name, const sf::Vector3f value) const
{
	GLCheck( glUniform3f(getUniformLocation(name), value.x, value.y, value.z) );
}

void Shader::setUniformVector3(const char* name, const float x, const float y, const float z) const
{
	GLCheck( glUniform3f(getUniformLocation(name), x, y, z) );
}

void Shader::setUniformVector4Color(const char* name, const sf::Color& color) const
{
	GLCheck( glUniform4f(getUniformLocation(name),
		static_cast<float>(color.r) / 255.f, static_cast<float>(color.g) / 255.f,
		static_cast<float>(color.b) / 255.f, static_cast<float>(color.a) / 255.f
	) );
}

void Shader::setUniformVector4(const char* name, const float x, const float y, const float z, const float w) const
{
	GLCheck( glUniform4f(getUniformLocation(name), x, y, z, w) );
}

void Shader::setUniformVector4Rect(const char* name, const FloatRect& r) const
{
	setUniformVector4(name, r.left, r.top, r.width, r.height);
}

void Shader::setUniformMatrix4x4(const char* name, const float* transform) const
{
	GLCheck( glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, transform) );
}

void Shader::setUniformFloatArray(const char* name, int count, const float* data) const
{
	GLCheck( glUniform1fv(getUniformLocation(name), count, data) );
}

void Shader::setUniformIntArray(const char* name, int count, const int* data) const
{
	GLCheck( glUniform1iv(getUniformLocation(name), count, data) );
}

int Shader::getUniformLocation(const char* name) const
{
	if(mUniformsLocationCache.find(name) != mUniformsLocationCache.end())
		return mUniformsLocationCache[name];

	GLCheck( int location = glGetUniformLocation(mID, name) );
	mUniformsLocationCache[name] = location;
	return location;
}

}

