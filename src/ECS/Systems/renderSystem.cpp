#include "pch.hpp"
#include "renderSystem.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/API/camera.hpp"
#include "Utilities/random.hpp"

namespace {
	ph::Camera defaultCamera = ph::Camera({320, 180}, {640, 360});
}

namespace ph::system {

RenderSystem::RenderSystem(entt::registry& registry, Texture& tileset)
	:System(registry)
{
	Renderer::setChunksTexture(tileset);
}

void RenderSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	// shake camera
	auto shakingCameras = mRegistry.view<component::CameraShake, component::Camera>();
	for(auto cameraEntity : shakingCameras)
	{
		auto& [shake, camera] = shakingCameras.get<component::CameraShake, component::Camera>(cameraEntity);
		if(shake.elapsedTime < shake.duration) {
			sf::Vector2f cameraOffset = Random::generateVector({-1.f, -1.f}, {1.f, 1.f});
			cameraOffset *= shake.magnitude;
			cameraOffset *= (shake.duration - shake.elapsedTime) / shake.duration;
			sf::Vector2f cameraPos = camera.camera.center();
			sf::Vector2f newCameraPos = cameraPos + cameraOffset;
			if(shake.smooth)
				camera.camera.setCenter(Math::lerp(cameraPos, newCameraPos, 2.f));
			else
				camera.camera.setCenter(newCameraPos);
			shake.elapsedTime += dt;
		}
		else {
			mRegistry.remove<component::CameraShake>(cameraEntity);
		}
	}

	// get current camera and update shake
	auto cameras = mRegistry.view<component::Camera>();
	Camera* currentCamera = &defaultCamera;
	cameras.each([&currentCamera](component::Camera& camera) {
		if(camera.name == component::Camera::currentCameraName) {
			currentCamera = &camera.camera;
		}
	});

	// set camera
	Renderer::setGameWorldCamera(*currentCamera);

	// submit light sources
	auto lightSources = mRegistry.view<component::LightSource, component::BodyRect>();
	lightSources.each([](const component::LightSource& pointLight, const component::BodyRect& body)
	{
		PH_ASSERT_UNEXPECTED_SITUATION(pointLight.startAngle <= pointLight.endAngle, "start angle must be lesser or equal to end angle");
		Renderer::submitLight(pointLight.color, body.pos + pointLight.offset, pointLight.startAngle, pointLight.endAngle,
			pointLight.attenuationAddition, pointLight.attenuationFactor, pointLight.attenuationSquareFactor);
	});

	//submit single light walls
	if(Renderer::getNrOfLights() > 0)
	{
		auto lightWalls = mRegistry.view<component::LightWall, component::BodyRect>();
		lightWalls.each([](const component::LightWall& lw, const component::BodyRect& body) 
		{
			if(lw.y == -1.f)
				Renderer::submitLightWall(body);
			else
				Renderer::submitLightWall(FloatRect(body.pos + lw.pos, lw.size));
		});
	}

	auto getIndoorBlendColor = [](component::IndoorBlend ib)
	{
		auto alpha = static_cast<unsigned char>(ib.alpha * 255.f);
		return sf::Color(255, 255, 255, alpha);
	};

	// submit ground chunks with indoor blend
	mRegistry.view<component::GroundRenderChunk, component::IndoorBlend>().each([&]
	(component::GroundRenderChunk& chunk, const component::IndoorBlend ib)
	{
		if(intersect(currentCamera->getBounds(), chunk.bounds))
			Renderer::submitGroundChunk(chunk.bounds.pos, chunk.textureRect, chunk.z, getIndoorBlendColor(ib));
	});

	// submit chunks with indoor blend
	mRegistry.view<component::RenderChunk, component::IndoorBlend>().each([&]
	(component::RenderChunk& chunk, const component::IndoorBlend ib)
	{
		if(intersect(currentCamera->getBounds(), chunk.quadsBounds) && !chunk.quads.empty())
			Renderer::submitChunk(chunk.quads, chunk.quadsBounds, chunk.z, &chunk.rendererID, getIndoorBlendColor(ib));

		if(!chunk.lightWalls.empty() && intersect(currentCamera->getBounds(), chunk.lightWallsBounds)) 
			Renderer::submitBunchOfLightWalls(chunk.lightWalls);
	});

	auto getOutdoorBlendColor = [](component::OutdoorBlend ob)
	{
		auto d = static_cast<unsigned char>(ob.darkness * 255.f);
		return sf::Color(d, d, d, 255);
	};

	// submit ground chunks with outdoor blend
	mRegistry.view<component::GroundRenderChunk, component::OutdoorBlend>().each([&]
	(component::GroundRenderChunk& chunk, const component::OutdoorBlend ob)
	{
		if(intersect(currentCamera->getBounds(), chunk.bounds))
			Renderer::submitGroundChunk(chunk.bounds.pos, chunk.textureRect, chunk.z, getOutdoorBlendColor(ob));
	});

	// submit chunks with outdoor blend
	mRegistry.view<component::RenderChunk, component::OutdoorBlend>().each([&]
	(component::RenderChunk& chunk, const component::OutdoorBlend ob)
	{
		if(intersect(currentCamera->getBounds(), chunk.quadsBounds) && !chunk.quads.empty())
			Renderer::submitChunk(chunk.quads, chunk.quadsBounds, chunk.z, &chunk.rendererID, getOutdoorBlendColor(ob));

		if(!chunk.lightWalls.empty() && intersect(currentCamera->getBounds(), chunk.lightWallsBounds)) 
			Renderer::submitBunchOfLightWalls(chunk.lightWalls);
	});

	auto getIndoorOutdoorColor = [](component::IndoorOutdoorBlend io)
	{
		auto dark = static_cast<unsigned char>(io.outdoorDarkness * 255.f);
		auto alpha = static_cast<unsigned char>(io.indoorAlpha * 255.f);
		return sf::Color(dark, dark, dark, alpha);
	};

	// submit render quads
	mRegistry.view<component::RenderQuad, component::IndoorOutdoorBlend, component::BodyRect>
	(entt::exclude<component::HiddenForRenderer, component::TextureRect>).each([&]
	(const component::RenderQuad& quad, const component::IndoorOutdoorBlend io, const component::BodyRect& body)
	{
		sf::Color color = quad.color * getIndoorOutdoorColor(io);
		Renderer::submitQuad(
			quad.texture, nullptr, &color, quad.shader,
			body.pos, body.size, quad.z, quad.rotation, quad.rotationOrigin);
	});

	// submit render quads with texture rect
	mRegistry.view<component::RenderQuad, component::TextureRect, component::BodyRect, component::IndoorOutdoorBlend>
	(entt::exclude<component::HiddenForRenderer>).each([&]
	(const component::RenderQuad& quad, const component::TextureRect& textureRect, 
	 const component::BodyRect& body, const component::IndoorOutdoorBlend io)
	{
		Renderer::submitQuad(
			quad.texture, &textureRect, &getIndoorOutdoorColor(io), quad.shader,
			body.pos, body.size, quad.z, quad.rotation, quad.rotationOrigin);
	});

	// submit render quads with no indoor outdoor component
	mRegistry.view<component::RenderQuad, component::BodyRect>
	(entt::exclude<component::HiddenForRenderer, component::TextureRect, component::IndoorOutdoorBlend>).each([&]
	(const component::RenderQuad& quad, const component::BodyRect& body)
	{
		Renderer::submitQuad(
			quad.texture, nullptr, &quad.color, quad.shader,
			body.pos, body.size, quad.z, quad.rotation, quad.rotationOrigin);
	});

	// submit render quads with texture rect and no indoor outdoor component
	mRegistry.view<component::RenderQuad, component::TextureRect, component::BodyRect>
	(entt::exclude<component::HiddenForRenderer, component::IndoorOutdoorBlend>).each([&]
	(const component::RenderQuad& quad, const component::TextureRect& textureRect, const component::BodyRect& body)
	{
		Renderer::submitQuad(
			quad.texture, &textureRect, &quad.color, quad.shader,
			body.pos, body.size, quad.z, quad.rotation, quad.rotationOrigin);
	});
}

}
