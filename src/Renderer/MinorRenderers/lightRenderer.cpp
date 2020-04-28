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
#include <imgui.h>

namespace ph {

static unsigned rays; // debug info

static unsigned lights;
static bool lightingEnabled = true;
static bool drawRays = false;

unsigned LightRenderer::getNrOfLights()
{
	return lights;
}

void LightRenderer::init()
{
	mLightShader.init(shader::lightSrc());
	mLightShader.initUniformBlock("SharedData", 0);

	glGenVertexArrays(1, &mLightTriangleFanVAO);
	glBindVertexArray(mLightTriangleFanVAO);

	glGenBuffers(1, &mLightTriangleFanVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mLightTriangleFanVBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

	mLightTriangleFanVertexData.reserve(361);
}

void LightRenderer::shutDown()
{
	glDeleteBuffers(1, &mLightTriangleFanVBO);
	glDeleteVertexArrays(1, &mLightTriangleFanVAO);
}

void LightRenderer::submitBunchOfLightWalls(const std::vector<FloatRect>& walls)
{
	mLightWalls.insert(mLightWalls.end(), walls.begin(), walls.end());
}

void LightRenderer::submitLightWall(FloatRect wall)
{
	if(mScreenBounds->doPositiveRectsIntersect(FloatRect(wall.left - 200.f, wall.top - 200.f, wall.width + 400.f, wall.height + 400.f)))
		mLightWalls.emplace_back(wall);
}

void LightRenderer::submitLight(Light light)
{
	// TODO: Culling

	if(lightingEnabled)
	{
		mLights.emplace_back(light);
	}	
}

void LightRenderer::submitDebug()
{
	if(ImGui::BeginTabItem("light renderer"))
	{
		ImGui::Checkbox("lighting", &lightingEnabled);
		ImGui::Checkbox("rays debug", &drawRays);
		ImGui::Text("light sources (draw calls): %u", mLights.size());
		ImGui::Text("light rays: %u", rays);
		ImGui::EndTabItem();
	}
	rays = 0;

}

void LightRenderer::flush()
{
	PH_PROFILE_FUNCTION();

	if(mLights.empty())
		return;

	lights = mLights.size();

	// submit quad which rays will hit if they won't hit anything in the scene
	submitLightWall(FloatRect(mScreenBounds->left - 100000.f, mScreenBounds->top - 100000.f,
	                          mScreenBounds->width + 200000.f, mScreenBounds->height + 200000.f));

	FloatRect expandedScreenSize(mScreenBounds->left - 400.f, mScreenBounds->top - 400.f, 800.f, 800.f);

	for(auto& light : mLights)
	{
		// make light position be first vertex of triangle fan
		mLightTriangleFanVertexData.emplace_back(light.pos);

		// create vertex data
		{
			PH_PROFILE_SCOPE("create vertex data");

			// TODO_ren: Optimize ray casting algorithm
			for(float angle = light.startAngle; angle <= light.endAngle; angle += 0.5)
			{
				++rays;

				float rad = Math::degreesToRadians(angle);
				sf::Vector2f rayDir(std::cos(rad), std::sin(rad));
				sf::Vector2f nearestIntersectionPoint;
				float nearestIntersectionDistance = INFINITY;
				for(FloatRect& wall : mLightWalls)
				{
					auto wallIntersection = getRayWallClosestIntersection(rayDir, light.pos, wall);
					if(wallIntersection.valid && wallIntersection.distance < nearestIntersectionDistance)
					{
						nearestIntersectionPoint = wallIntersection.point;
						nearestIntersectionDistance = wallIntersection.distance;
					}
				}
				mLightTriangleFanVertexData.emplace_back(nearestIntersectionPoint);
			}
		}

		// draw light using triangle fan
		mLightShader.bind();
		PH_PROFILE_SCOPE("draw light triangle fan");
		mLightShader.setUniformVector2("lightPos", light.pos);
		mLightShader.setUniformVector4Color("color", light.color);
		mLightShader.setUniformFloat("cameraZoom", mScreenBounds->height / 480);
		mLightShader.setUniformFloat("a", light.attenuationAddition);
		mLightShader.setUniformFloat("b", light.attenuationFactor);
		mLightShader.setUniformFloat("c", light.attenuationSquareFactor);
		glBindVertexArray(mLightTriangleFanVAO);
		glBindBuffer(GL_ARRAY_BUFFER ,mLightTriangleFanVBO); // TODO: Do I have to bind it?
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mLightTriangleFanVertexData.size(), mLightTriangleFanVertexData.data(), GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<unsigned>(mLightTriangleFanVertexData.size()));
		mLightTriangleFanVertexData.clear();

		if(drawRays)
		{
			for(auto& point : mLightTriangleFanVertexData) {
				Renderer::submitPoint(point, light.color, 0, 7.f);
				Renderer::submitLine(light.color, light.pos, point, 3.f);
			}
		}
	}

	// draw light sources as points
	if(drawRays)
		for(auto& light : mLights)
			Renderer::submitPoint(light.pos, light.color, 0, 15.f);

	mLightWalls.clear();
	mLights.clear();
}

RayWallIntersection LightRenderer::getRayWallClosestIntersection(sf::Vector2f rayDir, sf::Vector2f lightPos, FloatRect wall)
{
	sf::Vector2f results[4];
	results[0] = getVectorLineIntersectionPoint(rayDir, lightPos, wall.getTopLeft(), wall.getTopRight());
	results[1] = getVectorLineIntersectionPoint(rayDir, lightPos, wall.getBottomLeft(), wall.getBottomRight());
	results[2] = getVectorLineIntersectionPoint(rayDir, lightPos, wall.getTopLeft(), wall.getBottomLeft());
	results[3] = getVectorLineIntersectionPoint(rayDir, lightPos, wall.getTopRight(), wall.getBottomRight());

	RayWallIntersection closestIntersection;
	for(unsigned i = 0; i < 4; ++i) 
	{
		if(results[i] == Math::nullVector)
			continue;

		if(closestIntersection.valid) 
		{
			float distance = Math::distanceBetweenPoints(lightPos, results[i]);
			if(distance < closestIntersection.distance) {
				closestIntersection.distance = distance;
				closestIntersection.point = results[i];
			}
		}
		else
		{
			closestIntersection.distance = Math::distanceBetweenPoints(lightPos, results[i]);
			closestIntersection.point = results[i];
			closestIntersection.valid = true;
		}
	}

	return closestIntersection;
}

sf::Vector2f LightRenderer::getVectorLineIntersectionPoint(sf::Vector2f rayDir, sf::Vector2f lightPos, sf::Vector2f lineP1, sf::Vector2f lineP2)
{
	float x1 = lineP1.x;
	float y1 = lineP1.y;
	float x2 = lineP2.x;
	float y2 = lineP2.y;

	float x3 = lightPos.x;
	float y3 = lightPos.y;
	float x4 = lightPos.x + rayDir.x;
	float y4 = lightPos.y + rayDir.y;

	float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if(den == 0.f)
		return Math::nullVector; 

	float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
	float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
	
	if(t > 0 && t < 1 && u > 0)
		return sf::Vector2f(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
	else
		return Math::nullVector; 
}

}

