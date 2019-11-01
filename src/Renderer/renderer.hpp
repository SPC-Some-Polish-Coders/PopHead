#pragma once

#include "Vertices/vertexArray.hpp"
#include "Shaders/shaderLibary.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "sprite.hpp"
#include "glEnums.hpp"
#include "SFMLrenderer.hpp"
#include "Utilities/rect.hpp"
#include <SFML/System/Vector2.hpp>
#include <optional>

namespace ph {

class EfficiencyRegister;

class Renderer
{
public:
	static void init();
	
	static void beginScene(Camera&);
	static void endScene(sf::RenderWindow& window, EfficiencyRegister&);

	static void submitQuad(const sf::Color&, sf::Vector2f position, sf::Vector2i size, float rotation = 0.f);
	static void submitQuad(const Texture&, sf::Vector2f position, sf::Vector2i size, float rotation = 0.f);
	static void submitQuad(const Texture&, const Shader*, sf::Vector2f position, sf::Vector2i size, float rotation = 0.f);
	static void submitQuad(const Texture&, const IntRect& textureRect, sf::Vector2f position, sf::Vector2i size, float rotation = 0.f);
	static void submitQuad(const Texture&, const IntRect& textureRect, const Shader*, sf::Vector2f position, sf::Vector2i size, float rotation = 0.f);
	static void submitQuad(const Texture&, const sf::Color&, sf::Vector2f position, sf::Vector2i size, float rotation = 0.f);
	static void submitQuad(const Texture&, const sf::Color&, const Shader*, sf::Vector2f position, sf::Vector2i size, float rotation = 0.f);
	static void submitQuad(const Texture&, const sf::Color&, const IntRect& textureRect, sf::Vector2f position, sf::Vector2i size, float rotation = 0.f);
	static void submitQuad(const Texture&, const sf::Color&, const IntRect& textureRect, const Shader*, sf::Vector2f position, sf::Vector2i size, float rotation = 0.f);

	static void submit(VertexArray& vao, Shader& shader, const sf::Transform&, const sf::Vector2i size, DrawPrimitive = DrawPrimitive::Triangles);
	static void submit(VertexArray& vao, Shader& shader, const FloatRect bounds, DrawPrimitive = DrawPrimitive::Triangles);
	static void submit(VertexArray& vao, const FloatRect bounds, DrawPrimitive = DrawPrimitive::Triangles);
	static void submit(VertexArray& vao, const sf::Transform&, const sf::Vector2i size, DrawPrimitive = DrawPrimitive::Triangles);
	static void submit(Sprite&, Shader&, const sf::Transform&, DrawPrimitive = DrawPrimitive::Triangles);
	static void submit(Sprite&, const sf::Transform&, DrawPrimitive = DrawPrimitive::Triangles);
	static void submit(const sf::Drawable&);

	static void onWindowResize(unsigned width, unsigned height);

	static void setClearColor(const sf::Color&);

private:
	static void setQuadTransformUniforms(const Shader* shader, sf::Vector2f position, const sf::Vector2i size, float rotation);
	static bool isInsideScreen(const sf::Transform&, const sf::Vector2i size);
	static bool isInsideScreen(sf::Vector2f position, sf::Vector2i size);
	static bool isInsideScreen(const FloatRect objectBounds);
};

}
