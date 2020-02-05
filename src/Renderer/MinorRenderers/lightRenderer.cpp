#include "lightRenderer.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/API/shader.hpp"
#include "Utilities/math.hpp"
#include "Utilities/profiling.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"
#include "Logs/logs.hpp"
#include <optional>
#include <cmath>
#include <algorithm>
#include <GL/glew.h>

namespace ph {

void LightRenderer::init()
{
	mLightShader.init(shader::lightSrc());
	mLightShader.initUniformBlock("SharedData", 0);

	mLocalIlluminationShader.init(shader::localIlluminationSrc());
	mLocalIlluminationShader.initUniformBlock("SharedData", 0);

	glGenVertexArrays(1, &mLightTriangleFanVAO);
	glBindVertexArray(mLightTriangleFanVAO);

	glGenBuffers(1, &mLightTriangleFanVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mLightTriangleFanVBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

	mLightTriangleFanVertexData.reserve(361);

	glGenVertexArrays(1, &mLocalIlluminationQuadVAO);
	glBindVertexArray(mLocalIlluminationQuadVAO);

	unsigned iboData[6] = {0, 1, 2, 2, 3, 0};
	glGenBuffers(1, &mLocalIlluminationQuadIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mLocalIlluminationQuadIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(iboData), iboData, GL_STATIC_DRAW);

	glGenBuffers(1, &mLocalIlluminationQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mLocalIlluminationQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
}

void LightRenderer::shutDown()
{
	glDeleteBuffers(1, &mLightTriangleFanVBO);
	glDeleteVertexArrays(1, &mLightTriangleFanVAO);
	glDeleteBuffers(1, &mLocalIlluminationQuadVBO);
	glDeleteBuffers(1, &mLocalIlluminationQuadIBO);
	glDeleteVertexArrays(1, &mLocalIlluminationQuadVAO);
	mLightShader.remove();
}

void LightRenderer::submitLightBlockingQuad(sf::Vector2f position, sf::Vector2f size)
{
	if(!mScreenBounds->doPositiveRectsIntersect(FloatRect(position.x - 200.f, position.y - 200.f, size.x + 400.f, size.y + 400.f)))
		return;
	
	sf::Vector2f upLeftPoint = position;
	sf::Vector2f upRightPoint = sf::Vector2f(position.x + size.x, position.y);
	sf::Vector2f downRightPoint = position + size;
	sf::Vector2f downLeftPoint = sf::Vector2f(position.x, position.y + size.y);

	mWalls.emplace_back(Wall{upLeftPoint, upRightPoint});
	mWalls.emplace_back(Wall{upRightPoint, downRightPoint});
	mWalls.emplace_back(Wall{downRightPoint, downLeftPoint});
	mWalls.emplace_back(Wall{downLeftPoint, upLeftPoint});
}

void LightRenderer::submitLight(Light light)
{
	// TODO: Culling
	mLights.emplace_back(light);
}

void LightRenderer::submitLocalIllumination(LocalIllumination li)
{
	mLocalIlluminations.emplace_back(li);	
}

void LightRenderer::flush()
{
	PH_PROFILE_FUNCTION(0);

	// submit quad which rays will hit if they won't hit anything in the scene
	submitLightBlockingQuad(
		{mScreenBounds->left -2000.f, mScreenBounds->top -2000.f},
		{mScreenBounds->width + 4000.f, mScreenBounds->height + 4000.f});

	for(auto& light : mLights)
	{
		// make light position be first vertex of triangle fan
		mLightTriangleFanVertexData.emplace_back(light.pos);

		// create vertex data
		{
			PH_PROFILE_SCOPE("create vertex data", 0);

			// TODO_ren: Optimize ray casting algorithm
			for(float angle = light.startAngle; angle <= light.endAngle; angle += 0.5)
			{
				float rad = Math::degreesToRadians(angle);
				sf::Vector2f rayDir(std::cos(rad), std::sin(rad));
				sf::Vector2f nearestIntersectionPoint;
				float nearestIntersectionDistance = INFINITY;
				for(Wall& wall : mWalls)
				{
					auto intersectionPoint = getIntersectionPoint(rayDir, light.pos, wall);
					float intersectionDistance = Math::distanceBetweenPoints(light.pos, *intersectionPoint);
					if(intersectionPoint && intersectionDistance < nearestIntersectionDistance)
					{
						nearestIntersectionPoint = *intersectionPoint;
						nearestIntersectionDistance = intersectionDistance;
					}
				}
				mLightTriangleFanVertexData.emplace_back(nearestIntersectionPoint);
			}
		}

		// draw light using triangle fan
		if(sDebug.drawLight)
		{
			PH_PROFILE_SCOPE("draw light triangle fan", 0);

			mLightShader.bind();
			mLightShader.setUniformVector2("lightPos", light.pos);
			mLightShader.setUniformVector4Color("color", light.color);
			mLightShader.setUniformFloat("cameraZoom", mScreenBounds->height / 480);
			mLightShader.setUniformFloat("a", light.attenuationAddition);
			mLightShader.setUniformFloat("b", light.attenuationFactor);
			mLightShader.setUniformFloat("c", light.attenuationSquareFactor);
			glBindVertexArray(mLightTriangleFanVAO);
			glBindBuffer(GL_ARRAY_BUFFER ,mLightTriangleFanVBO); // TODO: Do I have to bind it?
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mLightTriangleFanVertexData.size(), mLightTriangleFanVertexData.data(), GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLE_FAN, 0, mLightTriangleFanVertexData.size());
		}

		// draw local illumination
		glBindVertexArray(mLocalIlluminationQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mLocalIlluminationQuadVBO);
		mLocalIlluminationShader.bind();
		for(auto& li : mLocalIlluminations)
		{
			float vertexData[8] = {
				li.pos.x, li.pos.y, 
				li.pos.x + li.size.x, li.pos.y, 
				li.pos.x + li.size.x, li.pos.y + li.size.y,
				li.pos.x, li.pos.y + li.size.y
			};
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData), vertexData);
			mLocalIlluminationShader.setUniformVector4Color("color", li.color);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		mLocalIlluminations.clear();

		// draw debug 
		if(sDebug.drawWalls)
			for(Wall& wall : mWalls)
				Renderer::submitLine(sf::Color::Red, wall.point1, wall.point2, 5);

		if(sDebug.drawRays)
		{
			for(auto& point : mLightTriangleFanVertexData) {
				Renderer::submitPoint(point, light.color, 0, 7.f);
				Renderer::submitLine(light.color, light.pos, point, 3.f);
			}
			for(const auto& light : mLights)
				Renderer::submitPoint(light.pos, light.color, 0, 15.f);
		}

		mLightTriangleFanVertexData.clear();
	}

	mWalls.clear();
	mLights.clear();
}

auto LightRenderer::getIntersectionPoint(const sf::Vector2f rayDir, sf::Vector2f lightPos, const Wall& wall) -> std::optional<sf::Vector2f>
{
	const float x1 = wall.point1.x;
	const float y1 = wall.point1.y;
	const float x2 = wall.point2.x;
	const float y2 = wall.point2.y;

	const float x3 = lightPos.x;
	const float y3 = lightPos.y;
	const float x4 = lightPos.x + rayDir.x;
	const float y4 = lightPos.y + rayDir.y;

	const float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if(den == 0.f)
		return std::nullopt;

	const float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
	const float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
	
	if(t > 0 && t < 1 && u > 0)
		return sf::Vector2f(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
	else
		return std::nullopt;
}

}

