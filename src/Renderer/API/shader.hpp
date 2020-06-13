#pragma once

#include "Utilities/rect.hpp"
#include <SFML/Graphics/Color.hpp>
#include <string>
#include <unordered_map>

namespace ph {

struct ShaderSource
{
	const char* vertexShader;
	const char* fragmentShader;
};

class Shader
{
public:
	void init(ShaderSource&);
	void remove();

	void bind() const;
	void unbind() const;

	void initUniformBlock(const char* uniformBlockName, u32 uniformBlockBinding);

	void setUniformBool(const char* name, bool value) const;
	void setUniformI32(const char* name, i32 value) const;
	void setUniformU32(const char* name, u32 value) const;
	void setUniformFloat(const char* name, float value) const;
	void setUniformVec2(const char* name, Vec2 value) const;
	void setUniformVec4(const char* name, Vec4 v) const;
	void setUniformVec4Color(const char* name, sf::Color) const;
	void setUniformVec4Rect(const char* name, const FloatRect&) const;
	void setUniformMatrix4x4(const char* name, float* transform) const;
	void setUniformFloatArray(const char* name, i32 count, float* data) const;
	void setUniformI32Array(const char* name, i32 count, i32* data) const;
	
	u32 getID() const { return mID; }

private:
	i32 compileShaderAndGetId(const char* sourceCode, u32 shaderType);
	void checkCompilationErrors(u32 shaderId, u32 shaderType);
	void linkProgram(i32 vertexShaderId, i32 fragmentShaderId);
	void checkLinkingErrors();

	i32 getUniformLocation(const char* name) const;

private:
	mutable std::unordered_map<std::string, i32> mUniformsLocationCache;
	u32 mID;
};

}

