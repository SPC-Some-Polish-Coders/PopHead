#include "pch.hpp"
#include "renderer.hpp"
#include "MinorRenderers/quadRenderer.hpp"
#include "MinorRenderers/lineRenderer.hpp"
#include "MinorRenderers/pointRenderer.hpp"
#include "MinorRenderers/LightRenderer.hpp"
#include "MinorRenderers/TextRenderer.hpp"
#include "API/shader.hpp"
#include "API/font.hpp"
#include "API/openglErrors.hpp"
#include "API/framebuffer.hpp"
#include "Shaders/embeddedShaders.hpp"
#include "Utilities/vector4.hpp"
#include "Utilities/mat4.hpp"
#include "Utilities/cast.hpp"

extern bool debugWindowOpen;

namespace ph::Renderer {

namespace {
	FloatRect gameWorldCameraBounds;
	mat4 gameWorldVPM;

	Shader defaultFramebufferShader;
	Shader gaussianBlurFramebufferShader;
	Shader circleShader;
	
	Framebuffer gameObjectsFramebuffer;
	Framebuffer lightingFramebuffer;
	Framebuffer lightingGaussianBlurFramebuffer;
	u32 screenVBO;
	u32 screenIBO;
	u32 screenVAO;
	 
	sf::Color ambientLightColor;

	u32 sharedDataUBO;

	PointRenderer pointRenderer;

	bool isDebugDisplayActive = false;

	#ifndef PH_DISTRIBUTION
	std::function<void(void)> renderSystemDebug;
	#endif
}

static void setClearColor(sf::Color color)
{
	GLCheck( glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f) );
}

static float getNormalizedZ(const u8 z)
{
	return z / 255.f;
}

void init(u32 screenWidth, u32 screenHeight)
{
	// initialize glew
	glewExperimental = GL_TRUE;
	PH_ASSERT_CRITICAL(glewInit() == GLEW_OK, "GLEW wasn't initialized correctly!");

	// initialize minor renderers
	QuadRenderer::setGameWorldCameraBoundsPtr(&gameWorldCameraBounds);
	pointRenderer.setGameWorldCameraBoundsPtr(&gameWorldCameraBounds);
	LineRenderer::setGameWorldCameraBoundsPtr(&gameWorldCameraBounds);
	LightRenderer::setGameWorldCameraBoundsPtr(&gameWorldCameraBounds);
	QuadRenderer::init();
	LineRenderer::init();
	pointRenderer.init();
	LightRenderer::init();
	TextRenderer::init();

	// set up blending
	GLCheck( glEnable(GL_BLEND) );
	GLCheck( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

	// set up uniform buffer object
	glGenBuffers(1, &sharedDataUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, sharedDataUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(mat4), Null, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, sharedDataUBO, 0, 2 * sizeof(mat4));

	// initialize gui view projection matrix
	mat4 guiVPM = makeOrthographic(FloatRect(0.f, 0.f, 1920.f, 1080.f));
	GLCheck( glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), guiVPM.e) );

	// init shaders
	circleShader.init(shader::circleSrc());
	defaultFramebufferShader.init(shader::defaultFramebufferSrc());
	gaussianBlurFramebufferShader.init(shader::gaussianBlurFramebufferSrc());

	// set up framebuffers
	gameObjectsFramebuffer.init(screenWidth, screenHeight);
	lightingFramebuffer.init(screenWidth, screenHeight);
	lightingGaussianBlurFramebuffer.init(screenWidth, screenHeight);

	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);

	float framebufferQuad[] = {
		1.f,-1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f,
	   -1.f, 1.f, 0.f, 1.f,
	   -1.f,-1.f, 0.f, 0.f
	};
	glGenBuffers(1, &screenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(framebufferQuad), framebufferQuad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), Null); 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); 

	u32 quadIndices[] = { 0, 1, 3, 1, 2, 3 };
	glGenBuffers(1, &screenIBO); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
}

void restart(u32 screenWidth, u32 screenHeight)
{
	shutDown();
	init(screenWidth, screenHeight);
}

void shutDown()
{
	QuadRenderer::shutDown();
	LineRenderer::shutDown();
	LightRenderer::shutDown();
	TextRenderer::shutDown();
	gameObjectsFramebuffer.remove();
	lightingFramebuffer.remove();
	lightingGaussianBlurFramebuffer.remove();
	defaultFramebufferShader.remove();
	gaussianBlurFramebufferShader.remove();
	glDeleteBuffers(1, &screenVBO);
	glDeleteBuffers(1, &screenIBO);
	glDeleteVertexArrays(1, &screenVAO);
}

void setGameWorldCamera(FloatRect bounds)
{
	gameWorldCameraBounds = bounds;
	gameWorldVPM = makeOrthographic(bounds);
}

void beginScene()
{
	PH_PROFILE_FUNCTION();

	gameObjectsFramebuffer.bind();
	setClearColor(sf::Color::Black);
	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );

	GLCheck( glBindBuffer(GL_UNIFORM_BUFFER, sharedDataUBO) );
	GLCheck( glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), gameWorldVPM.e) );
}

void endScene()
{
	PH_PROFILE_FUNCTION();

	bool rendererDebugTabActive = false; 
	if(debugWindowOpen)
	{
		rendererDebugTabActive = ImGui::BeginTabItem("renderer debug");
		if(rendererDebugTabActive)
			ImGui::BeginTabBar("renderer debug tabs");
	}

	// render scene
	QuadRenderer::flush(true);
	pointRenderer.flush();
	LineRenderer::flush();

	// render font debug
	if(FontDebugRenderer::isActive())
		FontDebugRenderer::draw();

	// render lights to lighting framebuffer
	lightingFramebuffer.bind();
	setClearColor(ambientLightColor);
	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );
	LightRenderer::flush();

	// use framebuffer vao for both lightingBlurFramebuffer and for default framebuffer
	glBindVertexArray(screenVAO);

	// apply gaussian blur for lighting
	lightingGaussianBlurFramebuffer.bind();
	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );
	lightingFramebuffer.bindTextureColorBuffer(0);
	gaussianBlurFramebufferShader.bind();
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );

	// render everything onto quad in default framebuffer
	GLCheck( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );
	GLCheck( glEnable(GL_FRAMEBUFFER_SRGB) );
	defaultFramebufferShader.bind();
	defaultFramebufferShader.setUniformI32("gameObjectsTexture", 0);
	gameObjectsFramebuffer.bindTextureColorBuffer(0);
	defaultFramebufferShader.setUniformI32("lightingTexture", 1);
	lightingGaussianBlurFramebuffer.bindTextureColorBuffer(1);
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );
	GLCheck( glDisable(GL_FRAMEBUFFER_SRGB) );
	QuadRenderer::flush(false);

	if(rendererDebugTabActive)
	{
		renderSystemDebug();
		QuadRenderer::submitDebug();
		LightRenderer::submitDebug(&ambientLightColor);
		pointRenderer.submitDebug();
		LineRenderer::submitDebug();
		ImGui::EndTabBar();
		ImGui::EndTabItem();
	}
}

void submitCircle(sf::Color color, Vec2 pos, float radius, u8 z, 
				  ProjectionType projectionType, bool isAffectedByLight)
{
	submitQuad(Null, Null, &color, &circleShader, pos, Vec2(2 * radius), z, 0.f, {}, projectionType, isAffectedByLight);
}

void submitQuad(Texture* texture, const IntRect* textureRect, const sf::Color* color, const Shader* shader,
                Vec2 pos, Vec2 size, u8 z, float rotation, Vec2 rotationOrigin,
                ProjectionType projectionType, bool isAffectedByLight)
{
	QuadRenderer::submitQuad(texture, textureRect, color, shader, pos, size,
	                         getNormalizedZ(z), rotation, rotationOrigin, projectionType, isAffectedByLight);
}

void submitQuad(Texture* texture, const IntRect* textureRect, const sf::Color* color, const Shader* shader,
				FloatRect posAndSize, u8 z, float rotation, Vec2 rotationOrigin, ProjectionType projectionType,
				bool isAffectedByLight)
{	
	QuadRenderer::submitQuad(texture, textureRect, color, shader, posAndSize.pos, posAndSize.size,
	                         getNormalizedZ(z), rotation, rotationOrigin, projectionType, isAffectedByLight);
}

void submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>& qd, Texture* t, const Shader* s,
                                          u8 z, ProjectionType projectionType)
{
	QuadRenderer::submitBunchOfQuadsWithTheSameTexture(qd, t, s, getNormalizedZ(z), projectionType);
}

void setChunksTexture(const Texture& texture)
{
	QuadRenderer::setChunksTexture(texture.getID());	
}

u32 registerNewChunk(const FloatRect& bounds)
{
	return QuadRenderer::registerNewChunk(bounds);
}

void submitChunk(std::vector<ChunkQuadData>& quadsData,
                 const FloatRect& bounds, u8 z, u32* rendererID, sf::Color color)
{
	QuadRenderer::submitChunk(quadsData, bounds, getNormalizedZ(z), rendererID, color);
}

void submitGroundChunk(Vec2 pos, const FloatRect& textureRect, u8 z, sf::Color color)  
{
	QuadRenderer::submitGroundChunk(pos, textureRect, getNormalizedZ(z), color);
}

void submitLine(sf::Color color, Vec2 posA, Vec2 posB, float thickness)
{
	submitLine(color, color, posA, posB, thickness);
}

void submitLine(sf::Color colorA, sf::Color colorB, Vec2 posA, Vec2 posB, float thickness)
{
	LineRenderer::submitLine(colorA, colorB, posA, posB, thickness);
}

void submitPoint(Vec2 pos, sf::Color color, float size)
{
	pointRenderer.submitPoint(pos, color, size);
}

void submitLight(sf::Color color, Vec2 pos, float startAngle, float endAngle,
                 float attenuationAddition, float attenuationFactor, float attenuationSquareFactor, bool rayCollisionDetection) 
{
	LightRenderer::submitLight(color, pos, startAngle, endAngle, attenuationAddition, attenuationFactor, attenuationSquareFactor, rayCollisionDetection);
}

void submitLightWall(FloatRect wall)
{
	LightRenderer::submitLightWall(wall);
}

void submitBunchOfLightWalls(const std::vector<FloatRect>& walls)
{
	LightRenderer::submitBunchOfLightWalls(walls);
}

u32 getNrOfCollisionLights()
{
	return LightRenderer::getNrOfCollisionLights();
}

void submitText(const char* text, const char* fontFilename, Vec2 pos, float characterSize, sf::Color color,
                u8 z, ProjectionType projecitonType, bool isAffectedByLight)
{
	TextRenderer::drawText(text, fontFilename, pos, characterSize, color, z, projecitonType, isAffectedByLight);
}

void submitTextWorldHD(const char* text, const char* fontFilename, Vec2 worldPos, 
                       float characterSize, sf::Color textColor, u8 z)
{
	TextRenderer::drawTextWorldHD(text, fontFilename, worldPos, gameWorldCameraBounds, characterSize, textColor, z);
}

void submitTextArea(const char* text, const char* fontFilename, Vec2 pos, float textAreaWidth,
                    TextAligment aligment, float size, sf::Color color, u8 z, ProjectionType projectionType, bool isAffectedByLight)
{
	TextRenderer::drawTextArea(text, fontFilename, pos, textAreaWidth, aligment, size, color, z, projectionType, isAffectedByLight);
}

#ifndef PH_DISTRIBUTION
void submitRenderSystemDebug(const std::function<void(void)>& fn)
{
	renderSystemDebug = fn;
}
#endif

void handleEvent(sf::Event e)
{
	if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F3) 
	{
		isDebugDisplayActive = !isDebugDisplayActive;
	}
	if(e.type == sf::Event::Resized) 
	{
		GLCheck( glViewport(0, 0, e.size.width, e.size.height) );
		gameObjectsFramebuffer.onWindowResize(e.size.width, e.size.height);
		lightingFramebuffer.onWindowResize(e.size.width, e.size.height);
		lightingGaussianBlurFramebuffer.onWindowResize(e.size.width, e.size.height);
	}
}

void setAmbientLightColor(sf::Color color)
{
	ambientLightColor = color;
}

}

