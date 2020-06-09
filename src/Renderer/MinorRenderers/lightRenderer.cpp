#include "pch.hpp"
#include "lightRenderer.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"

namespace ph {

static u32 rays; // debug info

static u32 lights;
static bool lightingEnabled = true;
static bool drawRays = false;

static u32 noCollisionLightRectVao;

void LightRenderer::init()
{
	// init ray collision light rendering
	mLightShader.init(shader::lightSrc());
	mLightShader.initUniformBlock("SharedData", 0);

	glGenVertexArrays(1, &mLightTriangleFanVao);
	glBindVertexArray(mLightTriangleFanVao);

	glGenBuffers(1, &mLightTriangleFanVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mLightTriangleFanVbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), Null);

	mLightTriangleFanVertexData.reserve(361);


	// init no ray collision light rendering
	mNoCollisionLightShader.init(shader::noCollisionLightSrc());
	mNoCollisionLightShader.initUniformBlock("SharedData", 0);

	glGenVertexArrays(1, &noCollisionLightRectVao);
	glBindVertexArray(noCollisionLightRectVao);

	float quadVertexData[] = {
		-1.f,  1.f,
		 1.f,  1.f,
		-1.f, -1.f,
		 1.f, -1.f
	};

	u32 quadVbo;
	glGenBuffers(1, &quadVbo);
	glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData), quadVertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), Null);

	glBindVertexArray(0);
}

void LightRenderer::shutDown()
{
	glDeleteBuffers(1, &mLightTriangleFanVbo);
	glDeleteVertexArrays(1, &mLightTriangleFanVao);
}

void LightRenderer::submitBunchOfLightWalls(const std::vector<FloatRect>& walls)
{
	mLightWalls.insert(mLightWalls.end(), walls.begin(), walls.end());
}

void LightRenderer::submitLightWall(FloatRect wall)
{
	if(intersect(*mGameWorldCameraBounds, FloatRect(wall.x - 200.f, wall.y - 200.f, wall.w + 400.f, wall.h + 400.f)))
		mLightWalls.emplace_back(wall);
}

void LightRenderer::submitLight(Light light, bool rayCollisionDetection)
{
	// TODO: Culling

	if(lightingEnabled)
	{
		if(rayCollisionDetection)
			mLights.emplace_back(light);
		else
			mNoCollisionLights.emplace_back(light);
	}	
}

void LightRenderer::flush()
{
	PH_PROFILE_FUNCTION();

	if(mLights.empty())
		return;

	lights = Cast<u32>(mLights.size());

	// submit quad which rays will hit if they won't hit anything in the scene
	submitLightWall(FloatRect(mGameWorldCameraBounds->x - 100000.f, mGameWorldCameraBounds->y - 100000.f,
	                          mGameWorldCameraBounds->w + 200000.f, mGameWorldCameraBounds->h + 200000.f));

	FloatRect expandedScreenSize(mGameWorldCameraBounds->x - 400.f, mGameWorldCameraBounds->y - 400.f, 800.f, 800.f);

	for(auto& light : mLights)
	{
		// make light position be first vertex of triangle fan
		mLightTriangleFanVertexData.emplace_back(light.pos);

		// create vertex data
		{
			PH_PROFILE_SCOPE("create vertex data");

			// TODO: Optimize ray casting algorithm
			for(float angle = light.startAngle; angle <= light.endAngle; angle += 0.5)
			{
				++rays;

				float rad = Math::degreesToRadians(angle);
				Vec2 rayDir(std::cos(rad), std::sin(rad));
				Vec2 nearestIntersectionPoint;
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
		PH_PROFILE_SCOPE("draw light triangle fan");
		mLightShader.bind();
		mLightShader.setUniformVec2("lightPos", light.pos);
		mLightShader.setUniformVec4Color("color", light.color);
		mLightShader.setUniformFloat("cameraZoom", mGameWorldCameraBounds->h / 480);
		mLightShader.setUniformFloat("a", light.attenuationAddition);
		mLightShader.setUniformFloat("b", light.attenuationFactor);
		mLightShader.setUniformFloat("c", light.attenuationSquareFactor);
		glBindVertexArray(mLightTriangleFanVao);
		glBindBuffer(GL_ARRAY_BUFFER ,mLightTriangleFanVbo); // TODO: Do I have to bind it?
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mLightTriangleFanVertexData.size(), mLightTriangleFanVertexData.data(), GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLE_FAN, 0, Cast<u32>(mLightTriangleFanVertexData.size()));
		mLightTriangleFanVertexData.clear();

		if(drawRays)
		{
			for(auto& point : mLightTriangleFanVertexData) 
			{
				Renderer::submitPoint(point, light.color, 0, 7.f);
				Renderer::submitLine(light.color, light.pos, point, 3.f);
			}
		}
	}

	// draw no ray collision detection lights
	{
		PH_PROFILE_SCOPE("draw no collision lights");

		mNoCollisionLightShader.bind();
		mNoCollisionLightShader.setUniformFloat("cameraZoom", mGameWorldCameraBounds->h / 480);
		for(auto& light : mNoCollisionLights)
		{
			mNoCollisionLightShader.setUniformVec2("lightPos", light.pos);
			mNoCollisionLightShader.setUniformVec4Color("color", light.color);
			mNoCollisionLightShader.setUniformFloat("a", light.attenuationAddition);
			mNoCollisionLightShader.setUniformFloat("b", light.attenuationFactor);
			mNoCollisionLightShader.setUniformFloat("c", light.attenuationSquareFactor);

			glBindVertexArray(noCollisionLightRectVao);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}

	// draw light sources as points
	if(drawRays)
	{
		for(auto& light : mLights)
			Renderer::submitPoint(light.pos, light.color, 0, 15.f);
		for(auto& light : mNoCollisionLights)
			Renderer::submitPoint(light.pos, light.color, 0, 15.f);
	}

	mLightWalls.clear();
	mLights.clear();
	mNoCollisionLights.clear();
}

RayWallIntersection LightRenderer::getRayWallClosestIntersection(Vec2 rayDir, Vec2 lightPos, FloatRect wall)
{
	Vec2 results[4];
	results[0] = getVectorLineIntersectionPoint(rayDir, lightPos, wall.pos, wall.topRight());
	results[1] = getVectorLineIntersectionPoint(rayDir, lightPos, wall.bottomLeft(), wall.bottomRight());
	results[2] = getVectorLineIntersectionPoint(rayDir, lightPos, wall.pos, wall.bottomLeft());
	results[3] = getVectorLineIntersectionPoint(rayDir, lightPos, wall.topRight(), wall.bottomRight());

	RayWallIntersection closestIntersection;
	for(u32 i = 0; i < 4; ++i) 
	{
		if(results[i] == Math::nullVec2)
			continue;

		if(closestIntersection.valid) 
		{
			float distance = Math::distanceBetweenPoints(lightPos, results[i]);
			if(distance < closestIntersection.distance) 
			{
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

Vec2 LightRenderer::getVectorLineIntersectionPoint(Vec2 rayDir, Vec2 lightPos, Vec2 lineP1, Vec2 lineP2)
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
		return Math::nullVec2; 

	float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
	float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
	
	if(t > 0 && t < 1 && u > 0)
		return Vec2(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
	else
		return Math::nullVec2; 
}

void LightRenderer::submitDebug(sf::Color* ambientLightColor)
{
	if(ImGui::BeginTabItem("light renderer"))
	{
		ImGui::Checkbox("lighting", &lightingEnabled);
		ImGui::Checkbox("rays debug", &drawRays);
		ImGui::Text("light sources (draw calls): %u", mLights.size());
		ImGui::Text("light rays: %u", rays);

		auto color = castToNormalizedColorVec4(*ambientLightColor);
		ImGui::ColorEdit3("ambient light color", &color.r);
		*ambientLightColor = castToPackedColor(color); 

		ImGui::EndTabItem();
	}
	rays = 0;
}

u32 LightRenderer::getNrOfLights()
{
	return lights;
}

}

