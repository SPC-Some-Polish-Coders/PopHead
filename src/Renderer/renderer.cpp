#include <GL/glew.h>
#include "renderer.hpp"
#include "EfficiencyRegister/efficiencyRegister.hpp"
#include "Logs/logs.hpp"
#include "openglErrors.hpp"
#include "framebuffer.hpp"
#include <SFML/Graphics/Transform.hpp>

namespace {
	// SceneData
	ph::FloatRect screenBounds;
	const float* viewProjectionMatrix = nullptr;

	// RendererData
	unsigned numberOfDrawCalls = 0;
	ph::Shader* singleColorSpriteShader;
	ph::Shader* textureSpriteShader;
	ph::Shader* coloredTextureSpriteShader;
	ph::Shader* defaultFramebufferShader;
	const ph::Shader* currentlyBoundShader = nullptr;
	ph::VertexArray* singleColorQuadVertexArray;
	ph::VertexArray* textureQuadVertexArray;
	ph::VertexArray* textureAnimatedQuadVertexArray;
	ph::VertexArray* framebufferVertexArray;
	ph::Framebuffer* framebuffer;
	bool isCustomTextureRectApplied = false;

	// TODO_ren: Get rid of SFML Renderer
	ph::SFMLRenderer sfmlRenderer;
}

namespace ph {

void Renderer::init(unsigned screenWidth, unsigned screenHeight)
{
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		PH_EXIT_GAME("GLEW wasn't initialized correctly!");

	// set up blending
	GLCheck( glEnable(GL_BLEND) );
	GLCheck( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

	// load default shaders
	auto& sl = ShaderLibrary::getInstance();
	sl.loadFromFile("singleColorSprite", "resources/shaders/singleColorSprite.vs.glsl", "resources/shaders/singleColorSprite.fs.glsl");
	singleColorSpriteShader = sl.get("singleColorSprite");
	sl.loadFromFile("textureSprite", "resources/shaders/textureSprite.vs.glsl", "resources/shaders/textureSprite.fs.glsl");
	textureSpriteShader = sl.get("textureSprite");
	sl.loadFromFile("coloredTextureSprite", "resources/shaders/coloredTextureSprite.vs.glsl", "resources/shaders/coloredTextureSprite.fs.glsl");
	coloredTextureSpriteShader = sl.get("coloredTextureSprite");
	sl.loadFromFile("defaultFramebuffer", "resources/shaders/defaultFramebuffer.vs.glsl", "resources/shaders/defaultFramebuffer.fs.glsl");
	defaultFramebufferShader = sl.get("defaultFramebuffer");

	// load quad vertex arrays
	float quadPositions[] = {
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f,
		0.f, 0.f
	};

	float quadPositionsAndTextureCoords[] = {
		1.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 1.f 
	};

	float framebufferQuad[] = {
		1.f,-1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f,
	   -1.f, 1.f, 0.f, 1.f,
	   -1.f,-1.f, 0.f, 0.f
	};

	unsigned quadIndices[] = { 0, 1, 3, 1, 2, 3 };
	IndexBuffer quadIBO = createIndexBuffer();
	setData(quadIBO, quadIndices, sizeof(quadIndices));
	
	VertexBuffer singleColorQuadVBO = createVertexBuffer();
	setData(singleColorQuadVBO, quadPositions, sizeof(quadPositions), DataUsage::staticDraw);
	singleColorQuadVertexArray = new VertexArray;
	singleColorQuadVertexArray->setVertexBuffer(singleColorQuadVBO, VertexBufferLayout::position2);
	singleColorQuadVertexArray->setIndexBuffer(quadIBO);

	VertexBuffer textureQuadVBO = createVertexBuffer();
	setData(textureQuadVBO, quadPositionsAndTextureCoords, sizeof(quadPositionsAndTextureCoords), DataUsage::staticDraw);
	textureQuadVertexArray = new VertexArray;
	textureQuadVertexArray->setVertexBuffer(textureQuadVBO, VertexBufferLayout::position2_texCoords2);
	textureQuadVertexArray->setIndexBuffer(quadIBO);

	VertexBuffer animatedTextureQuadVBO = createVertexBuffer();
	setData(animatedTextureQuadVBO, nullptr, sizeof(quadPositionsAndTextureCoords), DataUsage::dynamicDraw);
	textureAnimatedQuadVertexArray = new VertexArray;
	textureAnimatedQuadVertexArray->setVertexBuffer(animatedTextureQuadVBO, VertexBufferLayout::position2_texCoords2);
	textureAnimatedQuadVertexArray->setIndexBuffer(quadIBO);

	VertexBuffer framebufferVBO = createVertexBuffer();
	setData(framebufferVBO, framebufferQuad, sizeof(framebufferQuad), DataUsage::staticDraw);
	framebufferVertexArray = new VertexArray;
	framebufferVertexArray->setVertexBuffer(framebufferVBO, VertexBufferLayout::position2_texCoords2);
	framebufferVertexArray->setIndexBuffer(quadIBO);

	// set up framebuffer
	framebuffer = new Framebuffer(screenWidth, screenHeight);
}

void Renderer::reset(unsigned screenWidth, unsigned screenHeight)
{
	shutDown();
	init(screenWidth, screenHeight);
}

void Renderer::shutDown()
{
	delete singleColorQuadVertexArray;
	delete textureQuadVertexArray;
	delete textureAnimatedQuadVertexArray;
	delete framebufferVertexArray;
	delete framebuffer;
}

void Renderer::beginScene(Camera& camera)
{
	framebuffer->bind();

	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );

	viewProjectionMatrix = camera.getViewProjectionMatrix4x4().getMatrix();
	
	const sf::Vector2f center = camera.getCenter();
	const sf::Vector2f size = camera.getSize();
	screenBounds = FloatRect(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);
}

void Renderer::endScene(sf::RenderWindow& window, EfficiencyRegister& efficiencyRegister)
{
	Framebuffer::bindDefaultFramebuffer();
	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );
	framebufferVertexArray->bind();
	defaultFramebufferShader->bind();
	currentlyBoundShader = defaultFramebufferShader;
	framebuffer->bindTextureColorBuffer();
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );

	sfmlRenderer.drawSubmitedObjects(window);

	efficiencyRegister.setDrawCallsPerFrame(numberOfDrawCalls);
	numberOfDrawCalls = 0;
}

void Renderer::submitQuad(const sf::Color& color, sf::Vector2f position, sf::Vector2i size, float rotation)
{
	if(!isInsideScreen(position, size))
		return;

	if(singleColorSpriteShader != currentlyBoundShader) {
		singleColorSpriteShader->bind();
		currentlyBoundShader = singleColorSpriteShader;
	}

	singleColorSpriteShader->setUniformVector4Color("color", color);
	setQuadTransformUniforms(singleColorSpriteShader, position, size, rotation);

	singleColorQuadVertexArray->bind();

	GLCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	++numberOfDrawCalls;
}

void Renderer::submitQuad(const Texture& texture, sf::Vector2f position, sf::Vector2i size, float rotation)
{
	submitQuad(texture, textureSpriteShader, position, size, rotation);
}

void Renderer::submitQuad(const Texture& texture, const Shader* shader, sf::Vector2f position, sf::Vector2i size, float rotation)
{
	if(!isInsideScreen(position, size))
		return;

	if(shader != currentlyBoundShader) {
		shader->bind();
		currentlyBoundShader = shader;
	}
	setQuadTransformUniforms(shader, position, size, rotation);

	textureQuadVertexArray->bind();
	
	texture.bind();

	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );
	++numberOfDrawCalls;
}

void Renderer::submitQuad(const Texture& texture, const IntRect& textureRect, sf::Vector2f position, sf::Vector2i size, float rotation)
{
	submitQuad(texture, textureRect, textureSpriteShader, position, size, rotation);
}

void Renderer::submitQuad(const Texture& texture, const IntRect& textureRect, const Shader* shader,
                          sf::Vector2f position, sf::Vector2i size, float rotation)
{
	if(!isInsideScreen(position, size))
		return;

	if(shader != currentlyBoundShader) {
		shader->bind();
		currentlyBoundShader = shader;
	}
	setQuadTransformUniforms(shader, position, size, rotation);

	setTextureRect(textureAnimatedQuadVertexArray->getVertexBuffer(), textureRect, texture.getSize());
	textureAnimatedQuadVertexArray->bind();

	texture.bind();

	GLCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	++numberOfDrawCalls;
}

void Renderer::submitQuad(const Texture& texture, const sf::Color& color, sf::Vector2f position, sf::Vector2i size, float rotation)
{
	submitQuad(texture, color, coloredTextureSpriteShader, position, size, rotation);
}

void Renderer::submitQuad(const Texture& texture, const sf::Color& color, const Shader* shader, sf::Vector2f position, sf::Vector2i size, float rotation)
{
	if(!isInsideScreen(position, size))
		return;

	if(shader != currentlyBoundShader) {
		shader->bind();
		currentlyBoundShader = shader;
	}
	shader->setUniformVector4Color("color", color);
	setQuadTransformUniforms(shader, position, size, rotation);

	textureQuadVertexArray->bind();
	
	texture.bind();

	GLCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	++numberOfDrawCalls;
}

void Renderer::submitQuad(const Texture& texture, const sf::Color& color, const IntRect& textureRect, sf::Vector2f position, sf::Vector2i size, float rotation)
{
	submitQuad(texture, color, textureRect, coloredTextureSpriteShader, position, size, rotation);
}

void Renderer::submitQuad(const Texture& texture, const sf::Color& color, const IntRect& textureRect, const Shader* shader,
                          sf::Vector2f position, sf::Vector2i size, float rotation)
{
	if(!isInsideScreen(position, size))
		return;
	
	if(shader != currentlyBoundShader) {
		shader->bind();
		currentlyBoundShader = shader;
	}
	shader->setUniformVector4Color("color", color);
	setQuadTransformUniforms(shader, position, size, rotation);

	setTextureRect(textureAnimatedQuadVertexArray->getVertexBuffer(), textureRect, texture.getSize());
	textureAnimatedQuadVertexArray->bind();

	texture.bind();

	GLCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	++numberOfDrawCalls;
}

void Renderer::setQuadTransformUniforms(const Shader* shader, sf::Vector2f position, const sf::Vector2i size, float rotation)
{
	sf::Transform transform;
	transform.translate(position);
	transform.scale(static_cast<sf::Vector2f>(size));
	if(rotation != 0.f)
		transform.rotate(rotation);
	shader->setUniformMatrix4x4("modelMatrix", transform.getMatrix());
	shader->setUniformMatrix4x4("viewProjectionMatrix", viewProjectionMatrix);
	// TODO_ren: Does viewProjectionMatrix have to be set for each object even if we don't change shader
}

void Renderer::submit(VertexArray& vao, Shader& shader, const sf::Transform& transform, const sf::Vector2i size, DrawPrimitive drawMode)
{
	if(!isInsideScreen(transform, size))
		return;

	vao.bind();

	// TODO_ren: Make that we don't bind shader if it was already bound
 	shader.bind();
	shader.setUniformMatrix4x4("modelMatrix", transform.getMatrix());
	shader.setUniformMatrix4x4("viewProjectionMatrix", viewProjectionMatrix);
	
	GLCheck( glDrawElements(toGLEnum(drawMode), vao.getIndexBuffer().mNumberOfIndices, GL_UNSIGNED_INT, 0) );
	
	++numberOfDrawCalls;
}

void Renderer::submit(VertexArray& vao, Shader& shader, const FloatRect bounds, DrawPrimitive drawMode)
{
	if(!isInsideScreen(bounds))
		return;

	vao.bind();

	// TODO_ren: Make that we don't bind shader if it was already bound
	shader.bind();
	shader.setUniformMatrix4x4("modelMatrix", sf::Transform::Identity.getMatrix());
	shader.setUniformMatrix4x4("viewProjectionMatrix", viewProjectionMatrix);

	GLCheck(glDrawElements(toGLEnum(drawMode), vao.getIndexBuffer().mNumberOfIndices, GL_UNSIGNED_INT, 0));

	++numberOfDrawCalls;
}

void Renderer::submit(VertexArray& vao, const FloatRect bounds, DrawPrimitive drawMode)
{
	submit(vao, *textureSpriteShader, bounds, drawMode);
}

void Renderer::submit(VertexArray& vao, const sf::Transform& transform, const sf::Vector2i size, DrawPrimitive drawMode)
{
	submit(vao, *textureSpriteShader, transform, size, drawMode);
}

void Renderer::submit(Sprite& sprite, Shader& shader, const sf::Transform& transform, DrawPrimitive drawMode)
{
	sprite.mTexture.bind();
	submit(sprite.mVertexArray, shader, transform, sprite.mSize, drawMode);
}

void Renderer::submit(Sprite& sprite, const sf::Transform& transform, DrawPrimitive drawMode)
{
	submit(sprite, *textureSpriteShader, transform, drawMode);
}

void Renderer::submit(const sf::Drawable& object)
{
	sfmlRenderer.submit(&object);
	++numberOfDrawCalls;
}

bool Renderer::isInsideScreen(const sf::Transform& transform, const sf::Vector2i size)
{
	const FloatRect objectRect(transform.getMatrix()[12], transform.getMatrix()[13], static_cast<float>(size.x), static_cast<float>(size.y));
	return screenBounds.doPositiveRectsIntersect(objectRect);
}

bool Renderer::isInsideScreen(sf::Vector2f position, sf::Vector2i size)
{
	return isInsideScreen(sf::FloatRect(position.x, position.y, size.x, size.y));
}

bool Renderer::isInsideScreen(const FloatRect objectBounds)
{
	return screenBounds.doPositiveRectsIntersect(objectBounds);
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
