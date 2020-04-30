#include "embeddedShaders.hpp" 

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


// circle 
static const char* sCircleVS = 
	#include "circle.vs.glsl"
;
static const char* sCircleFS =
	#include "circle.fs.glsl"
;
ShaderSource circleSrc()
{
	return ShaderSource{sCircleVS, sCircleFS};
}

// quad 
static const char* sQuadVS = 
	#include "quad.vs.glsl"
;
static const char* sQuadFS =
	#include "quad.fs.glsl"
;
ShaderSource quadSrc()
{
	return ShaderSource{sQuadVS, sQuadFS};
}



// ground chunk 
static const char* sChunkVS = 
	#include "chunk.vs.glsl"
;
static const char* sChunkFS =
	#include "chunk.fs.glsl"
;
ShaderSource chunkSrc()
{
	return ShaderSource{sChunkVS, sChunkFS};
}


// ground chunk 
static const char* sGroundChunkVS = 
	#include "groundChunk.vs.glsl"
;
static const char* sGroundChunkFS =
	#include "groundChunk.fs.glsl"
;
ShaderSource groundChunkSrc()
{
	return ShaderSource{sGroundChunkVS, sGroundChunkFS};
}


// gui
static const char* sGuiFS =
	#include "gui.fs.glsl"
;
ShaderSource guiSrc()
{
	return ShaderSource{sQuadVS, sGuiFS};
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

