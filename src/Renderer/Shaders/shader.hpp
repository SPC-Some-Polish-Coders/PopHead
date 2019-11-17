#pragma once

#include "Utilities/rect.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>
#include <unordered_map>
#include <optional>

namespace ph {

class Shader
{
public:
	Shader();

	bool loadFromFile(const char* vertexShaderFilename, const char* fragmentShaderFilename);
	void loadFromString(const char* vertexShaderSource, const char* fragmentShaderSource);

	void bind() const;
	void unbind() const;

	void setUniformBool(const char* name, const bool value) const;
	void setUniformInt(const char* name, const int value) const;
	void setUniformUnsignedInt(const char* name, const unsigned value) const;
	void setUniformFloat(const char* name, const float value) const;
	void setUniformVector2(const char* name, const sf::Vector2f value) const;
	void setUniformVector2(const char* name, const float x, const float y) const;
	void setUniformVector3(const char* name, const sf::Vector3f value) const;
	void setUniformVector3(const char* name, const float x, const float y, const float z) const;
	void setUniformVector4Color(const char* name, const sf::Color&) const;
	void setUniformVector4(const char* name, const float x, const float y, const float z, const float w) const;
	void setUniformVector4Rect(const char* name, const FloatRect&) const;
	void setUniformMatrix4x4(const char* name, const float* transform) const;

private:
	auto getShaderCodeFromFile(const char* filename) -> const std::optional<std::string>;
	int compileShaderAndGetId(const char* sourceCode, const unsigned shaderType);
	void checkCompilationErrors(const unsigned shaderId, const unsigned shaderType);
	void linkProgram(const int vertexShaderId, const int fragmentShaderId);
	void checkLinkingErrors();

	int getUniformLocation(const char* name) const;

private:
	mutable std::unordered_map<std::string, int> mUniformsLocationCache;
	unsigned mID;
};

}
