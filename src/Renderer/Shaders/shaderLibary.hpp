#pragma once

#include "shader.hpp"
#include <optional>
#include <map>

namespace ph {

// TODO_ren: Make freeing shaders possible

class ShaderLibrary
{
	ShaderLibrary() {};
public:
	ShaderLibrary(ShaderLibrary&) = delete;
	void operator=(ShaderLibrary const&) = delete;

	static ShaderLibrary& getInstance()
	{
		static ShaderLibrary shaderLibary;
		return shaderLibary;
	}

	void loadFromFile(const std::string& name, const char* vertexShaderFilepath, const char* fragmentShaderFilepath);
	void loadFromString(const std::string& name, const char* vertexShaderCode, const char* fragmentShaderCode);
	Shader* get(const std::string& name);

private:
	std::map<std::string, Shader> mShaders;
};

}
