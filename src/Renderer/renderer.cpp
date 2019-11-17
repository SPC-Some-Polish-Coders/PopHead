#include <GL/glew.h>
#include "renderer.hpp"
#include "MinorRenderers/slowQuadRenderer.hpp"
#include "MinorRenderers/quadRenderer.hpp"
#include "MinorRenderers/SFMLrenderer.hpp"
#include "Shaders/shaderLibary.hpp"
#include "Buffers/vertexArray.hpp"
#include "camera.hpp"
#include "EfficiencyRegister/efficiencyRegister.hpp"
#include "Logs/logs.hpp"
#include "openglErrors.hpp"
#include "framebuffer.hpp"
#include "Utilities/vector4.hpp"
#include "Utilities/cast.hpp"
#include <SFML/Graphics/Transform.hpp>
#include <vector>
#include <algorithm>

namespace {
	ph::FloatRect screenBounds;

	ph::Shader* defaultFramebufferShader;
	const ph::Shader* currentlyBoundShader = nullptr;
	
	ph::VertexArray framebufferVertexArray;
	ph::Framebuffer* framebuffer;

	ph::SlowQuadRenderer slowQuadRenderer;
	ph::QuadRenderer quadRenderer;
	ph::SFMLRenderer sfmlRenderer;
}

namespace ph {

void Renderer::init(unsigned screenWidth, unsigned screenHeight)
{
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		PH_EXIT_GAME("GLEW wasn't initialized correctly!");

	slowQuadRenderer.init();
	slowQuadRenderer.setScreenBoundsPtr(&screenBounds);

	quadRenderer.init();
	quadRenderer.setScreenBoundsPtr(&screenBounds);

	// set up blending
	GLCheck( glEnable(GL_BLEND) );
	GLCheck( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

	// set up framebuffer
	auto& sl = ShaderLibrary::getInstance();
	sl.loadFromFile("defaultFramebuffer", "resources/shaders/defaultFramebuffer.vs.glsl", "resources/shaders/defaultFramebuffer.fs.glsl");
	defaultFramebufferShader = sl.get("defaultFramebuffer");

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
	framebufferVBO.setData(framebufferQuad, sizeof(framebufferQuad), DataUsage::staticDraw);
	framebufferVertexArray.init();
	framebufferVertexArray.setVertexBuffer(framebufferVBO, VertexBufferLayout::position2_texCoords2);
	framebufferVertexArray.setIndexBuffer(quadIBO);

	// set up framebuffer
	framebuffer = new Framebuffer(screenWidth, screenHeight);
}

void Renderer::restart(unsigned screenWidth, unsigned screenHeight)
{
	shutDown();
	init(screenWidth, screenHeight);
}

void Renderer::shutDown()
{
	slowQuadRenderer.shutDown();
	quadRenderer.shutDown();
	framebufferVertexArray.remove();
	delete framebuffer;
}

void Renderer::beginScene(Camera& camera)
{
	framebuffer->bind();

	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );

	const float* viewProjectionMatrix = camera.getViewProjectionMatrix4x4().getMatrix();
	slowQuadRenderer.setViewProjectionMatrix(viewProjectionMatrix);
	quadRenderer.setViewProjectionMatrix(viewProjectionMatrix);
	
	const sf::Vector2f center = camera.getCenter();
	const sf::Vector2f size = camera.getSize();
	screenBounds = FloatRect(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);
}

void Renderer::endScene(sf::RenderWindow& window, EfficiencyRegister& efficiencyRegister)
{
	quadRenderer.flush();

	efficiencyRegister.setAllDrawCallsPerFrame(sfmlRenderer.getNumberOfSubmitedObjects() + quadRenderer.getNumberOfDrawCalls());
	efficiencyRegister.setNumberOfInstancedDrawCalls(quadRenderer.getNumberOfDrawCalls());
	efficiencyRegister.setNumberOfDrawnInstancedSprites(quadRenderer.getNumberOfDrawnSprites());
	efficiencyRegister.setNumberOfTexturesDrawnByInstancedRendering(quadRenderer.getNumberOfDrawnTextures());
	efficiencyRegister.setNumberOfSFMLDrawCalls(sfmlRenderer.getNumberOfSubmitedObjects());
	quadRenderer.setDebugNumbersToZero();

	sfmlRenderer.flush(window);

	Framebuffer::bindDefaultFramebuffer();
	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );
	framebufferVertexArray.bind();
	defaultFramebufferShader->bind();
	currentlyBoundShader = defaultFramebufferShader;
	framebuffer->bindTextureColorBuffer(0);
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );
}

void Renderer::slowSubmitQuad(const Texture* texture, const IntRect* textureRect, const sf::Color* color, const Shader* shader,
                              sf::Vector2f position, sf::Vector2i size, float rotation)
{
	slowQuadRenderer.drawQuad(texture, textureRect, color, shader, position, size, rotation);
}

void Renderer::submitQuad(const Texture* texture, const IntRect* textureRect, const sf::Color* color, 
                          sf::Vector2f position, sf::Vector2f size, float rotation)
{
	quadRenderer.submitQuad(texture, textureRect, color, position, size, rotation);
}

void Renderer::submitSFMLObject(const sf::Drawable& object)
{
	sfmlRenderer.submit(&object);
}

void Renderer::onWindowResize(unsigned width, unsigned height)
{
	GLCheck( glViewport(0, 0, width, height) );
	framebuffer->reset(width, height);
}

void Renderer::setClearColor(const sf::Color& color)
{
	GLCheck( glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f) );
}

}
