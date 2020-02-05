#pragma once

#include "Renderer/API/shader.hpp"

namespace ph::shader {

ShaderSource defaultFramebufferSrc();
ShaderSource gaussianBlurFramebufferSrc();
ShaderSource lightSrc();
ShaderSource localIlluminationSrc();
ShaderSource quadSrc();
ShaderSource lineSrc();
ShaderSource pointsSrc();
ShaderSource textSrc();
ShaderSource fontBitmapDebugSrc();

}

