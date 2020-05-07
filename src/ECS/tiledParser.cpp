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

static void loadEntity(const Xml& entityNode, EntitiesTemplateStorage& templates, 
                       entt::registry& registry, SceneManager& sceneManager, bool* isPlayerOnScene)
{
	auto type = entityNode.getAttribute("type")->toString();

	// NOTE: entity is initialized inside block after if statement. It is declared here for convenience 
	//       so we can capture it with lambda and we don't have to pass it to lambda as argument.
	entt::entity entity;

	auto createDebugName = [&]()
	{
		#ifndef PH_DISTRIBUTION
		auto& debugName = registry.assign<component::DebugName>(entity);
		auto name = entityNode.getAttribute("type")->toString();
		memcpy(debugName.name, name.c_str(), name.length()); 
		#endif
	};

	auto getPosAttribute = [&]()
	{
		auto x = entityNode.getAttribute("x");
		auto y = entityNode.getAttribute("y");
		PH_ASSERT_UNEXPECTED_SITUATION(x && y, "entity of type " + entityNode.getAttribute("type")->toString() + " doesn't have position");
		return sf::Vector2f(x->toFloat(), y->toFloat());
	};

	auto getSizeAttribute = [&]()
	{
		auto w = entityNode.getAttribute("width");
		auto h = entityNode.getAttribute("height");
		PH_ASSERT_UNEXPECTED_SITUATION(w && h, "entity of type " + entityNode.getAttribute("type")->toString() + " doesn't have size");
		return sf::Vector2f(w->toFloat(), h->toFloat());
	};

	auto getOptionalSizeAttribute = [&]() -> std::optional<sf::Vector2f>
	{
		auto w = entityNode.getAttribute("width");
		auto h = entityNode.getAttribute("height");
		if(w && h)
			return sf::Vector2f(w->toFloat(), h->toFloat());
		else
			return std::nullopt;
	};

	auto loadPosition = [&]()
	{
		auto& bodyRect = registry.get<component::BodyRect>(entity);
		bodyRect.pos = getPosAttribute();
	};

	auto loadSize = [&]()
	{
		auto& bodyRect = registry.get<component::BodyRect>(entity);
		bodyRect.size = getSizeAttribute();
	};

	auto loadPositionAndSize = [&]()
	{
		auto& bodyRect = registry.get<component::BodyRect>(entity);
		bodyRect.pos = getPosAttribute();
		bodyRect.size = getSizeAttribute();
	};

	auto loadPositionAndOptionalSize = [&]()
	{
		auto& bodyRect = registry.get<component::BodyRect>(entity);
		bodyRect.pos = getPosAttribute();
		if(auto size = getOptionalSizeAttribute())
			bodyRect.size = *size;
	};

	auto getProperty = [&](const std::string& propertyName)
	{
		if(const auto propertiesNode = entityNode.getChild("properties")) 
		{
			const std::vector<Xml> properties = propertiesNode->getChildren("property");
			for(const auto& property : properties)
			{
				if(property.getAttribute("name")->toString() == propertyName)
				{
					return *property.getAttribute("value");
				}
			}
		}

		Xml objectTypesFile;
		PH_ASSERT_CRITICAL(objectTypesFile.loadFromFile("scenes/map/objecttypes.xml"),
			"Tiled object type file \"scenes/map/objecttypes.xml\" wasn't loaded correctly!");
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
		auto& healthComponent = registry.get<component::Health>(entity);
		healthComponent.healthPoints = getProperty("hp").toUnsigned();
		healthComponent.maxHealthPoints = getProperty("maxHp").toUnsigned();
	};

	auto loadIndoorOutdoorBlendComponent = [&]()
	{
		auto& io = registry.get<component::IndoorOutdoorBlend>(entity);
		bool outdoor = getProperty("outdoor").toBool();
		io.outdoor = outdoor ? 1.f : 0.f;
		if(outdoor == 1.f)
		{
			io.outdoor = 1.f;
			io.outdoorDarkness = 1.f; 
			io.indoorAlpha = 1.f; 
		}
		else
		{
			io.outdoor = 0.f;
			io.outdoorDarkness = 1.f; 
			io.indoorAlpha = 0.f; 
		}
	};

	auto loadPuzzleColorAndTextureRect = [&]()
	{
		auto& puzzleColor = registry.get<component::PuzzleColor>(entity);
		auto& textureRect = registry.get<component::TextureRect>(entity);
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
		loadPosition();
		loadHealthComponent();
		loadIndoorOutdoorBlendComponent();
		createDebugName();
	}
	else if(type == "Player") 
	{
		*isPlayerOnScene = true;

		createCopy("Player");
		auto& playerBody = registry.get<component::BodyRect>(entity);
		auto& playerCamera = registry.get<component::Camera>(entity);
		
		playerBody.pos = sceneManager.hasPlayerPositionForNextScene() ?
			sceneManager.getPlayerPositionForNextScene() : getPosAttribute();

		playerCamera.camera = Camera(playerBody.center(), sf::Vector2f(640, 360));

		templates.createCopy("Pistol", registry);
		auto shotgun = templates.createCopy("Shotgun", registry);
		auto melee = templates.createCopy("BaseballBat", registry);
		registry.assign<component::CurrentMeleeWeapon>(melee);

		if(getProperty("hasFlashlight").toBool()) 
		{
			component::LightSource flashlight;
			flashlight.offset = {10.f, 10.f};
			flashlight.color = sf::Color(255, 255, 255, 90);
			flashlight.startAngle = 40.f;
			flashlight.endAngle = 40.f;
			flashlight.attenuationAddition = 0.1f;
			flashlight.attenuationFactor = 3.f;
			flashlight.attenuationSquareFactor = 1.5f;
			registry.assign_or_replace<component::LightSource>(entity, flashlight);
		}	

		createDebugName();
	}
	else if(type == "Camera") 
	{
		if(getProperty("isValid").toBool()) 
		{
			createCopy("Camera");
			auto& camera = registry.get<component::Camera>(entity);
			sf::Vector2f pos = getPosAttribute();
			sf::Vector2f size = getSizeAttribute();
			sf::Vector2f center(pos + (size / 2.f));
			camera.camera = Camera(center, size);
			camera.name = getProperty("name").toString();
			createDebugName();
		}
	}
	else if(type == "BulletBox") 
	{
		createCopy("BulletBox");
		loadPosition();
		loadIndoorOutdoorBlendComponent();
		auto& bullets = registry.get<component::Bullets>(entity);
		bullets.numOfPistolBullets = getProperty("numOfPistolBullets").toInt();
		bullets.numOfShotgunBullets = getProperty("numOfShotgunBullets").toInt();
		createDebugName();
	}
	else if(type == "Medkit") 
	{
		createCopy("Medkit");
		loadPosition();
		loadIndoorOutdoorBlendComponent();
		createDebugName();
	}
	else if(type == "VelocityChangingArea")
	{
		createCopy("VelocityChangingArea");
		loadPosition();
		loadSize();
		float& areaSpeedMultiplier = registry.get<component::AreaVelocityChangingEffect>(entity).areaSpeedMultiplier;
		areaSpeedMultiplier = getProperty("velocityMultiplier").toFloat();
		createDebugName();
	}
	else if(type == "PushingArea")
	{
		createCopy("PushingArea");
		loadPosition();
		loadSize();
		auto& pushDirection = registry.get<component::PushingArea>(entity);
		pushDirection.pushForce.x = getProperty("pushForceX").toFloat();
		pushDirection.pushForce.y = getProperty("pushForceY").toFloat();
		createDebugName();
	}
	else if(type == "HintArea")
	{
		createCopy("HintArea");
		loadPosition();
		loadSize();
		auto& hint = registry.get<component::Hint>(entity);
		hint.hintName = getProperty("hintName").toString();
		hint.keyboardContent = getProperty("hintKeyboardContent").toString();
		hint.joystickContent = getProperty("hintJoystickContent").toString();
		createDebugName();
	}
	else if(type == "Gate")
	{
		createCopy("Gate");
		loadPosition();
		auto& gate = registry.get<component::Gate>(entity);
		gate.id = getProperty("id").toUnsigned();
		createDebugName();
	}
	else if(type == "Lever") 
	{
		createCopy("Lever");
		loadPosition();
		loadIndoorOutdoorBlendComponent();
		createDebugName();
	}
	else if(type == "Sprite")
	{
		// create sprite entity
		createCopy("Sprite");
		auto& [rq, body] = registry.get<component::RenderQuad, component::BodyRect>(entity);

		// load texture
		const std::string texturePath = getProperty("texturePath").toString();
		if(texturePath != "none") {
			if(loadTexture(texturePath))
				rq.texture = &getTexture(texturePath);
			else
				PH_EXIT_GAME("TiledParser::loadSprite() wasn't able to load texture \"" + texturePath + "\"");
		}

		// load texture rect
		if(getProperty("activeTextureRect").toBool()) {
			registry.assign_or_replace<component::TextureRect>(
				entity,
				IntRect(
					getProperty("textureRectLeft").toInt(),
					getProperty("textureRectTop").toInt(),
					getProperty("textureRectWidth").toInt(),
					getProperty("textureRectHeight").toInt()
				)
			);
		}

		// load hidden forrenderer
		if(getProperty("hiddenForRenderer").toBool())
			registry.assign_or_replace<component::HiddenForRenderer>(entity);

		// load shader
		rq.shader = nullptr;
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
		rq.z = getProperty("z").toUnsignedChar();

		// TODO: Load color
		rq.color = sf::Color::White;

		// load body rect
		loadPositionAndSize();

		createDebugName();
	}
	else if(type == "Torch") 
	{
		createCopy("Torch");
		loadPosition();
		createDebugName();
	}
	else if(type == "LightWall")
	{
		// TODO: Delete this one
		createCopy("LightWall");
		loadPositionAndSize();
		createDebugName();
	}
	else if(type == "FlowingRiver")
	{
		createCopy("FlowingRiver");
		auto& [pushingArea, particleEmitter, body] = registry.get<component::PushingArea, component::ParticleEmitter, component::BodyRect>(entity);
		body.pos = getPosAttribute();
		auto size = getSizeAttribute();
		body.size = size;
		const sf::Vector2f pushForce(getProperty("pushForceX").toFloat(), getProperty("pushForceY").toFloat());
		PH_ASSERT_CRITICAL(pushForce.x == 0.f || pushForce.y == 0.f, "We don't support diagonal flowing rivers! - Either pushForceX or pushForceY must be zero.");
		pushingArea.pushForce = pushForce;
		const float particleAmountMultiplier = getProperty("particleAmountMultiplier").toFloat();
		particleEmitter.amountOfParticles = static_cast<unsigned>(particleAmountMultiplier * size.x * size.y / 100.f);
		particleEmitter.parInitialVelocity = pushForce;
		particleEmitter.parInitialVelocityRandom = pushForce;
		if(pushForce.y > 0.f) {
			particleEmitter.spawnPositionOffset = {0.f, 0.f};
			particleEmitter.randomSpawnAreaSize = {size.x, 0.f};
		}
		else if(pushForce.y < 0.f) {
			particleEmitter.spawnPositionOffset = {0.f, size.y};
			particleEmitter.randomSpawnAreaSize = {size.x, 0.f};
		}
		else if(pushForce.x > 0.f) {
			particleEmitter.spawnPositionOffset = {0.f, 0.f};
			particleEmitter.randomSpawnAreaSize = {0.f, size.y};
		}
		else if(pushForce.x < 0.f) {
			particleEmitter.spawnPositionOffset = {size.x, 0.f};
			particleEmitter.randomSpawnAreaSize = {0.f, size.y};
		}
		particleEmitter.parWholeLifetime = pushForce.x == 0.f ? std::abs(size.y / pushForce.y) : std::abs(size.x / pushForce.x);
		createDebugName();
	}
	else if(type == "IndoorOutdoorBlendArea")
	{
		using component::IndoorOutdoorBlendArea;
		IndoorOutdoorBlendArea area;
		if(getProperty("exit_left").toBool())
		{
			area.exit = IndoorOutdoorBlendArea::Left;
		}
		else if(getProperty("exit_right").toBool())
		{
			area.exit = IndoorOutdoorBlendArea::Right;
		}
		else if(getProperty("exit_top").toBool())
		{
			area.exit = IndoorOutdoorBlendArea::Top;
		}
		else if(getProperty("exit_down").toBool())
		{
			area.exit = IndoorOutdoorBlendArea::Down;
		}

		entity = registry.create();
		registry.assign<component::IndoorOutdoorBlendArea>(entity, area);
		registry.assign<component::BodyRect>(entity);
		loadPositionAndSize();
		createDebugName();
	}
	else if(type == "PuzzleBoulder")
	{
		createCopy("PuzzleBoulder");
		loadPosition();
		loadIndoorOutdoorBlendComponent();
		loadPuzzleColorAndTextureRect();
		createDebugName();
	}
	else if(type == "PressurePlate")
	{
		createCopy("PressurePlate");
		loadPosition();
		loadIndoorOutdoorBlendComponent();
		loadPuzzleColorAndTextureRect();
		auto& plate = registry.get<component::PressurePlate>(entity);
		plate.puzzleId = getProperty("puzzleId").toUnsigned();
		plate.id = getProperty("id").toUnsigned();
		plate.isPressIrreversible = getProperty("isPressIrreversible").toBool();
		if(plate.isPressIrreversible)
		{
			auto& textureRect = registry.get<component::TextureRect>(entity);
			textureRect.x = 72;
		}
		createDebugName();
	}
	else if(type == "Puzzle")
	{
		entity = registry.create();
		unsigned id = getProperty("id").toUnsigned(); 
		registry.assign<component::Puzzle>(entity, id);
		createDebugName();
	}
	else if(type == "Spikes")
	{
		createCopy("Spikes");
		loadIndoorOutdoorBlendComponent();

		auto pos = getPosAttribute();
		auto size = getSizeAttribute();
		pos.x -= static_cast<float>(static_cast<int>(pos.x) % 16); 
		pos.y -= static_cast<float>(static_cast<int>(pos.y) % 16); 
		size.x = static_cast<float>(size.x + 16 - static_cast<int>(size.x + 16) % 16);
		size.y = static_cast<float>(size.y + 16 - static_cast<int>(size.y + 16) % 16);
		auto& body = registry.get<component::BodyRect>(entity);
		body.pos = pos;
		body.size = size;

		auto& textureRect = registry.get<component::TextureRect>(entity);
		textureRect.size = static_cast<sf::Vector2i>(size);

		createDebugName();
	}
	else 
	{
		PH_LOG_ERROR("The type of object in map file (" + entityNode.getAttribute("type")->toString() + ") is unknown!");
	}
}

void loadEntitiesFromMapFile(const Xml& mapNode, EntitiesTemplateStorage& templates, entt::registry& registry,
							 SceneManager& sceneManager, bool* isPlayerOnScene)
{
	// TODO: Change map layer name in tiled to entities
	std::vector<Xml> objectGroupNodes = mapNode.getChildren("objectgroup");
	for(auto& objectGroupNode : objectGroupNodes)
	{
		if((objectGroupNode.getAttribute("name")->toString() == "gameObjects"))
		{
			std::vector<Xml> objects = objectGroupNode.getChildren("object");
			for(const auto& entityNode : objects)
			{
				loadEntity(entityNode, templates, registry, sceneManager, isPlayerOnScene);
			}
			return;
		}
	}
}

} 
