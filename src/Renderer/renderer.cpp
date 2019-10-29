#include <GL/glew.h>
#include "renderer.hpp"
#include "GameObjects/gameObject.hpp"
#include "EfficiencyRegister/efficiencyRegister.hpp"
#include "Logs/logs.hpp"
#include "openglErrors.hpp"
#include <SFML/Graphics/Transform.hpp>
#include <memory>

namespace {
	// SceneData
	ph::FloatRect screenBounds;
	const float* viewProjectionMatrix = nullptr;

	// RendererData
	unsigned numberOfDrawCalls = 0;
	ph::Shader* defaultShader;
	const ph::Shader* currentlyBoundShader = nullptr;
	std::unique_ptr<ph::VertexArray> quadVertexArray;
	std::unique_ptr<ph::VertexArray> animatedQuadVertexArray;
	bool isCustomTextureRectApplied = false;

	// TODO_ren: Get rid of SFML Renderer
	ph::SFMLRenderer sfmlRenderer;
}

namespace ph {

void Renderer::init()
{
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		PH_EXIT_GAME("GLEW wasn't initialized correctly!");

	// set up blending
	GLCheck( glEnable(GL_BLEND) );
	GLCheck( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

	// load default shaders
	auto& sl = ShaderLibrary::getInstance();
	sl.loadFromFile("perfectPixel", "resources/shaders/staticPixelPerfect.vs.glsl", "resources/shaders/texture.fs.glsl");
	sl.loadFromFile("dynamic", "resources/shaders/default.vs.glsl", "resources/shaders/texture.fs.glsl");
	defaultShader = sl.get("dynamic");

	// load quad vertex arrays
	float quadVertices[] = {
		1.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 1.f 
	};

	unsigned quadIndices[] = { 0, 1, 3, 1, 2, 3 };
	IndexBuffer quadIBO = createIndexBuffer();
	setData(quadIBO, quadIndices, sizeof(quadIndices));

	VertexBuffer quadVBO = createVertexBuffer();
	setData(quadVBO, quadVertices, sizeof(quadVertices), DataUsage::staticDraw);
	quadVertexArray = std::make_unique<VertexArray>();
	quadVertexArray->setVertexBuffer(quadVBO, VertexBufferLayout::position2_texCoords2);
	quadVertexArray->setIndexBuffer(quadIBO);

	VertexBuffer animatedQuadVBO = createVertexBuffer();
	setData(animatedQuadVBO, nullptr, sizeof(quadVertices), DataUsage::dynamicDraw);
	animatedQuadVertexArray = std::make_unique<VertexArray>();
	animatedQuadVertexArray->setVertexBuffer(animatedQuadVBO, VertexBufferLayout::position2_texCoords2);
	animatedQuadVertexArray->setIndexBuffer(quadIBO);
}

void Renderer::beginScene(Camera& camera)
{
	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );

	viewProjectionMatrix = camera.getViewProjectionMatrix4x4().getMatrix();
	
	const sf::Vector2f center = camera.getCenter();
	const sf::Vector2f size = camera.getSize();
	screenBounds = FloatRect(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);
}

void Renderer::endScene(sf::RenderWindow& window, EfficiencyRegister& efficiencyRegister)
{
	sfmlRenderer.drawSubmitedObjects(window);

	efficiencyRegister.setDrawCallsPerFrame(numberOfDrawCalls);
	numberOfDrawCalls = 0;
}

void Renderer::submitQuad(const Texture& texture, sf::Vector2f position, sf::Vector2i size, float rotation)
{
	submitQuad(texture, defaultShader, position, size, rotation);
}

void Renderer::submitQuad(const Texture& texture, const Shader* shader, sf::Vector2f position, sf::Vector2i size, float rotation)
{
	if(!isInsideScreen(position, size))
		return;

	if(shader != currentlyBoundShader) {
		shader->bind();
		currentlyBoundShader = shader;
	}

	quadVertexArray->bind();
	texture.bind();
	
	sf::Transform transform;
	transform.translate(position);
	transform.scale(static_cast<sf::Vector2f>(size));
	if(rotation != 0.f)
		transform.rotate(rotation);
	shader->setUniformMatrix4x4("modelMatrix", transform.getMatrix());

	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );

	++numberOfDrawCalls;
}

void Renderer::submitQuad(const Texture& texture, const IntRect& textureRect, sf::Vector2f position, sf::Vector2i size, float rotation)
{
	submitQuad(texture, textureRect, defaultShader, position, size, rotation);
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

	setTextureRect(animatedQuadVertexArray->getVertexBuffer(), textureRect, texture.getSize());

	animatedQuadVertexArray->bind();
	texture.bind();

	sf::Transform transform;
	transform.translate(position);
	transform.scale(static_cast<sf::Vector2f>(size));
	if(rotation != 0.f)
		transform.rotate(rotation);
	shader->setUniformMatrix4x4("modelMatrix", transform.getMatrix());

	GLCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

	++numberOfDrawCalls;
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
	submit(vao, *defaultShader, bounds, drawMode);
}

void Renderer::submit(VertexArray& vao, const sf::Transform& transform, const sf::Vector2i size, DrawPrimitive drawMode)
{
	submit(vao, *defaultShader, transform, size, drawMode);
}

void Renderer::submit(Sprite& sprite, Shader& shader, const sf::Transform& transform, DrawPrimitive drawMode)
{
	sprite.mTexture.bind();
	submit(sprite.mVertexArray, shader, transform, sprite.mSize, drawMode);
}

void Renderer::submit(Sprite& sprite, const sf::Transform& transform, DrawPrimitive drawMode)
{
	submit(sprite, *defaultShader, transform, drawMode);
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
}

void Renderer::setClearColor(const sf::Color& color)
{
	GLCheck( glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f) );
}

}
