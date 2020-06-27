#include "pch.hpp"
#include "ECS/entitiesParser.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/aiComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"
#include "ECS/entitiesTemplateStorage.hpp"
#include "Renderer/API/shader.hpp"
#include "Resources/textureHolder.hpp"
#include "Resources/animationStatesResources.hpp"
#include "Utilities/random.hpp"

namespace ph {

static void parseComponents(entt::registry& registry, std::vector<Xml>& componentNodes, entt::entity entity)
{
	using namespace component;

	for(auto& componentNode : componentNodes)
	{
		const std::string& componentName = componentNode.getAttribute("name")->toString();

		//NOTE: We get a little time penalty from using assign_or_replace. However we need it for templates that base on other templates.

		if(componentName == "BodyRect")
		{
			auto xNode = componentNode.getAttribute("x");
			auto yNode = componentNode.getAttribute("y");
			auto wNode = componentNode.getAttribute("w");
			auto hNode = componentNode.getAttribute("h");
			float x = xNode ? xNode->toFloat() : 0.f;
			float y = yNode ? yNode->toFloat() : 0.f;
			float w = wNode ? wNode->toFloat() : 0.f;
			float h = hNode ? hNode->toFloat() : 0.f;
			registry.assign_or_replace<BodyRect>(entity, FloatRect(x, y, w, h));
		}
		else if(componentName == "BodyCircle")
		{
			float x = componentNode.getAttribute("x")->toFloat();
			float y = componentNode.getAttribute("y")->toFloat();
			float radius = componentNode.getAttribute("radius")->toFloat();
			registry.assign_or_replace<BodyCircle>(entity, Vec2(x, y), radius);
		}
		else if(componentName == "RenderQuad")
		{
			RenderQuad quad;

			// parse texture
			if(auto textureFilepathXml = componentNode.getAttribute("textureFilepath")) 
			{
				std::string filepath = textureFilepathXml->toString();
				if(loadTexture(filepath))
					quad.texture = &getTexture(filepath);
				else
					PH_EXIT_GAME("EntitiesParser::parseRenderQuad() wasn't able to load texture \"" + filepath + "\"");
			}
			else
			{
				quad.texture = Null;
			}

			// parse shader
			quad.shader = Null;
			// TODO: Enable custom shaders
			//if(auto shaderNameXml = componentNode.getAttribute("shaderName")) 
			//{
			//	PH_ASSERT_UNEXPECTED_SITUATION(componentNode.getAttribute("vertexShaderFilepath").has_value(), "Not specifiled vertexShaderFilepath attribute!");
			//	const std::string vertexShaderFilepath = componentNode.getAttribute("vertexShaderFilepath")->toString();

			//	PH_ASSERT_UNEXPECTED_SITUATION(componentNode.getAttribute("fragmentShaderFilepath").has_value(), "Not specified fragmentShaderFilepath attribute!");
			//	const std::string fragmentShaderFilepath = componentNode.getAttribute("fragmentShaderFilepath")->toString();

			//	auto& sl = ShaderLibrary::getInstance();
			//	const std::string shaderName = shaderNameXml->toString();
			//	if(sl.loadFromFile(shaderName, vertexShaderFilepath.c_str(), fragmentShaderFilepath.c_str()))
			//		quad.shader = sl.get(shaderName);
			//	else
			//		PH_EXIT_GAME("EntitiesParser::parseRenderQuad() wasn't able to load shader!");
			//}
			//else

			// Color parsing
			auto color = componentNode.getAttribute("color");
			quad.color = color ? color->toColor() : sf::Color::White;

			// parse rotation
			auto rotation = componentNode.getAttribute("rotation");
			quad.rotation = rotation ? rotation->toFloat() : 0.f;

			// parse rotation origin
			auto rotationOrigin = componentNode.getAttribute("rotationOrigin");
			quad.rotationOrigin = rotationOrigin ? rotationOrigin->toVec2() : Vec2();

			// parse z
			auto z = componentNode.getAttribute("z");
			quad.z = z ? z->toU8() : 100;

			// assign component
			registry.assign_or_replace<RenderQuad>(entity, quad);
		}
		else if(componentName == "IndoorOutdoor")
		{
			registry.assign_or_replace<IndoorOutdoorBlend>(entity);
		}
		else if(componentName == "TextureRect")
		{
			i32 x = componentNode.getAttribute("x")->toI32();
			i32 y = componentNode.getAttribute("y")->toI32();
			i32 w = componentNode.getAttribute("w")->toI32();
			i32 h = componentNode.getAttribute("h")->toI32();
			IntRect rect(x, y, w, h);
			registry.assign_or_replace<TextureRect>(entity, rect);
		}
		else if(componentName == "LightWall")
		{
			FloatRect rect;
			if(auto x = componentNode.getAttribute("x"))
			{
				rect = FloatRect(
					x->toFloat(),
					componentNode.getAttribute("y")->toFloat(),
					componentNode.getAttribute("w")->toFloat(),
					componentNode.getAttribute("h")->toFloat()
				);
			}
			else
			{
				rect = FloatRect(-1.f, -1.f, -1.f, -1.f);
			}

			registry.assign_or_replace<LightWall>(entity, rect);
		}
		else if(componentName == "PushingArea")
		{
			float directionX = componentNode.getAttribute("pushForceX")->toFloat();
			float directionY = componentNode.getAttribute("pushForceY")->toFloat();
			registry.assign_or_replace<PushingArea>(entity, Vec2(directionX, directionY));
		}
		else if(componentName == "Hint")
		{
			std::string hintName = componentNode.getAttribute("hintName")->toString();
			registry.assign_or_replace<Hint>(entity, hintName);
		}
		else if(componentName == "CharacterSpeed")
		{
			float speed = componentNode.getAttribute("speed")->toFloat();
			registry.assign_or_replace<CharacterSpeed>(entity, speed);
		}
		else if(componentName == "CollisionWithPlayer")
		{
			float pushForce = componentNode.getAttribute("pushForce")->toFloat();
			registry.assign_or_replace<CollisionWithPlayer>(entity, pushForce, false);
		}
		else if(componentName == "Kinematics")
		{
			float friction = componentNode.getAttribute("friction")->toFloat();
			registry.assign_or_replace<Kinematics>(entity, Vec2(), Vec2(), friction, friction, 0.5f);
		}
		else if(componentName == "Health")
		{
			i16 healthPoints = componentNode.getAttribute("healthPoints")->toI16();
			i16 maxHealthPoints = componentNode.getAttribute("maxHealthPoints")->toI16();
			registry.assign_or_replace<Health>(entity, healthPoints, maxHealthPoints);
		}
		else if(componentName == "Damage")
		{
			i16 damageDealt = componentNode.getAttribute("damageDealt")->toI16();
			registry.assign_or_replace<Damage>(entity, damageDealt);
		}
		else if(componentName == "Medkit")
		{
			i16 addHealthPoints = componentNode.getAttribute("addHealthPoints")->toI16();
			registry.assign_or_replace<Medkit>(entity, addHealthPoints);
		}
		else if(componentName == "Player")
		{
			registry.assign_or_replace<Player>(entity);
		}
		else if(componentName == "Gate")
		{
			registry.assign_or_replace<Gate>(entity);
		}
		else if(componentName == "Lever")
		{
			registry.assign_or_replace<Lever>(entity);
		}
		else if(componentName == "VelocityChangingEffect")
		{
			float velocityMultiplier = componentNode.getAttribute("velocityMultiplier")->toFloat();
			registry.assign_or_replace<AreaVelocityChangingEffect>(entity, velocityMultiplier);
		}
		else if(componentName == "KinematicCollisionBody")
		{
			float mass = componentNode.getAttribute("mass")->toFloat();
			registry.assign_or_replace<KinematicCollisionBody>(entity, mass);
		}
		else if(componentName == "StaticCollisionBody")
		{
			registry.assign_or_replace<StaticCollisionBody>(entity);
		}
		else if(componentName == "MultiStaticCollisionBody")
		{
			registry.assign_or_replace<MultiStaticCollisionBody>(entity);
		}
		else if(componentName == "FaceDirection")
		{
			registry.assign_or_replace<FaceDirection>(entity, Vec2(0, 0));
		}
		else if(componentName == "Lifetime")
		{
			const float entityLifetime = componentNode.getAttribute("lifetime")->toFloat();
			registry.assign_or_replace<Lifetime>(entity, entityLifetime);
		}
		else if(componentName == "ParticleEmitter")
		{
			// TODO: Make them be attributes of ParticleNode and not separate nodes

			ParticleEmitter emitter;
			auto particleAttribs = componentNode.getChildren("particleAttrib");
			bool wasEndColorAssigned = false;
			bool wasInitialVelocityRandomAssigned = false;
			for(const auto& attrib : particleAttribs)
			{
				const std::string name = attrib.getAttribute("name")->toString();
				if(name == "startColor") 
				{
					u8 r = attrib.getAttribute("r")->toU8();
					u8 g = attrib.getAttribute("g")->toU8();
					u8 b = attrib.getAttribute("b")->toU8();
					u8 a = attrib.getAttribute("a")->toU8();
					if(!wasEndColorAssigned)
						emitter.parEndColor = {r, g, b, a};
					emitter.parStartColor = {r, g, b, a};
				}
				else if(name == "endColor") 
				{
					u8 r = attrib.getAttribute("r")->toU8();
					u8 g = attrib.getAttribute("g")->toU8();
					u8 b = attrib.getAttribute("b")->toU8();
					u8 a = attrib.getAttribute("a")->toU8();
					emitter.parEndColor = {r, g, b, a};
					wasEndColorAssigned = true;
				}
				else if(name == "texture") 
				{
					const std::string filepath = attrib.getAttribute("filepath")->toString();
					if(loadTexture(filepath))
						emitter.parTexture = &getTexture(filepath);
					else
						PH_EXIT_GAME("EntitiesParser::parseParticleEmitter() wasn't able to load texture!");
				}
				else if(name == "spawnPositionOffset") 
				{
					const float x = attrib.getAttribute("x")->toFloat();
					const float y = attrib.getAttribute("y")->toFloat();
					emitter.spawnPositionOffset = {x, y};
				}
				else if(name == "randomSpawnAreaSize") 
				{
					const float w = attrib.getAttribute("w")->toFloat();
					const float h = attrib.getAttribute("h")->toFloat();
					emitter.randomSpawnAreaSize = {w, h};
				}
				else if(name == "initialVelocity") 
				{
					const float x = attrib.getAttribute("x")->toFloat();
					const float y = attrib.getAttribute("y")->toFloat();
					emitter.parInitialVelocity = {x, y};
					if(!wasInitialVelocityRandomAssigned)
						emitter.parInitialVelocityRandom = emitter.parInitialVelocity;
				}
				else if(name == "initialVelocityRandom") 
				{
					const float x = attrib.getAttribute("x")->toFloat();
					const float y = attrib.getAttribute("y")->toFloat();
					emitter.parInitialVelocityRandom = {x, y};
				}
				else if(name == "acceleration") 
				{
					const float x = attrib.getAttribute("x")->toFloat();
					const float y = attrib.getAttribute("y")->toFloat();
					emitter.parAcceleration = {x, y};
				}
				else if(name == "size") 
				{
					const float x = attrib.getAttribute("x")->toFloat();
					const float y = attrib.getAttribute("y")->toFloat();
					emitter.parSize = {x, y};
				}
				else if(name == "amount") 
				{
					emitter.amountOfParticles = attrib.getAttribute("v")->toU16();
				}
				else if(name == "lifetime") 
				{
					emitter.parWholeLifetime = attrib.getAttribute("v")->toFloat();
				}
				else if(name == "z") 
				{
					emitter.parZ = attrib.getAttribute("v")->toU8();
				}
				else if(name == "isEmitting") 
				{
					emitter.isEmitting = attrib.getAttribute("v")->toBool();
				}
			}
			registry.assign_or_replace<ParticleEmitter>(entity, emitter);
		}
		else if(componentName == "MultiParticleEmitter")
		{
			registry.assign_or_replace<MultiParticleEmitter>(entity);
		}
		else if(componentName == "Zombie")
		{
			Zombie zombie;
			zombie.timeFromLastGrowl = Random::generateNumber(0.f, 2.5f);
			zombie.timeToMoveToAnotherTile = componentNode.getAttribute("timeToMoveToAnotherTile")->toFloat();
			registry.assign_or_replace<Zombie>(entity, zombie);
		}
		else if(componentName == "SlowZombieBehavior")
		{
			registry.assign_or_replace<SlowZombieBehavior>(entity);
		}
		else if(componentName == "RenderChunk")
		{
			registry.assign_or_replace<RenderChunk>(entity);
		}
		else if(componentName == "GroundRenderChunk")
		{
			registry.assign_or_replace<GroundRenderChunk>(entity);
		}
		else if(componentName == "BulletBox")
		{
			registry.assign_or_replace<BulletBox>(entity);
		}
		else if(componentName == "PressurePlate")
		{
			registry.assign_or_replace<PressurePlate>(entity);
		}
		else if(componentName == "PuzzleColor")
		{
			registry.assign_or_replace<PuzzleColor>(entity);
		}
		else if(componentName == "Spikes")
		{
			registry.assign_or_replace<Spikes>(entity);
		}
		else if(componentName == "GunAttacker")
		{
			GunAttacker gunAttacker;
			gunAttacker.isTryingToAttack = componentNode.getAttribute("isTryingToAttack")->toBool();
			gunAttacker.timeBeforeHiding = componentNode.getAttribute("timeBeforeHiding")->toFloat();
			gunAttacker.timeToHide = 0.f;
			registry.assign_or_replace<GunAttacker>(entity, gunAttacker);
		}
		else if(componentName == "MeleeProperties")
		{
			MeleeProperties mp;
			mp.minHitInterval = componentNode.getAttribute("minHitInterval")->toFloat();
			mp.rotationSpeed = componentNode.getAttribute("rotationSpeed")->toFloat();
			mp.rotationRange = componentNode.getAttribute("rotationRange")->toFloat();
			mp.range = componentNode.getAttribute("range")->toFloat();
			mp.damage = componentNode.getAttribute("damage")->toU16();
			registry.assign_or_replace<MeleeProperties>(entity, mp);
		}
		else if(componentName == "GunProperties")
		{
			GunProperties gp;

			gp.shotSoundFilepath = componentNode.getAttribute("shotSoundFilepath")->toString();
			gp.range = componentNode.getAttribute("range")->toFloat();
			gp.deflectionAngle = componentNode.getAttribute("deflectionAngle")->toFloat();
			gp.damage = componentNode.getAttribute("damage")->toU16();
			gp.numberOfBullets = componentNode.getAttribute("numberOfBullets")->toU16();
			gp.gunId = componentNode.getAttribute("gunId")->toU8();
			
			const std::string type = componentNode.getAttribute("type")->toString();
			if(type == "pistol")
				gp.type = GunProperties::Type::Pistol;
			else if(type == "shotgun")
				gp.type = GunProperties::Type::Shotgun;
			else
				PH_UNEXPECTED_SITUATION("Unknown Gun type!");
			
			registry.assign_or_replace<GunProperties>(entity, gp);
		}
		else if(componentName == "CurrentGun")
		{
			registry.assign_or_replace<CurrentGun>(entity);
		}
		else if(componentName == "CurrentMeleeWeapon")
		{
			registry.assign_or_replace<CurrentMeleeWeapon>(entity);
		}
		else if(componentName == "Killable")
		{
			registry.assign_or_replace<Killable>(entity);
		}
		else if(componentName == "Bullets")
		{
			Bullets bullets;
			bullets.numOfPistolBullets = componentNode.getAttribute("numOfPistolBullets")->toU16();
			bullets.numOfShotgunBullets = componentNode.getAttribute("numOfShotgunBullets")->toU16();
			registry.assign_or_replace<Bullets>(entity, bullets);
		}
		else if(componentName == "HiddenForRenderer")
		{
			registry.assign_or_replace<HiddenForRenderer>(entity);
		}
		else if(componentName == "SavePoint")
		{
			registry.assign_or_replace<SavePoint>(entity);
		}
		else if(componentName == "PuzzleBoulder")
		{
			registry.assign_or_replace<PuzzleBoulder>(entity);
		}
		else if(componentName == "PuzzleGridPos")
		{
			registry.assign_or_replace<PuzzleGridPos>(entity);
		}
		else if(componentName == "PuzzleId")
		{
			registry.assign_or_replace<PuzzleId>(entity);
		}
		else if(componentName == "CameraRoom")
		{
			registry.assign_or_replace<CameraRoom>(entity);
		}
		else if(componentName == "MovingPlatform")
		{
			registry.assign_or_replace<MovingPlatform>(entity);
		}
		else if(componentName == "FallingPlatform")
		{
			registry.assign_or_replace<FallingPlatform>(entity);
		}
		else if(componentName == "Camera")
		{
			Camera camera;
			float x = componentNode.getAttribute("x")->toFloat();
			float y = componentNode.getAttribute("y")->toFloat();
			float w = componentNode.getAttribute("w")->toFloat();
			float h = componentNode.getAttribute("h")->toFloat();
			camera.bounds = FloatRect(x, y, w, h);
			camera.name = componentNode.getAttribute("cameraName")->toString();
			registry.assign_or_replace<Camera>(entity, camera);
		}
		else if(componentName == "LightSource")
		{
			LightSource lightSource;
			lightSource.offset = { componentNode.getAttribute("offsetX")->toFloat(), componentNode.getAttribute("offsetY")->toFloat() };
			lightSource.color = componentNode.getAttribute("color")->toColor();
			auto startAngle = componentNode.getAttribute("startAngle");
			lightSource.startAngle = startAngle ? startAngle->toFloat() : 0.f;
			auto endAngle = componentNode.getAttribute("endAngle");
			lightSource.endAngle = endAngle ? endAngle->toFloat() : 360.f;
			if(auto attenuationAddition = componentNode.getAttribute("attenuationAddition"))
				lightSource.attenuationAddition = attenuationAddition->toFloat();
			if(auto attenuationFactor = componentNode.getAttribute("attenuationFactor"))
				lightSource.attenuationFactor = attenuationFactor->toFloat();
			if(auto attenuationSquareFactor = componentNode.getAttribute("attenuationSquareFactor"))
				lightSource.attenuationSquareFactor = attenuationSquareFactor->toFloat();
			if(auto rayCollisionDetection = componentNode.getAttribute("rayCollisionDetection"))
				lightSource.rayCollisionDetection = rayCollisionDetection->toBool();
			registry.assign_or_replace<LightSource>(entity, lightSource);
		}
		else if(componentName == "AnimationData")
		{
			AnimationData animationData;

			std::string animationStateFilepath = componentNode.getAttribute("animationStatesFile")->toString();
			loadAnimationStatesFromFile(animationStateFilepath);
			animationData.states = getAnimationStates(animationStateFilepath);
			
			animationData.currentStateName = componentNode.getAttribute("firstStateName")->toString();

			float delay = componentNode.getAttribute("delay")->toFloat();
			animationData.delay = delay;
			animationData.elapsedTime = delay;

			if(auto isPlaying = componentNode.getAttribute("isPlaying"))
				animationData.isPlaying = isPlaying->toBool();
			
			registry.assign_or_replace<AnimationData>(entity, animationData);
		}
		else if(componentName == "Puzzle")
		{
			registry.assign_or_replace<Puzzle>(entity);
		}
		else if(componentName == "WeatherArea")
		{
			registry.assign_or_replace<WeatherArea>(entity);
		}
		else if(componentName == "AlwaysInsideSimRegion")
		{
			registry.assign_or_replace<DontCareAboutSimRegion>(entity);
			registry.assign_or_replace<InsideSimRegion>(entity);
		}
		else if(componentName == "NeverInsideSimRegion")
		{
			registry.assign_or_replace<DontCareAboutSimRegion>(entity);
		}
		else
		{
			PH_EXIT_GAME("Component " + componentName + " wasn't found in Entities Parser");
		}
	}
}

void parseEntities(const std::string& filePath, EntitiesTemplateStorage& templateStorage,
                   entt::registry& gameRegistry)
{
	Xml entitiesFile;
	PH_ASSERT_CRITICAL(entitiesFile.loadFromFile(filePath), "entities file \"" + filePath + "\"wasn't loaded correctly!");
	auto rootNode = entitiesFile.getChild("root");

	// parse templates
	auto entityTemplatesNode = rootNode->getChild("entityTemplates");
	std::vector<Xml> entityTemplates = entityTemplatesNode->getChildren("entityTemplate");
	for(auto& entityTemplate : entityTemplates)
	{
		std::string templateName = entityTemplate.getAttribute("name")->toString();
		auto entity = templateStorage.create(templateName);
		if(auto sourceTemplate = entityTemplate.getAttribute("sourceTemplate"))
			templateStorage.stomp(entity, sourceTemplate->toString(), templateStorage.getTemplateRegistry());
		auto componentNodes = entityTemplate.getChildren("component");
		if(!componentNodes.empty())
			parseComponents(templateStorage.getTemplateRegistry(), componentNodes, entity);
	}

	// parse entities
	const auto entitiesNode = rootNode->getChild("entities");
	if(entitiesNode)
	{
		std::vector<Xml> entities = entitiesNode->getChildren("entity");
		for(auto& entity : entities) 
		{
			auto newEntity = gameRegistry.create();
			if(auto sourceTemplate = entity.getAttribute("sourceTemplate"))
				templateStorage.stomp(newEntity, sourceTemplate->toString(), gameRegistry);
			auto componentNodes = entity.getChildren("component");
			if(!componentNodes.empty())
				parseComponents(gameRegistry, componentNodes, newEntity);
		}
	}
}

}

