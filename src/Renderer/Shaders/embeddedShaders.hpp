#pragma once

#include "Renderer/API/shader.hpp"

namespace ph::shader {

ShaderSource defaultFramebufferSrc();
ShaderSource fontBitmapDebugSrc();
ShaderSource gaussianBlurFramebufferSrc();
ShaderSource instancedSpriteSrc();
ShaderSource lightSrc();
ShaderSource lineSrc();
ShaderSource pointsSrc();
ShaderSource textSrc();

}

