#include "pch.hpp"
#include "lightRenderer.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"

namespace ph::LightRenderer {

// TODO: Add Z to Light

struct Light
{
	sf::Color color;
	Vec2 pos;
	float startAngle;
	float endAngle;
	float attenuationAddition;
	float attenuationFactor;
	float attenuationSquareFactor;
};

struct Ray
{
	Vec2 direction;
	float angle;
};

struct Wall
{
	Vec2 point1;
	Vec2 point2;
};

struct RayWallIntersection
{
	Vec2 point;
	float distance;
	bool valid = false;
};

namespace 
{
	// debug info	
	u32 rays; 
	bool lightingEnabled = true;
	bool drawRays = false;
	u32 nrOfNoCollisionLights;

	// NOTE: This is used in debug and for optimalization in RenderSystem 
	u32 nrOfCollisionLights; 

	std::vector<FloatRect> lightWalls;
	std::vector<Light> lights;
	std::vector<Light> noCollisionLights;
	std::vector<Vec2> lightTriangleFanVertexData;
	const FloatRect* gameWorldCameraBounds;
	Shader lightShader;
	Shader noCollisionLightShader;
	u32 lightTriangleFanVao, lightTriangleFanVbo;
	u32 noCollisionLightRectVao;
}

void init()
{
	// init ray collision light rendering
	lightShader.init(shader::lightSrc());
	lightShader.initUniformBlock("SharedData", 0);

	glGenVertexArrays(1, &lightTriangleFanVao);
	glBindVertexArray(lightTriangleFanVao);

	glGenBuffers(1, &lightTriangleFanVbo);
	glBindBuffer(GL_ARRAY_BUFFER, lightTriangleFanVbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), Null);

	lightTriangleFanVertexData.reserve(361);


	// init no ray collision light rendering
	noCollisionLightShader.init(shader::noCollisionLightSrc());
	noCollisionLightShader.initUniformBlock("SharedData", 0);

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

void shutDown()
{
	glDeleteBuffers(1, &lightTriangleFanVbo);
	glDeleteVertexArrays(1, &lightTriangleFanVao);
}

void submitBunchOfLightWalls(const std::vector<FloatRect>& walls)
{
	lightWalls.insert(lightWalls.end(), walls.begin(), walls.end());
}

void submitLightWall(FloatRect wall)
{
	if(intersect(*gameWorldCameraBounds, FloatRect(wall.x - 200.f, wall.y - 200.f, wall.w + 400.f, wall.h + 400.f)))
		lightWalls.emplace_back(wall);
}

void submitLight(sf::Color color, Vec2 pos, float startAngle, float endAngle,
				 float attenuationAddition, float attenuationFactor, float attenuationSquareFactor,
				 bool rayCollisionDetection)
{
	// TODO: Culling

	if(lightingEnabled)
	{
		Light light{color, pos, startAngle, endAngle, attenuationAddition, attenuationFactor, attenuationSquareFactor};
		if(rayCollisionDetection)
		{
			lights.emplace_back(light);
			++nrOfCollisionLights;
		}
		else
		{
			noCollisionLights.emplace_back(light);
		}
	}
}

static Vec2 getVectorLineIntersectionPoint(Vec2 rayDir, Vec2 lightPos, Vec2 lineP1, Vec2 lineP2)
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
		return nullVec2; 

	float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
	float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
	
	if(t > 0 && t < 1 && u > 0)
		return Vec2(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
	else
		return nullVec2; 
}

static RayWallIntersection getRayWallClosestIntersection(Vec2 rayDir, Vec2 lightPos, FloatRect wall)
{
	Vec2 results[4];
	results[0] = getVectorLineIntersectionPoint(rayDir, lightPos, wall.pos, wall.topRight());
	results[1] = getVectorLineIntersectionPoint(rayDir, lightPos, wall.bottomLeft(), wall.bottomRight());
	results[2] = getVectorLineIntersectionPoint(rayDir, lightPos, wall.pos, wall.bottomLeft());
	results[3] = getVectorLineIntersectionPoint(rayDir, lightPos, wall.topRight(), wall.bottomRight());

	RayWallIntersection closestIntersection;
	for(u32 i = 0; i < 4; ++i) 
	{
		if(results[i] == nullVec2)
			continue;

		if(closestIntersection.valid) 
		{
			float distance = distanceBetweenPoints(lightPos, results[i]);
			if(distance < closestIntersection.distance) 
			{
				closestIntersection.distance = distance;
				closestIntersection.point = results[i];
			}
		}
		else
		{
			closestIntersection.distance = distanceBetweenPoints(lightPos, results[i]);
			closestIntersection.point = results[i];
			closestIntersection.valid = true;
		}
	}

	return closestIntersection;
}

void flush()
{
	PH_PROFILE_FUNCTION();

	// submit quad which rays will hit if they won't hit anything in the scene
	submitLightWall(FloatRect(gameWorldCameraBounds->x - 100000.f, gameWorldCameraBounds->y - 100000.f,
	                          gameWorldCameraBounds->w + 200000.f, gameWorldCameraBounds->h + 200000.f));

	FloatRect expandedScreenSize(gameWorldCameraBounds->x - 400.f, gameWorldCameraBounds->y - 400.f, 800.f, 800.f);

	for(auto& light : lights)
	{
		// make light position be first vertex of triangle fan
		lightTriangleFanVertexData.emplace_back(light.pos);

		// create vertex data
		{
			PH_PROFILE_SCOPE("create vertex data");

			// TODO: Optimize ray casting algorithm
			for(float angle = light.startAngle; angle <= light.endAngle; angle += 0.5)
			{
				++rays;

				float rad = degreesToRadians(angle);
				Vec2 rayDir(std::cos(rad), std::sin(rad));
				Vec2 nearestIntersectionPoint;
				float nearestIntersectionDistance = INFINITY;
				for(FloatRect& wall : lightWalls)
				{
					auto wallIntersection = getRayWallClosestIntersection(rayDir, light.pos, wall);
					if(wallIntersection.valid && wallIntersection.distance < nearestIntersectionDistance)
					{
						nearestIntersectionPoint = wallIntersection.point;
						nearestIntersectionDistance = wallIntersection.distance;
					}
				}
				lightTriangleFanVertexData.emplace_back(nearestIntersectionPoint);
			}
		}

		// draw light using triangle fan
		PH_PROFILE_SCOPE("draw light triangle fan");
		lightShader.bind();
		lightShader.setUniformVec2("lightPos", light.pos);
		lightShader.setUniformVec4Color("color", light.color);
		lightShader.setUniformFloat("cameraZoom", gameWorldCameraBounds->h / 480);
		lightShader.setUniformFloat("a", light.attenuationAddition);
		lightShader.setUniformFloat("b", light.attenuationFactor);
		lightShader.setUniformFloat("c", light.attenuationSquareFactor);
		glBindVertexArray(lightTriangleFanVao);
		glBindBuffer(GL_ARRAY_BUFFER ,lightTriangleFanVbo); // TODO: Do I have to bind it?
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * lightTriangleFanVertexData.size(), lightTriangleFanVertexData.data(), GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLE_FAN, 0, Cast<u32>(lightTriangleFanVertexData.size()));
		lightTriangleFanVertexData.clear();

		if(drawRays)
		{
			for(auto& point : lightTriangleFanVertexData) 
			{
				Renderer::submitPoint(point, light.color, 0, 7.f);
				Renderer::submitLine(light.color, light.pos, point, 3.f);
			}
		}
	}

	// draw no ray collision detection lights
	{
		PH_PROFILE_SCOPE("draw no collision lights");

		noCollisionLightShader.bind();
		noCollisionLightShader.setUniformFloat("cameraZoom", gameWorldCameraBounds->h / 480);
		for(auto& light : noCollisionLights)
		{
			noCollisionLightShader.setUniformVec2("lightPos", light.pos);
			noCollisionLightShader.setUniformVec4Color("color", light.color);
			noCollisionLightShader.setUniformFloat("a", light.attenuationAddition);
			noCollisionLightShader.setUniformFloat("b", light.attenuationFactor);
			noCollisionLightShader.setUniformFloat("c", light.attenuationSquareFactor);

			glBindVertexArray(noCollisionLightRectVao);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}

	// draw light sources as points
	if(drawRays)
	{
		for(auto& light : lights)
			Renderer::submitPoint(light.pos, light.color, 0, 15.f);
		for(auto& light : noCollisionLights)
			Renderer::submitPoint(light.pos, light.color, 0, 15.f);
	}

	nrOfNoCollisionLights = Cast<u32>(noCollisionLights.size());

	lightWalls.clear();
	lights.clear();
	noCollisionLights.clear();
}

void submitDebug(sf::Color* ambientLightColor)
{
	if(ImGui::BeginTabItem("light renderer"))
	{
		ImGui::Checkbox("lighting", &lightingEnabled);
		ImGui::Checkbox("rays debug", &drawRays);
		ImGui::Text("collision lights: %u", nrOfCollisionLights);
		ImGui::Text("no collision light: %u", nrOfNoCollisionLights);
		ImGui::Text("all light: %u", nrOfNoCollisionLights + nrOfCollisionLights);
		ImGui::Text("light rays: %u", rays);

		auto color = castToNormalizedColorVec4(*ambientLightColor);
		ImGui::ColorEdit3("ambient light color", &color.r);
		*ambientLightColor = castToPackedColor(color); 

		ImGui::EndTabItem();
	}
	rays = 0;
}

void setGameWorldCameraBoundsPtr(const FloatRect* bounds)
{
	gameWorldCameraBounds = bounds;	
}

u32 getNrOfCollisionLights()
{
	return nrOfCollisionLights;
}

}

