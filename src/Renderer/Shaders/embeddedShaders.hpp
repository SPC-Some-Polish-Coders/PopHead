#pragma once

#include "Renderer/API/shader.hpp"

namespace ph::shader {

ShaderSource defaultFramebufferSrc();
ShaderSource gaussianBlurFramebufferSrc();
ShaderSource lightSrc();
ShaderSource quadSrc();
ShaderSource lineSrc();
ShaderSource pointsSrc();
ShaderSource textSrc();
ShaderSource fontBitmapDebugSrc();

}

