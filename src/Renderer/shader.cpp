#include "shader.hpp"
#include "openglErrors.hpp"
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace ph {

Shader::Shader()
{
	mID = glCreateProgram();
}

void Shader::loadFromFile(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{
	const std::string vertexShaderCode = getShaderCodeFromFile(vertexShaderFilename);
	const std::string fragmentShaderCode = getShaderCodeFromFile(fragmentShaderFilename);
	loadFromString(vertexShaderCode.c_str(), fragmentShaderCode.c_str());
}

const std::string Shader::getShaderCodeFromFile(const char* filename)
{
	std::string code;
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		file.open(filename);
		std::stringstream stream;
		stream << file.rdbuf();
		file.close();
		code = stream.str();
	}
	catch(std::istream::failure) {
		throw std::runtime_error("Shader file was not succesfully read.");
	}
	return code;
}

void Shader::loadFromString(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	int vertexShaderId = compileShaderAndGetId(vertexShaderSource, GL_VERTEX_SHADER);
	int fragmentShaderId = compileShaderAndGetId(fragmentShaderSource, GL_FRAGMENT_SHADER);
	linkProgram(vertexShaderId, fragmentShaderId);
}

int Shader::compileShaderAndGetId(const char* sourceCode, const unsigned shaderType)
{
	unsigned shaderId = glCreateShader(shaderType);
	GLCheck(glShaderSource(shaderId, 1, &sourceCode, nullptr));
	GLCheck(glCompileShader(shaderId));
	checkCompilationErrors(shaderId, shaderType);
	return shaderId;
}

void Shader::checkCompilationErrors(const unsigned shaderId, const unsigned shaderType)
{
	int success;
	char infoLog[200];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(shaderId, sizeof(infoLog), NULL, infoLog);
		std::string type = shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
		std::cout << type << " shader compilation failed:\n" << infoLog << std::endl;
	}
}

void Shader::linkProgram(const int vertexShaderId, const int fragmentShaderId)
{
	GLCheck(glAttachShader(mID, vertexShaderId));
	GLCheck(glAttachShader(mID, fragmentShaderId));
	GLCheck(glLinkProgram(mID));
	checkLinkingErrors();
}

void Shader::checkLinkingErrors()
{
	int success;
	char infoLog[200];
	glGetProgramiv(mID, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(mID, sizeof(infoLog), NULL, infoLog);
		std::cout << "Shader linking error:\n" << infoLog << std::endl;
	}
}

void Shader::bind() const
{
	GLCheck(glUseProgram(mID));
}

void Shader::unbind() const
{
	GLCheck(glUseProgram(0));
}

void Shader::setUniformBool(const std::string& name, const bool value) const
{
	GLCheck(glUniform1i(getUniformLocation(name), static_cast<int>(value)));
}

void Shader::setUniformInt(const std::string& name, const int value) const
{
	GLCheck(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniformFloat(const std::string& name, const float value) const
{
	GLCheck(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniformVector2(const std::string& name, const sf::Vector2f value) const
{
	GLCheck(glUniform2f(getUniformLocation(name), value.x, value.y));
}

void Shader::setUniformVector2(const std::string& name, const float x, const float y) const
{
	GLCheck(glUniform2f(getUniformLocation(name), x, y));
}

void Shader::setUniformVector3(const std::string& name, const sf::Vector3f value) const
{
	GLCheck(glUniform3f(getUniformLocation(name), value.x, value.y, value.z));
}

void Shader::setUniformVector3(const std::string& name, const float x, const float y, const float z) const
{
	GLCheck(glUniform3f(getUniformLocation(name), x, y, z));
}

void Shader::setUniformVector4Color(const std::string& name, const sf::Color& color) const
{
	GLCheck(glUniform4f(getUniformLocation(name), color.r / 255, color.g / 255, color.b / 255, color.a / 255));
}

void Shader::setUniformVector4(const std::string& name, const float x, const float y, const float z, const float w) const
{
	GLCheck(glUniform4f(getUniformLocation(name), x, y, z, w));
}

void Shader::setUniformMatrix3x3(const std::string& name, const sf::Transform& transform) const
{
	glUniform3fv(getUniformLocation(name), 1, transform.getMatrix());
}

int Shader::getUniformLocation(const std::string& name) const
{
	if(mUniformsLocationCache.find(name) != mUniformsLocationCache.end())
		return mUniformsLocationCache[name];

	int location = glGetUniformLocation(mID, name.c_str());
	mUniformsLocationCache[name] = location;
	return location;
}

}