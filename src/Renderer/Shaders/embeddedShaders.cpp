#include "embeddedShaders.hpp" 
#include <string_view>

namespace ph::shader {

// default framebuffer
static const char* sDefaultFramebufferVS =
	#include "defaultFramebuffer.vs.glsl"
;
static const char* sDefaultFramebufferFS =
	#include "defaultFramebuffer.fs.glsl"
;
ShaderSource defaultFramebufferSrc() 
{
	return ShaderSource{sDefaultFramebufferVS, sDefaultFramebufferFS};
}


// gaussian blur
static const char* sGaussianBlurFS =
	#include "gaussianBlur.fs.glsl"
;
ShaderSource gaussianBlurFramebufferSrc()
{
	return ShaderSource{sDefaultFramebufferVS, sGaussianBlurFS};
}


// light
static const char* sLightVS =
	#include "light.vs.glsl"
;
static const char* sLightFS =
	#include "light.fs.glsl"
;
ShaderSource lightSrc()
{
	return ShaderSource{sLightVS, sLightFS};
}


// quad 
static const char* sInstancedSpriteVS = 
	#include "quad.vs.glsl"
;
static const char* sInstancedSpriteFS =
	#include "quad.fs.glsl"
;
ShaderSource quadSrc()
{
	return ShaderSource{sInstancedSpriteVS, sInstancedSpriteFS};
}


// line
static const char* sLineVS =
	#include "line.vs.glsl"
;
static const char* sLineFS =
	#include "line.fs.glsl"
;
ShaderSource lineSrc()
{
	return ShaderSource{sLineVS, sLineFS};
}

// points
static const char* sPointsVS =
	#include "points.vs.glsl"
;
static const char* sPointsFS =
	#include "points.fs.glsl"
;
ShaderSource pointsSrc()
{
	return ShaderSource{sPointsVS, sPointsFS};
}


// text
static const char* sTextVS =
	#include "text.vs.glsl"
;
static const char* sTextFS =
	#include "text.fs.glsl"
;
ShaderSource textSrc()
{
	return ShaderSource{sTextVS, sTextFS};
}


// debug text
static const char* sDebugTextVS =
	#include "debugText.vs.glsl"
;
static const char* sDebugTextFS =
	#include "debugText.fs.glsl"
;
ShaderSource debugTextSrc()
{
	return ShaderSource{sDebugTextVS, sDebugTextFS};
}


// debug text background
static const char* sDebugTextBackgroundVS =
	#include "debugTextBackground.vs.glsl"
;
static const char* sDebugTextBackgroundFS =
	#include "debugTextBackground.fs.glsl"
;
ShaderSource debugTextBackgroundSrc()
{
	return ShaderSource{sDebugTextBackgroundVS, sDebugTextBackgroundFS};
}


// font bitmap debug
static const char* sFontBitmapDebugVS = 
	#include "fontBitmapDebug.vs.glsl"
;
static const char* sFontBitmapDebugFS = 
	#include "fontBitmapDebug.fs.glsl"
;
ShaderSource fontBitmapDebugSrc()
{
	return ShaderSource{sFontBitmapDebugVS, sFontBitmapDebugFS};
}


};

