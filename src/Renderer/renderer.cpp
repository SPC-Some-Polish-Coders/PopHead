#include "renderer.hpp"
#include "MinorRenderers/quadRenderer.hpp"
#include "MinorRenderers/lineRenderer.hpp"
#include "MinorRenderers/pointRenderer.hpp"
#include "MinorRenderers/lightRenderer.hpp"
#include "MinorRenderers/textRenderer.hpp"
#include "API/shader.hpp"
#include "API/vertexArray.hpp"
#include "API/camera.hpp"
#include "API/font.hpp"
#include "API/openglErrors.hpp"
#include "API/framebuffer.hpp"
#include "Shaders/embeddedShaders.hpp"
#include "Logs/logs.hpp"
#include "Utilities/vector4.hpp"
#include "Utilities/cast.hpp"
#include "Utilities/profiling.hpp"
#include <SFML/Graphics/Transform.hpp>
#include <GL/glew.h>
#include <vector>
#include <algorithm>
#include <cstdio>

namespace {
	ph::FloatRect screenBounds;

	ph::Shader defaultFramebufferShader;
	ph::Shader gaussianBlurFramebufferShader;
	
	ph::VertexArray framebufferVertexArray;
	ph::Framebuffer gameObjectsFramebuffer;
	ph::Framebuffer lightingFramebuffer;
	ph::Framebuffer lightingGaussianBlurFramebuffer;
	 
	sf::Color ambientLightColor;

	unsigned sharedDataUBO;

	ph::QuadRenderer quadRenderer;
	ph::PointRenderer pointRenderer;
	ph::LineRenderer lineRenderer;
	ph::LightRenderer lightRenderer;
	ph::TextRenderer textRenderer;

	ph::Camera gameWorldCamera; 

	bool isDebugDisplayActive = false;
}

namespace ph::Renderer {

static void setClearColor(sf::Color);
static float getNormalizedZ(const unsigned char z);

void init(unsigned screenWidth, unsigned screenHeight)
{
	// initialize glew
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		PH_EXIT_GAME("GLEW wasn't initialized correctly!");

	// initialize minor renderers
	quadRenderer.init();
	lineRenderer.init();
	pointRenderer.init();
	lightRenderer.init();
	textRenderer.init();
	quadRenderer.setScreenBoundsPtr(&screenBounds);
	pointRenderer.setScreenBoundsPtr(&screenBounds);
	lineRenderer.setScreenBoundsPtr(&screenBounds);
	lightRenderer.setScreenBoundsPtr(&screenBounds);

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
	
	// set up framebuffer
	defaultFramebufferShader.init(shader::defaultFramebufferSrc());
	gaussianBlurFramebufferShader.init(shader::gaussianBlurFramebufferSrc());

	float framebufferQuad[] = {
		1.f,-1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f,
	   -1.f, 1.f, 0.f, 1.f,
	   -1.f,-1.f, 0.f, 0.f
	};

	unsigned quadIndices[] = { 0, 1, 3, 1, 2, 3 };
	IndexBuffer quadIBO;
	quadIBO.init();
	quadIBO.setData(quadIndices, sizeof(quadIndices));

	VertexBuffer framebufferVBO;
	framebufferVBO.init();
	framebufferVBO.setData(framebufferQuad, sizeof(framebufferQuad), GL_STATIC_DRAW);
	framebufferVertexArray.init();
	framebufferVertexArray.setVertexBuffer(framebufferVBO, VertexBufferLayout::position2_texCoords2);
	framebufferVertexArray.setIndexBuffer(quadIBO);

	gameObjectsFramebuffer.init(screenWidth, screenHeight);
	lightingFramebuffer.init(screenWidth, screenHeight);
	lightingGaussianBlurFramebuffer.init(screenWidth, screenHeight);
}

void restart(unsigned screenWidth, unsigned screenHeight)
{
	shutDown();
	init(screenWidth, screenHeight);
}

void shutDown()
{
	quadRenderer.shutDown();
	lineRenderer.shutDown();
	lightRenderer.shutDown();
	textRenderer.shutDown();
	framebufferVertexArray.remove();
	gameObjectsFramebuffer.remove();
	lightingFramebuffer.remove();
	lightingGaussianBlurFramebuffer.remove();
	defaultFramebufferShader.remove();
	gaussianBlurFramebufferShader.remove();
}

void setGameWorldCamera(Camera& camera)
{
	gameWorldCamera = camera;	
}

void beginScene()
{
	PH_PROFILE_FUNCTION(0);

	gameObjectsFramebuffer.bind();
	GLCheck( glEnable(GL_DEPTH_TEST) );
	GLCheck( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );

	const float* viewProjectionMatrix = gameWorldCamera.getViewProjectionMatrix4x4().getMatrix();
	GLCheck( glBindBuffer(GL_UNIFORM_BUFFER, sharedDataUBO) );
	GLCheck( glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), viewProjectionMatrix) );
	
	sf::Vector2f center = gameWorldCamera.getCenter();
	sf::Vector2f size = gameWorldCamera.getSize();
	screenBounds = FloatRect(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);

	textRenderer.beginDebugDisplay();
}

void endScene()
{
	PH_PROFILE_FUNCTION(0);

	// render scene
	quadRenderer.flush(true);
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
	framebufferVertexArray.bind();

	// apply gaussian blur for lighting
	lightingGaussianBlurFramebuffer.bind();
	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );
	lightingFramebuffer.bindTextureColorBuffer(0);
	gaussianBlurFramebufferShader.bind();
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );

	// render everything onto quad in default framebuffer
	GLCheck( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );
	defaultFramebufferShader.bind();
	defaultFramebufferShader.setUniformInt("gameObjectsTexture", 0);
	gameObjectsFramebuffer.bindTextureColorBuffer(0);
	defaultFramebufferShader.setUniformInt("lightingTexture", 1);
	lightingGaussianBlurFramebuffer.bindTextureColorBuffer(1);
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );
	quadRenderer.flush(false);

	// display renderer debug info 
	if(isDebugDisplayActive)
	{
		char debugText[50];
		auto submitDebugCounter = [&debugText](const char* text, unsigned number) {
			sprintf_s(debugText, "%s%u", text, number);
			submitDebugText(debugText, "LiberationMono.ttf", 20.f, 0.f, 0.f, sf::Color::White);
		};

		auto quadRendererNumbers = quadRenderer.getDebugNumbers();

		submitDebugCounter("All draw calls per frame: ",
			quadRendererNumbers.drawCalls + lineRenderer.getNumberOfDrawCalls() + pointRenderer.getNrOfDrawCalls());

		submitDebugCounter("Nr of instanced draw calls: ", quadRendererNumbers.drawCalls);
		submitDebugCounter("Nr of render groups: ", quadRendererNumbers.renderGroups);
		submitDebugCounter("Nr of drawn instanced sprites: ", quadRendererNumbers.drawnSprites);
		submitDebugCounter("Nr of instanced textures: ", quadRendererNumbers.drawnTextures);
		submitDebugCounter("Nr of line draw calls: ", lineRenderer.getNumberOfDrawCalls());
		submitDebugCounter("Nr of point draw calls: ", pointRenderer.getNrOfDrawCalls());
		submitDebugCounter("Nr of drawn points: ", pointRenderer.getNrOfDrawnPoints());
		submitDebugCounter("Nr of light draw calls: ", lightRenderer.getNrOfDrawCalls());
		submitDebugCounter("Nr of light rays: ", lightRenderer.getNrOfRays());
		
		quadRenderer.resetDebugNumbers();
		lineRenderer.resetDebugNumbers();
		pointRenderer.resetDebugNumbers();
		lightRenderer.resetDebugNumbers();
	}
}

void submitQuad(const Texture* texture, const IntRect* textureRect, const sf::Color* color, const Shader* shader,
                          sf::Vector2f position, sf::Vector2f size, unsigned char z, float rotation, sf::Vector2f rotationOrigin,
                          ProjectionType projectionType, bool isAffectedByLight)
{
	quadRenderer.submitQuad(texture, textureRect, color, shader, position, size,
		getNormalizedZ(z), rotation, rotationOrigin, projectionType, isAffectedByLight);
}

void submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>& qd, const Texture* t, const Shader* s,
                                                    unsigned char z, ProjectionType projectionType)
{
	quadRenderer.submitBunchOfQuadsWithTheSameTexture(qd, t, s, getNormalizedZ(z), projectionType);
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

void submitText(const char* text, const char* fontFilename, sf::Vector2f position, float characterSize, sf::Color color,
                unsigned char z, ProjectionType projecitonType, bool isAffectedByLight)
{
	textRenderer.drawText(text, fontFilename, position, characterSize, color, z, projecitonType, isAffectedByLight);
}

void submitDebugText(const char* text, const char* fontFilename, float characterSize, float upMargin, float downMargin,
                     sf::Color textColor)
{
	textRenderer.drawDebugText(text, fontFilename, characterSize, upMargin, downMargin, textColor);
}

void submitTextArea(const char* text, const char* fontFilename, sf::Vector2f position, float textAreaWidth,
                    TextAligment aligment, float size, sf::Color color, unsigned char z, ProjectionType projectionType, bool isAffectedByLight)
{
	textRenderer.drawTextArea(text, fontFilename, position, textAreaWidth, aligment, size, color, z, projectionType, isAffectedByLight);
}

void submitLightBlockingQuad(sf::Vector2f position, sf::Vector2f size)
{
	lightRenderer.submitLightBlockingQuad(position, size);
}

void handleEvent(Event& phEvent)
{
	if(auto* e = std::get_if<sf::Event>(&phEvent))
	{
		if(e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::F3) {
			isDebugDisplayActive = !isDebugDisplayActive;
			lineRenderer.setDebugCountingActive(isDebugDisplayActive);
			pointRenderer.setDebugCountingActive(isDebugDisplayActive);
			quadRenderer.
			setDebugCountingActive(isDebugDisplayActive);
		}
		if(e->type == sf::Event::Resized) {
			GLCheck( glViewport(0, 0, e->size.width, e->size.height) );
			gameObjectsFramebuffer.onWindowResize(e->size.width, e->size.height);
			lightingFramebuffer.onWindowResize(e->size.width, e->size.height);
		}
	}
}

void setAmbientLightColor(sf::Color color)
{
	ambientLightColor = color;
}

void setClearColor(sf::Color color)
{
	GLCheck( glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f) );
}

float getNormalizedZ(const unsigned char z)
{
	return z / 255.f;
}

}
