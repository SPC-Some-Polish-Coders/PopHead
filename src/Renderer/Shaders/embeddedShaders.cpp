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


// gaussian blur
static const char* sGaussianBlurFS =
	#include "gaussianBlur.fs.glsl"
;
ShaderSource gaussianBlurFramebufferSrc()
{
	return ShaderSource{sDefaultFramebufferVS, sGaussianBlurFS};
}


// instanced sprite
static const char* sInstancedSpriteVS = 
	#include "instancedSprite.vs.glsl"
;
static const char* sInstancedSpriteFS =
	#include "instancedSprite.fs.glsl"
;
ShaderSource instancedSpriteSrc()
{
	return ShaderSource{sInstancedSpriteVS, sInstancedSpriteFS};
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

};

