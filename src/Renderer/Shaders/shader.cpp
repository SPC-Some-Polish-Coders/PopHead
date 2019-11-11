#include "shader.hpp"
#include "Renderer/openglErrors.hpp"
#include "Logs/logs.hpp"
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace ph {

Shader::Shader()
{
	mID = glCreateProgram();
}

bool Shader::loadFromFile(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{
	auto vertexShaderCode = getShaderCodeFromFile(vertexShaderFilename);
	auto fragmentShaderCode = getShaderCodeFromFile(fragmentShaderFilename);
	
	if(vertexShaderCode == std::nullopt || fragmentShaderCode == std::nullopt)
		return false;

	loadFromString(vertexShaderCode->c_str(), fragmentShaderCode->c_str());
	return true;
}

auto Shader::getShaderCodeFromFile(const char* filename) -> const std::optional<std::string>
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
		PH_LOG_ERROR("Shader file \"" + std::string(filename) + "\" was not succesfully read. (probably file doesn't exist)");
		return std::nullopt;
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
		std::cout << type + " shader compilation failed:\n" + infoLog << std::endl;
		PH_EXIT_GAME(type + " shader compilation failed:\n" + infoLog);
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
		PH_EXIT_GAME("Shader linking error:\n" + std::string(infoLog));
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

void Shader::setUniformUnsignedInt(const std::string& name, const unsigned value) const
{
	GLCheck(glUniform1ui(getUniformLocation(name), value));
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
	GLCheck(glUniform4f(getUniformLocation(name),
		static_cast<float>(color.r) / 255.f, static_cast<float>(color.g) / 255.f,
		static_cast<float>(color.b) / 255.f, static_cast<float>(color.a) / 255.f
	));
}

void Shader::setUniformVector4(const std::string& name, const float x, const float y, const float z, const float w) const
{
	GLCheck(glUniform4f(getUniformLocation(name), x, y, z, w));
}

void Shader::setUniformVector4Rect(const std::string& name, const FloatRect& r) const
{
	setUniformVector4(name, r.left, r.top, r.width, r.height);
}

void Shader::setUniformMatrix4x4(const std::string& name, const float* transform) const
{
	GLCheck(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, transform));
}

int Shader::getUniformLocation(const std::string& name) const
{
	if(mUniformsLocationCache.find(name) != mUniformsLocationCache.end())
		return mUniformsLocationCache[name];

	GLCheck(int location = glGetUniformLocation(mID, name.c_str()));
	mUniformsLocationCache[name] = location;
	return location;
}

}
