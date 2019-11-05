#pragma once

namespace ph{

class Texture;
class Shader;

namespace component {

	struct TextureRef
	{
		Texture* texture;
	};

	struct ShaderRef
	{
		Shader* shader;
	};

}}