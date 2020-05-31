#include "pch.hpp"
#include "tiledParser.hpp"

#include "Components/physicsComponents.hpp"
#include "Components/charactersComponents.hpp"
#include "Components/graphicsComponents.hpp"
#include "Components/objectsComponents.hpp"
#include "Components/itemComponents.hpp"
#include "Components/particleComponents.hpp"
#include "Components/debugComponents.hpp"

#include "entitiesTemplateStorage.hpp"
#include "Scenes/sceneManager.hpp"
#include "Resources/textureHolder.hpp"
#include "Renderer/API/shader.hpp"

namespace ph {

using namespace component;

static void loadEntity(const Xml& entityNode, EntitiesTemplateStorage& templates, 
                       entt::registry& registry, SceneManager& sceneManager, bool* isPlayerOnScene)
{
	auto type = entityNode.getAttribute("type")->toString();

	// NOTE: entity is initialized inside block after if statement. It is declared here for convenience 
	//       so we can capture it with lambda and we don't have to pass it to lambda as argument.
	entt::entity entity;

	auto getPosAttribute = [&]()
	{
		auto x = entityNode.getAttribute("x");
		auto y = entityNode.getAttribute("y");
		PH_ASSERT_UNEXPECTED_SITUATION(x && y, "entity of type " + entityNode.getAttribute("type")->toString() + " doesn't have position");
		return Vec2(x->toFloat(), y->toFloat());
	};

	auto getSizeAttribute = [&]()
	{
		auto w = entityNode.getAttribute("width");
		auto h = entityNode.getAttribute("height");
		PH_ASSERT_UNEXPECTED_SITUATION(w && h, "entity of type " + entityNode.getAttribute("type")->toString() + " doesn't have size");
		return Vec2(w->toFloat(), h->toFloat());
	};

	auto getPosAndSizeAttributes = [&]()
	{
		return FloatRect(getPosAttribute(), getSizeAttribute());
	};

	auto getOptionalSizeAttribute = [&]() -> std::optional<Vec2>
	{
		auto w = entityNode.getAttribute("width");
		auto h = entityNode.getAttribute("height");
		if(w && h)
			return Vec2(w->toFloat(), h->toFloat());
		else
			return std::nullopt;
	};

	auto loadPos = [&]()
	{
		auto& body = registry.get<BodyRect>(entity);
		body.pos = getPosAttribute();
		return body;
	};

	auto loadSize = [&]()
	{
		auto& body = registry.get<BodyRect>(entity);
		body.size = getSizeAttribute();
		return body;
	};

	auto loadPosAndSize = [&]()
	{
		auto& body = registry.get<BodyRect>(entity);
		body.pos = getPosAttribute();
		body.size = getSizeAttribute();
		return body;
	};

	auto loadPosAndOptionalSize = [&]()
	{
		auto& body = registry.get<BodyRect>(entity);
		body.pos = getPosAttribute();
		if(auto size = getOptionalSizeAttribute())
			body.size = *size;
		return body;
	};

	auto getOffsetFromTileTopLeft = [](Vec2 pos)
	{
		float remX = abs(fmod(pos.x, 16.f));
		float remY = abs(fmod(pos.y, 16.f));
		return Vec2(
			pos.x > 0.f ? remX : 16.f - remX,
			pos.y > 0.f ? remY : 16.f - remY
		);
	};

	auto loadAndAlignPos = [&]()
	{
		auto& body = registry.get<BodyRect>(entity);
		body.pos = getPosAttribute();
		body.pos -= getOffsetFromTileTopLeft(body.pos);
		return body;
	};

	auto loadAndAlignPosAndSize = [&]()
	{
		auto& body = registry.get<BodyRect>(entity);
		body.pos = getPosAttribute();
		body.size = getSizeAttribute();
		Vec2 bottomRight = body.bottomRight();
		body.pos -= getOffsetFromTileTopLeft(body.pos); 
		bottomRight += Vec2(16.f);
		bottomRight -= getOffsetFromTileTopLeft(bottomRight); 
		body.size = bottomRight - body.pos;
		return body;
	};

	auto loadPuzzleGridPos = [&](const FloatRect& body)
	{	
		auto& puzzleGridPos = registry.get<PuzzleGridPos>(entity);
		auto center = body.center();
		if(center.x < 0.f) center.x -= 16.f;
		if(center.y < 0.f) center.y -= 16.f;
		puzzleGridPos = Cast<Vec2i>(Math::hadamardDiv(center, Vec2(16, 16)));
	};

	auto createAndLoadPosAndSize = [&]()
	{
		registry.assign<BodyRect>(entity);
		loadPosAndSize();
	};

	auto getProperty = [&](const std::string& propertyName)
	{
		if(const auto propertiesNode = entityNode.getChild("properties")) 
		{
			const std::vector<Xml> properties = propertiesNode->getChildren("property");
			for(const auto& property : properties)
			{
				if(property.getAttribute("name")->toString() == propertyName)
					return *property.getAttribute("value");
			}
		}

		Xml objectTypesFile;
		PH_ASSERT_CRITICAL(objectTypesFile.loadFromFile("scenes/map/objecttypes.xml"), "Tiled object type file \"scenes/map/objecttypes.xml\" wasn't loaded correctly!");
		const auto objectTypesNode = objectTypesFile.getChild("objecttypes");
		std::vector<Xml> objectNodes = objectTypesNode->getChildren("objecttype");
		for(const auto& objectNode : objectNodes)
		{
			if(objectNode.getAttribute("name")->toString() == type) 
			{
				std::vector<Xml> propertiesNodes = objectNode.getChildren("property");
				for(const auto& propertyNode : propertiesNodes)
				{
					if(propertyNode.getAttribute("name")->toString() == propertyName)
					{
						return *propertyNode.getAttribute("default");
					}
				}
			}
		}

		PH_UNEXPECTED_SITUATION("property " + propertyName + " for entity type " + type + " wasn't found!");
		return Xml();
	};

	auto loadHealthComponent = [&]()
	{
		auto& healthComponent = registry.get<Health>(entity);
		healthComponent.healthPoints = getProperty("hp").toU32();
		healthComponent.maxHealthPoints = getProperty("maxHp").toU32();
	};

	auto loadIndoorOutdoorBlendComponent = [&]()
	{
		auto& io = registry.get<IndoorOutdoorBlend>(entity);
		bool outdoor = getProperty("outdoor").toBool();
		io.outdoor = outdoor ? 1.f : 0.f;
		if(outdoor == 1.f)
		{
			io.outdoor = 1.f;
			io.brightness = 1.f; 
			io.alpha = 1.f; 
		}
		else
		{
			io.outdoor = 0.f;
			io.brightness = 1.f; 
			io.alpha = 0.f; 
		}
	};

	auto loadPuzzleColorAndTextureRect = [&]()
	{
		auto& puzzleColor = registry.get<PuzzleColor>(entity);
		auto& textureRect = registry.get<TextureRect>(entity);
		auto color = getProperty("color").toString();
		using component::PuzzleColor;
		if(color == "red")
		{
			puzzleColor = PuzzleColor::Red;
			textureRect.x = 18;
		}
		else if(color == "green")
		{
			puzzleColor = PuzzleColor::Green;
			textureRect.x = 36;
		}
		else if(color == "blue")
		{
			puzzleColor = PuzzleColor::Blue;
			textureRect.x = 54;
		}
		else 
		{
			puzzleColor = PuzzleColor::Grey;
			textureRect.x = 0;
		}
	};

	auto createCopy = [&](const std::string& templateName)
	{
		entity = templates.createCopy(templateName, registry);
	};

	if(type == "Zombie" || type == "SlowZombie")
	{
		createCopy(type);
		loadPos();
		loadHealthComponent();
		loadIndoorOutdoorBlendComponent();
	}
	else if(type == "Player") 
	{
		*isPlayerOnScene = true;

		createCopy("Player");
		auto& playerBody = registry.get<BodyRect>(entity);
		auto& playerCamera = registry.get<component::Camera>(entity);
		
		playerBody.pos = sceneManager.hasPlayerPositionForNextScene() ?
			sceneManager.getPlayerPositionForNextScene() : getPosAttribute();

		playerCamera = ph::Camera(playerBody.center(), Vec2(640, 360));

		templates.createCopy("Pistol", registry);
		auto shotgun = templates.createCopy("Shotgun", registry);
		auto melee = templates.createCopy("BaseballBat", registry);
		registry.assign<CurrentMeleeWeapon>(melee);

		if(getProperty("hasFlashlight").toBool()) 
		{
			LightSource flashlight;
			flashlight.offset = {10.f, 10.f};
			flashlight.color = sf::Color(255, 255, 255, 90);
			flashlight.startAngle = 40.f;
			flashlight.endAngle = 40.f;
			flashlight.attenuationAddition = 0.1f;
			flashlight.attenuationFactor = 3.f;
			flashlight.attenuationSquareFactor = 1.5f;
			registry.assign_or_replace<LightSource>(entity, flashlight);
		}	
	}
	else if(type == "Camera") 
	{
		if(getProperty("isValid").toBool()) 
		{
			createCopy("Camera");
			auto& camera = registry.get<component::Camera>(entity);
			Vec2 pos = getPosAttribute();
			Vec2 size = getSizeAttribute();
			Vec2 center(pos + (size / 2.f));
			camera = ph::Camera(center, size);
			camera.name = getProperty("name").toString();
		}
	}
	else if(type == "BulletBox") 
	{
		createCopy("BulletBox");
		loadPos();
		loadIndoorOutdoorBlendComponent();
		auto& bullets = registry.get<Bullets>(entity);
		bullets.numOfPistolBullets = getProperty("numOfPistolBullets").toI32();
		bullets.numOfShotgunBullets = getProperty("numOfShotgunBullets").toI32();
	}
	else if(type == "Medkit") 
	{
		createCopy("Medkit");
		loadPos();
		loadIndoorOutdoorBlendComponent();
	}
	else if(type == "VelocityChangingArea")
	{
		createCopy("VelocityChangingArea");
		loadPos();
		loadSize();
		float& areaSpeedMultiplier = registry.get<AreaVelocityChangingEffect>(entity).areaSpeedMultiplier;
		areaSpeedMultiplier = getProperty("velocityMultiplier").toFloat();
	}
	else if(type == "PushingArea")
	{
		createCopy("PushingArea");
		loadPos();
		loadSize();
		auto& pushDirection = registry.get<PushingArea>(entity);
		pushDirection.pushForce.x = getProperty("pushForceX").toFloat();
		pushDirection.pushForce.y = getProperty("pushForceY").toFloat();
	}
	else if(type == "HintArea")
	{
		createCopy("HintArea");
		loadPos();
		loadSize();
		auto& hint = registry.get<Hint>(entity);
		hint.hintName = getProperty("hintName").toString();
		hint.keyboardContent = getProperty("hintKeyboardContent").toString();
		hint.joystickContent = getProperty("hintJoystickContent").toString();
	}
	else if(type == "Gate")
	{
		createCopy("Gate");
		loadPos();
		auto& gate = registry.get<Gate>(entity);
		gate.id = getProperty("id").toU32();
	}
	else if(type == "Lever") 
	{
		createCopy("Lever");
		auto& body = loadPos();
		loadIndoorOutdoorBlendComponent();
		auto& lever = registry.get<Lever>(entity);
		lever.id = getProperty("id").toU32();
		lever.puzzleId = getProperty("puzzleId").toU32();
		lever.active = getProperty("active").toBool();
		lever.turnOffAfterSwitch = getProperty("turnOffAfterSwitch").toBool();
	}
	else if(type == "Sprite")
	{
		// create sprite entity
		createCopy("Sprite");
		auto& [rq, body] = registry.get<RenderQuad, BodyRect>(entity);

		// load texture
		const std::string texturePath = getProperty("texturePath").toString();
		if(texturePath != "none") 
		{
			if(loadTexture(texturePath))
				rq.texture = &getTexture(texturePath);
			else
				PH_EXIT_GAME("TiledParser::loadSprite() wasn't able to load texture \"" + texturePath + "\"");
		}

		// load texture rect
		if(getProperty("activeTextureRect").toBool()) 
		{
			registry.assign_or_replace<TextureRect>(
				entity,
				IntRect(getProperty("textureRectLeft").toI32(),
				        getProperty("textureRectTop").toI32(),
				        getProperty("textureRectWidth").toI32(),
				        getProperty("textureRectHeight").toI32())
			);
		}

		// load hidden forrenderer
		if(getProperty("hiddenForRenderer").toBool())
			registry.assign_or_replace<HiddenForRenderer>(entity);

		// load shader
		rq.shader = Null;
		// TODO: Enable custom shaders
		/*const std::string shaderName = getProperty(spriteNode, "shaderName").toString();
		if(shaderName != "none") {
			const std::string vertexShaderFilepath = getProperty(spriteNode, "vertexShaderFilepath").toString();
			PH_ASSERT_CRITICAL(vertexShaderFilepath != "none", "TiledParser::loadSprite(): Sprite has 'shaderName' but doesn't have 'vertexShaderFilepath'!");
			const std::string fragmentShaderFilepath = getProperty(spriteNode, "vertexShaderFilepath").toString();
			PH_ASSERT_CRITICAL(fragmentShaderFilepath != "none", "TiledParser::loadSprite(): Sprite has 'shaderName' but doesn't have 'fragmentShaderFilepath'!");

			auto& sl = ShaderLibrary::getInstance();
			if(sl.loadFromFile(shaderName, vertexShaderFilepath.c_str(), fragmentShaderFilepath.c_str()))
				rq.shader = sl.get(shaderName);
			else
				PH_EXIT_GAME("EntitiesParser::parseRenderQuad() wasn't able to load shader!");
		}*/

		// load rotation and rotation origin
		rq.rotation = getProperty("rotation").toFloat();
		rq.rotationOrigin.x = getProperty("rotationOriginX").toFloat();
		rq.rotationOrigin.y = getProperty("rotationOriginY").toFloat();

		// load z
		rq.z = getProperty("z").toU8();

		// TODO: Load color
		rq.color = sf::Color::White;

		// load body rect
		loadPosAndSize();
	}
	else if(type == "Torch") 
	{
		createCopy("Torch");
		loadPos();
	}
	else if(type == "LightWall")
	{
		// TODO: Delete this one
		createCopy("LightWall");
		loadPosAndSize();
	}
	else if(type == "FlowingRiver")
	{
		createCopy("FlowingRiver");
		auto& [pushingArea, particleEmitter, body] = registry.get<PushingArea, ParticleEmitter, BodyRect>(entity);
		body.pos = getPosAttribute();
		auto size = getSizeAttribute();
		body.size = size;
		const Vec2 pushForce(getProperty("pushForceX").toFloat(), getProperty("pushForceY").toFloat());
		PH_ASSERT_CRITICAL(pushForce.x == 0.f || pushForce.y == 0.f, "We don't support diagonal flowing rivers! - Either pushForceX or pushForceY must be zero.");
		pushingArea.pushForce = pushForce;
		const float particleAmountMultiplier = getProperty("particleAmountMultiplier").toFloat();
		particleEmitter.amountOfParticles = Cast<u32>(particleAmountMultiplier * size.x * size.y / 100.f);
		particleEmitter.parInitialVelocity = pushForce;
		particleEmitter.parInitialVelocityRandom = pushForce;
		if(pushForce.y > 0.f) 
		{
			particleEmitter.spawnPositionOffset = {0.f, 0.f};
			particleEmitter.randomSpawnAreaSize = {size.x, 0.f};
		}
		else if(pushForce.y < 0.f) 
		{
			particleEmitter.spawnPositionOffset = {0.f, size.y};
			particleEmitter.randomSpawnAreaSize = {size.x, 0.f};
		}
		else if(pushForce.x > 0.f) 
		{
			particleEmitter.spawnPositionOffset = {0.f, 0.f};
			particleEmitter.randomSpawnAreaSize = {0.f, size.y};
		}
		else if(pushForce.x < 0.f) 
		{
			particleEmitter.spawnPositionOffset = {size.x, 0.f};
			particleEmitter.randomSpawnAreaSize = {0.f, size.y};
		}
		particleEmitter.parWholeLifetime = pushForce.x == 0.f ? std::abs(size.y / pushForce.y) : std::abs(size.x / pushForce.x);
	}
	else if(type == "IndoorOutdoorBlendArea")
	{
		using component::IndoorOutdoorBlendArea;
		IndoorOutdoorBlendArea area;
		if(getProperty("exit_left").toBool())       area.exit = IndoorOutdoorBlendArea::Left;
		else if(getProperty("exit_right").toBool()) area.exit = IndoorOutdoorBlendArea::Right;
		else if(getProperty("exit_top").toBool())   area.exit = IndoorOutdoorBlendArea::Top;
		else if(getProperty("exit_down").toBool())  area.exit = IndoorOutdoorBlendArea::Down;

		entity = registry.create();
		registry.assign<IndoorOutdoorBlendArea>(entity, area);
		registry.assign<BodyRect>(entity);
		loadPosAndSize();
	}
	else if(type == "IndoorArea")
	{
		entity = registry.create();
		registry.assign<IndoorArea>(entity);
		createAndLoadPosAndSize();
	}
	else if(type == "OutdoorArea")
	{
		entity = registry.create();
		registry.assign<OutdoorArea>(entity);
		createAndLoadPosAndSize();
	}
	else if(type == "PuzzleBoulder")
	{
		createCopy("PuzzleBoulder");
		auto& body = loadAndAlignPos();
		loadPuzzleGridPos(body);
		loadIndoorOutdoorBlendComponent();
		loadPuzzleColorAndTextureRect();
	}
	else if(type == "PressurePlate")
	{
		createCopy("PressurePlate");
		auto& body = loadAndAlignPos();
		loadPuzzleGridPos(body);
		loadIndoorOutdoorBlendComponent();
		loadPuzzleColorAndTextureRect();
		auto& plate = registry.get<PressurePlate>(entity);
		plate.puzzleId = getProperty("puzzleId").toU32();
		plate.id = getProperty("id").toU32();
		plate.isPressIrreversible = getProperty("isPressIrreversible").toBool();
		if(plate.isPressIrreversible)
		{
			auto& textureRect = registry.get<TextureRect>(entity);
			textureRect.x = 72;
		}
	}
	else if(type == "Spikes")
	{
		createCopy("Spikes");
		loadIndoorOutdoorBlendComponent();
		loadAndAlignPosAndSize();

		auto& spikes = registry.get<Spikes>(entity);
		spikes.puzzleId = getProperty("puzzleId").toU32();
		spikes.id = getProperty("id").toU32();
		spikes.timeToChange = getProperty("timeToChange").toFloat();
		spikes.changeFrequency = getProperty("changeFrequency").toFloat();
		spikes.changes = getProperty("changes").toBool();
		spikes.active = getProperty("active").toBool();

		auto& body = registry.get<BodyRect>(entity);
		auto& textureRect = registry.get<TextureRect>(entity);
		textureRect.size = Cast<Vec2i>(body.size);
	}
	else if(type == "SavePoint")
	{
		createCopy("SavePoint");
		loadPos();
		loadIndoorOutdoorBlendComponent();
	}
	else if(type == "TeleportPoint")
	{
		entity = registry.create();
		registry.assign<BodyRect>(entity);
		registry.assign<TeleportPoint>(entity, getProperty("name").toString());
		loadPos();
	}
	else if(type == "CameraRoom")
	{
		createCopy("CameraRoom");
		loadPosAndSize();
		auto& camRoom = registry.get<CameraRoom>(entity);
		camRoom.edgeAreaSize = getProperty("edgeAreaSize").toFloat();
	}
	else if(type == "MovingPlatform")
	{
		createCopy("MovingPlatform");

		auto& platform = registry.get<MovingPlatform>(entity);
		platform.pathBody = getPosAndSizeAttributes();
		platform.velocity = Vec2(getProperty("velX").toFloat(), getProperty("velY").toFloat());
		platform.active = getProperty("active").toBool();

		auto& body = registry.get<BodyRect>(entity);
		Vec2 offset(getProperty("platformOffsetX").toFloat(), getProperty("platformOffsetY").toFloat());
		body.pos = platform.pathBody.pos + offset;
	}
	else 
	{
		PH_LOG_ERROR("The type of object in map file (" + entityNode.getAttribute("type")->toString() + ") is unknown!");
	}
}

void loadEntitiesFromMapFile(const Xml& mapNode, EntitiesTemplateStorage& templates, entt::registry& registry,
							 SceneManager& sceneManager, bool* isPlayerOnScene)
{
	std::vector<Xml> objectGroupNodes = mapNode.getChildren("objectgroup");
	for(auto& objectGroupNode : objectGroupNodes)
	{
		if((objectGroupNode.getAttribute("name")->toString().find("Entities")) != std::string::npos)
		{
			std::vector<Xml> objects = objectGroupNode.getChildren("object");
			for(const auto& entityNode : objects)
				loadEntity(entityNode, templates, registry, sceneManager, isPlayerOnScene);
		}
	}
}

} 
