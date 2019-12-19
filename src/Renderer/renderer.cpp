#include <GL/glew.h>
#include "renderer.hpp"
#include "MinorRenderers/quadRenderer.hpp"
#include "MinorRenderers/lineRenderer.hpp"
#include "MinorRenderers/SFMLrenderer.hpp"
#include "MinorRenderers/pointRenderer.hpp"
#include "MinorRenderers/lightRenderer.hpp"
#include "API/shader.hpp"
#include "API/vertexArray.hpp"
#include "API/camera.hpp"
#include "DebugCounter/debugCounter.hpp"
#include "Logs/logs.hpp"
#include "API/openglErrors.hpp"
#include "API/framebuffer.hpp"
#include "Utilities/vector4.hpp"
#include "Utilities/cast.hpp"
#include "Utilities/profiling.hpp"
#include <SFML/Graphics/Transform.hpp>
#include <vector>
#include <algorithm>

namespace {
	ph::FloatRect screenBounds;

	ph::Shader* defaultFramebufferShader;
	ph::Shader* gaussianBlurFramebufferShader;
	
	ph::VertexArray framebufferVertexArray;
	ph::Framebuffer gameObjectsFramebuffer;
	ph::Framebuffer lightingFramebuffer;
	ph::Framebuffer lightingGaussianBlurFramebuffer;
	 
	sf::Color ambientLightColor;

	unsigned sharedDataUBO;

	ph::QuadRenderer quadRenderer;
	ph::PointRenderer pointRenderer;
	ph::LineRenderer lineRenderer;
	ph::SFMLRenderer sfmlRenderer;
	ph::LightRenderer lightRenderer;
}

namespace ph {

static void setClearColor(sf::Color);
static float getNormalizedZ(const unsigned char z);

void Renderer::init(unsigned screenWidth, unsigned screenHeight)
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
	glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(float), nullptr, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, sharedDataUBO, 0, 16 * sizeof(float));

	// set up framebuffer
	auto& sl = ShaderLibrary::getInstance();
	sl.loadFromFile("defaultFramebuffer", "resources/shaders/defaultFramebuffer.vs.glsl", "resources/shaders/defaultFramebuffer.fs.glsl");
	defaultFramebufferShader = sl.get("defaultFramebuffer");
	sl.loadFromFile("gaussianBlurFramebuffer", "resources/shaders/defaultFramebuffer.vs.glsl", "resources/shaders/gaussianBlur.fs.glsl");
	gaussianBlurFramebufferShader = sl.get("gaussianBlurFramebuffer");

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

void Renderer::restart(unsigned screenWidth, unsigned screenHeight)
{
	shutDown();
	init(screenWidth, screenHeight);
}

void Renderer::shutDown()
{
	quadRenderer.shutDown();
	lineRenderer.shutDown();
	lightRenderer.shutDown();
	framebufferVertexArray.remove();
	gameObjectsFramebuffer.remove();
	lightingFramebuffer.remove();
	lightingGaussianBlurFramebuffer.remove();
}

void Renderer::beginScene(Camera& camera)
{
	PH_PROFILE_FUNCTION();

	gameObjectsFramebuffer.bind();
	GLCheck( glEnable(GL_DEPTH_TEST) );
	GLCheck( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );

	const float* viewProjectionMatrix = camera.getViewProjectionMatrix4x4().getMatrix();
	GLCheck( glBindBuffer(GL_UNIFORM_BUFFER, sharedDataUBO) );
	GLCheck( glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), viewProjectionMatrix) );
	
	const sf::Vector2f center = camera.getCenter();
	const sf::Vector2f size = camera.getSize();
	screenBounds = FloatRect(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);
}

void Renderer::endScene(sf::RenderWindow& window, DebugCounter& debugCounter)
{
	PH_PROFILE_FUNCTION();

	// render scene
	quadRenderer.flush();
	pointRenderer.flush();

	// disable depth test for performance purposes
	GLCheck( glDisable(GL_DEPTH_TEST) );

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
	gaussianBlurFramebufferShader->bind();
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );

	// render everything onto quad in default framebuffer
	GLCheck( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );
	defaultFramebufferShader->bind();
	defaultFramebufferShader->setUniformInt("gameObjectsTexture", 0);
	gameObjectsFramebuffer.bindTextureColorBuffer(0);
	defaultFramebufferShader->setUniformInt("lightingTexture", 1);
	lightingGaussianBlurFramebuffer.bindTextureColorBuffer(1);
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );

	// pass debug data to debug counter
	debugCounter.setAllDrawCallsPerFrame(
		sfmlRenderer.getNumberOfSubmitedObjects() + quadRenderer.getNumberOfDrawCalls() +
		lineRenderer.getNumberOfDrawCalls() + pointRenderer.getNrOfDrawCalls()
	);
	debugCounter.setNumberOfInstancedDrawCalls(quadRenderer.getNumberOfDrawCalls());
	debugCounter.setNumberOfRenderGroups(quadRenderer.getNumberOfRenderGroups());
	debugCounter.setNumberOfDrawnInstancedSprites(quadRenderer.getNumberOfDrawnSprites());
	debugCounter.setNumberOfTexturesDrawnByInstancedRendering(quadRenderer.getNumberOfDrawnTextures());
	debugCounter.setNumberOfSFMLDrawCalls(sfmlRenderer.getNumberOfSubmitedObjects());
	debugCounter.setNumberOfLineDrawCalls(lineRenderer.getNumberOfDrawCalls());
	debugCounter.setNumberOfDrawnLines(lineRenderer.getNumberOfDrawnLines());
	debugCounter.setNumberOfPointDrawCalls(pointRenderer.getNrOfDrawCalls());
	debugCounter.setNumberOfDrawnPoints(pointRenderer.getNrOfDrawnPoints());
	quadRenderer.setDebugNumbersToZero();
	lineRenderer.setDebugNumbersToZero();
	pointRenderer.setDebugNumbersToZero();

	// draw gui using sfml
	sfmlRenderer.flush(window);
}

void Renderer::submitQuad(const Texture* texture, const IntRect* textureRect, const sf::Color* color, const Shader* shader,
                          sf::Vector2f position, sf::Vector2f size, unsigned char z, float rotation, sf::Vector2f rotationOrigin,
                          bool blocksLight)
{
	quadRenderer.submitQuad(texture, textureRect, color, shader, position, size, getNormalizedZ(z), rotation, rotationOrigin);
	if(blocksLight)
		lightRenderer.submitLightBlockingQuad(position, size);
}

void Renderer::submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>& qd, const Texture* t, const Shader* s, unsigned char z)
{
	quadRenderer.submitBunchOfQuadsWithTheSameTexture(qd, t, s, getNormalizedZ(z));
}

void Renderer::submitLine(sf::Color color, const sf::Vector2f positionA, const sf::Vector2f positionB, float thickness)
{
	submitLine(color, color, positionA, positionB, thickness);
}

void Renderer::submitLine(sf::Color colorA, sf::Color colorB,
                          const sf::Vector2f positionA, const sf::Vector2f positionB, float thickness)
{
	lineRenderer.drawLine(colorA, colorB, positionA, positionB, thickness);
}

void Renderer::submitPoint(sf::Vector2f position, sf::Color color, unsigned char z, float size)
{
	pointRenderer.submitPoint(position, color, getNormalizedZ(z), size);
}

void Renderer::submitLight(sf::Color color, sf::Vector2f position, float startAngle, float endAngle,
                           float attenuationAddition, float attenuationFactor, float attenuationSquareFactor) 
{
	lightRenderer.submitLight({color, position, startAngle, endAngle, attenuationAddition, attenuationFactor, attenuationSquareFactor});
}

void Renderer::submitSFMLObject(const sf::Drawable& object)
{
	sfmlRenderer.submit(&object);
}

void Renderer::onWindowResize(unsigned width, unsigned height)
{
	GLCheck( glViewport(0, 0, width, height) );
	gameObjectsFramebuffer.onWindowResize(width, height);
	lightingFramebuffer.onWindowResize(width, height);
}

void Renderer::setAmbientLightColor(sf::Color color)
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
