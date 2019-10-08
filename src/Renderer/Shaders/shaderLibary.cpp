#include "shaderLibary.hpp"
#include "Logs/logs.hpp"

namespace ph {

void ShaderLibrary::loadFromFile(const std::string& name, const char* vertexShaderFilepath, const char* fragmentShaderFilepath)
{
	Shader shader;
	shader.loadFromFile(vertexShaderFilepath, fragmentShaderFilepath);
	mShaders[name] = shader;
}

void ShaderLibrary::loadFromString(const std::string& name, const char* vertexShaderCode, const char* fragmentShaderCode)
{
	Shader shader;
	shader.loadFromString(vertexShaderCode, fragmentShaderCode);
	mShaders[name] = shader;
}

Shader& ShaderLibrary::get(const std::string& name)
{
	auto found = mShaders.find(name);
	if(found == mShaders.end()) {
		PH_EXIT_GAME("You try to get a shader that wasn't loaded: " + name);
	}
	return found->second;
}

}
