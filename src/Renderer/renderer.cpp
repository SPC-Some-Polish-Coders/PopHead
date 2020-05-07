#include "pch.hpp"
#include "renderer.hpp"
#include "MinorRenderers/quadRenderer.hpp"
#include "MinorRenderers/lineRenderer.hpp"
#include "MinorRenderers/pointRenderer.hpp"
#include "MinorRenderers/lightRenderer.hpp"
#include "MinorRenderers/TextRenderer.hpp"
#include "API/shader.hpp"
#include "API/camera.hpp"
#include "API/font.hpp"
#include "API/openglErrors.hpp"
#include "API/framebuffer.hpp"
#include "Shaders/embeddedShaders.hpp"
#include "Utilities/vector4.hpp"
#include "Utilities/cast.hpp"

extern bool debugWindowOpen;

namespace {
	ph::FloatRect screenBounds;

	ph::Shader defaultFramebufferShader;
	ph::Shader gaussianBlurFramebufferShader;
	ph::Shader circleShader;
	
	ph::Framebuffer gameObjectsFramebuffer;
	ph::Framebuffer lightingFramebuffer;
	ph::Framebuffer lightingGaussianBlurFramebuffer;
	unsigned screenVBO;
	unsigned screenIBO;
	unsigned screenVAO;
	 
	sf::Color ambientLightColor;

	unsigned sharedDataUBO;

	ph::PointRenderer pointRenderer;
	ph::LineRenderer lineRenderer;
	ph::LightRenderer lightRenderer;

	ph::Camera gameWorldCamera; 

	bool isDebugDisplayActive = false;
}

namespace ph::Renderer {

static void setClearColor(sf::Color color)
{
	GLCheck( glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f) );
}

static float getNormalizedZ(const unsigned char z)
{
	return z / 255.f;
}

void init(unsigned screenWidth, unsigned screenHeight)
{
	// initialize glew
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		PH_EXIT_GAME("GLEW wasn't initialized correctly!");

	// initialize minor renderers
	QuadRenderer::setScreenBoundsPtr(&screenBounds);
	pointRenderer.setScreenBoundsPtr(&screenBounds);
	lineRenderer.setScreenBoundsPtr(&screenBounds);
	lightRenderer.setScreenBoundsPtr(&screenBounds);
	QuadRenderer::init();
	lineRenderer.init();
	pointRenderer.init();
	lightRenderer.init();
	TextRenderer::init();

	// set up blending
	GLCheck( glEnable(GL_BLEND) );
	GLCheck( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

	// set up uniform buffer object
	glGenBuffers(1, &sharedDataUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, sharedDataUBO);
	glBufferData(GL_UNIFORM_BUFFER, 32 * sizeof(float), nullptr, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, sharedDataUBO, 0, 32 * sizeof(float));

	// initialize gui view projection matrix
	ph::Camera guiCamera({960, 540}, {1920, 1080});
	const float* guiViewProjectionMatrix = guiCamera.getViewProjectionMatrix4x4().getMatrix();
	GLCheck( glBindBuffer(GL_UNIFORM_BUFFER, sharedDataUBO) );
	GLCheck( glBufferSubData(GL_UNIFORM_BUFFER, 16 * sizeof(float), 16 * sizeof(float), guiViewProjectionMatrix) );

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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); 

	unsigned quadIndices[] = { 0, 1, 3, 1, 2, 3 };
	glGenBuffers(1, &screenIBO); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
}

void restart(unsigned screenWidth, unsigned screenHeight)
{
	shutDown();
	init(screenWidth, screenHeight);
}

void shutDown()
{
	QuadRenderer::shutDown();
	lineRenderer.shutDown();
	lightRenderer.shutDown();
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

void setGameWorldCamera(Camera& camera)
{
	gameWorldCamera = camera;	
}

void beginScene()
{
	PH_PROFILE_FUNCTION();

	gameObjectsFramebuffer.bind();
	setClearColor(sf::Color::Black);
	GLCheck( glEnable(GL_DEPTH_TEST) );
	GLCheck( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );

	const float* viewProjectionMatrix = gameWorldCamera.getViewProjectionMatrix4x4().getMatrix();
	GLCheck( glBindBuffer(GL_UNIFORM_BUFFER, sharedDataUBO) );
	GLCheck( glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), viewProjectionMatrix) );
	
	sf::Vector2f center = gameWorldCamera.center();
	sf::Vector2f size = gameWorldCamera.getSize();
	screenBounds = FloatRect(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);
}

void endScene()
{
	PH_PROFILE_FUNCTION();

	bool rendererDebugTabActive = false; 
	if(debugWindowOpen)
	{
		rendererDebugTabActive = ImGui::BeginTabItem("renderer debug");
		if(rendererDebugTabActive)
		{
			ImGui::BeginTabBar("renderer debug tabs");
		}
	}

	// render scene
	QuadRenderer::flush(true);
	pointRenderer.flush();

	// disable depth test for performance purposes
	GLCheck( glDisable(GL_DEPTH_TEST) );

	// render font debug
	if(FontDebugRenderer::isActive())
		FontDebugRenderer::draw();

	// render lights to lighting framebuffer
	lightingFramebuffer.bind();
	setClearColor(ambientLightColor);
	GLCheck( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
	lightRenderer.flush();

	// user framebuffer vao for both lightingBlurFramebuffer and for default framebuffer
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
	defaultFramebufferShader.setUniformInt("gameObjectsTexture", 0);
	gameObjectsFramebuffer.bindTextureColorBuffer(0);
	defaultFramebufferShader.setUniformInt("lightingTexture", 1);
	lightingGaussianBlurFramebuffer.bindTextureColorBuffer(1);
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );
	GLCheck( glDisable(GL_FRAMEBUFFER_SRGB) );
	QuadRenderer::flush(false);

	if(rendererDebugTabActive)
	{
		QuadRenderer::submitDebug();
		lightRenderer.submitDebug();
		pointRenderer.submitDebug();
		lineRenderer.submitDebug();
		ImGui::EndTabBar();
		ImGui::EndTabItem();
	}
}

void submitCircle(sf::Color color, sf::Vector2f position, float radius, unsigned char z, 
				  ProjectionType projectionType, bool isAffectedByLight)
{
	submitQuad(nullptr, nullptr, &color, &circleShader, position, {radius, radius}, z, 0.f, {}, projectionType, isAffectedByLight);
}

void submitQuad(Texture* texture, const IntRect* textureRect, const sf::Color* color, const Shader* shader,
                sf::Vector2f position, sf::Vector2f size, unsigned char z, float rotation, sf::Vector2f rotationOrigin,
                ProjectionType projectionType, bool isAffectedByLight)
{
	QuadRenderer::submitQuad(texture, textureRect, color, shader, position, size,
	                         getNormalizedZ(z), rotation, rotationOrigin, projectionType, isAffectedByLight);
}

void submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>& qd, Texture* t, const Shader* s,
                                          unsigned char z, ProjectionType projectionType)
{
	QuadRenderer::submitBunchOfQuadsWithTheSameTexture(qd, t, s, getNormalizedZ(z), projectionType);
}

void setChunksTexture(const Texture& texture)
{
	QuadRenderer::setChunksTexture(texture.getID());	
}

unsigned registerNewChunk(const FloatRect& bounds)
{
	return QuadRenderer::registerNewChunk(bounds);
}

void submitChunk(std::vector<ChunkQuadData>& quadsData,
                 const FloatRect& bounds, unsigned char z, unsigned* rendererID, sf::Color color)
{
	QuadRenderer::submitChunk(quadsData, bounds, getNormalizedZ(z), rendererID, color);
}

void submitGroundChunk(sf::Vector2f pos, const FloatRect& textureRect, unsigned char z, sf::Color color)  
{
	QuadRenderer::submitGroundChunk(pos, textureRect, getNormalizedZ(z), color);
}

void submitLine(sf::Color color, const sf::Vector2f positionA, const sf::Vector2f positionB, float thickness)
{
	submitLine(color, color, positionA, positionB, thickness);
}

void submitLine(sf::Color colorA, sf::Color colorB,
                          const sf::Vector2f positionA, const sf::Vector2f positionB, float thickness)
{
	lineRenderer.drawLine(colorA, colorB, positionA, positionB, thickness);
}

void submitPoint(sf::Vector2f position, sf::Color color, unsigned char z, float size)
{
	pointRenderer.submitPoint(position, color, getNormalizedZ(z), size);
}

void submitLight(sf::Color color, sf::Vector2f position, float startAngle, float endAngle,
                 float attenuationAddition, float attenuationFactor, float attenuationSquareFactor) 
{
	lightRenderer.submitLight({color, position, startAngle, endAngle, attenuationAddition, attenuationFactor, attenuationSquareFactor});
}

void submitLightWall(FloatRect wall)
{
	lightRenderer.submitLightWall(wall);
}

void submitBunchOfLightWalls(const std::vector<FloatRect>& walls)
{
	lightRenderer.submitBunchOfLightWalls(walls);
}

unsigned getNrOfLights()
{
	return lightRenderer.getNrOfLights();
}

void submitText(const char* text, const char* fontFilename, sf::Vector2f position, float characterSize, sf::Color color,
                unsigned char z, ProjectionType projecitonType, bool isAffectedByLight)
{
	TextRenderer::drawText(text, fontFilename, position, characterSize, color, z, projecitonType, isAffectedByLight);
}

void submitTextArea(const char* text, const char* fontFilename, sf::Vector2f position, float textAreaWidth,
                    TextAligment aligment, float size, sf::Color color, unsigned char z, ProjectionType projectionType, bool isAffectedByLight)
{
	TextRenderer::drawTextArea(text, fontFilename, position, textAreaWidth, aligment, size, color, z, projectionType, isAffectedByLight);
}

void handleEvent(sf::Event e)
{
	if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F3) {
		isDebugDisplayActive = !isDebugDisplayActive;
	}
	if(e.type == sf::Event::Resized) {
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

