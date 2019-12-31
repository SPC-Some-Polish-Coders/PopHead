#include "ECS/entitiesParser.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/aiComponents.hpp"
#include "ECS/entitiesTemplateStorage.hpp"
#include "Renderer/API/shader.hpp"
#include "Resources/animationStatesResources.hpp"
#include "Utilities/xml.hpp"
#include "Utilities/random.hpp"

namespace ph {

EntitiesParser::EntitiesParser()
	:mTemplateStorage(nullptr)
	,mUsedRegistry(nullptr)
	,mTextureHolder(nullptr)
{
}

void EntitiesParser::parseFile(const std::string& filePath, EntitiesTemplateStorage& templateStorage, entt::registry& gameRegistry,
                               TextureHolder& textureHolder)
{
	mTextureHolder = &textureHolder;

	mTemplateStorage = &templateStorage;
	Xml entitiesFile;
	entitiesFile.loadFromFile(filePath);

	mUsedRegistry = &templateStorage.getTemplateRegistry();
	const Xml entityTemplatesNode = entitiesFile.getChild("entityTemplates");
	parseTemplates(entityTemplatesNode);

	// TODO: Enable entities parsing in some form
	//mUsedRegistry = &gameRegistry;
	//const Xml entitiesNode = entitiesFile.getChild("entities");
	//parseEntities(entitiesNode);

	mTemplateStorage = nullptr;
	mUsedRegistry = nullptr;
}

bool EntitiesParser::loadedPlayer() const
{
	return mHasLoadedPlayer;
}

void EntitiesParser::parseTemplates(const Xml& entityTemplatesNode)
{
	std::vector<Xml> entityTemplates = entityTemplatesNode.getChildren("entityTemplate");
	for (auto& entityTemplate : entityTemplates)
	{
		const std::string& templateName = entityTemplate.getAttribute("name").toString();
		auto entity = mTemplateStorage->create(templateName);
		if (entityTemplate.hasAttribute("sourceTemplate"))
		{
			const std::string& sourceTemplateName = entityTemplate.getAttribute("sourceTemplate").toString();
			mTemplateStorage->stomp(entity, sourceTemplateName);
		}
		std::vector<Xml> entityComponents = entityTemplate.getChildren("component");
		parseComponents(entityComponents, entity);
	}
}

void EntitiesParser::parseEntities(const Xml& entitiesNode)
{
	std::vector<Xml> entities = entitiesNode.getChildren("entity");
	for (auto& entity : entities)
	{
		auto newEntity = mUsedRegistry->create();
		if (entity.hasAttribute("sourceTemplate"))
		{
			const std::string& sourceTemplateName = entity.getAttribute("sourceTemplate").toString();
			mTemplateStorage->stomp(newEntity, sourceTemplateName, *mUsedRegistry);
		}
		auto entityComponents = entity.getChildren("component");
		parseComponents(entityComponents, newEntity);
	}
}

void EntitiesParser::parseComponents(std::vector<Xml>& entityComponents, entt::entity& entity)
{
	if (entityComponents.empty())
		return;

	std::unordered_map<std::string, void(EntitiesParser::*)(const Xml&, entt::entity&)> mComponentsMap = {
		{"BodyRect",			        &EntitiesParser::parseBodyRect},
		{"RenderQuad",			  	    &EntitiesParser::parseRenderQuad},
		{"TextureRect",			  	    &EntitiesParser::parseTextureRect},
		{"BlocksLight",			  	    &EntitiesParser::parseBlocksLight},
		{"PushingArea",			  	    &EntitiesParser::parsePushingArea},
		{"CharacterSpeed",		  	    &EntitiesParser::parseCharacterSpeed},
		{"Killable",			  	    &EntitiesParser::parseKillable},
		{"Health",	              	    &EntitiesParser::parseHealth},
		{"Damage",	              	    &EntitiesParser::parseDamage},
		{"Medkit",	              	    &EntitiesParser::parseMedkit},
		{"Player",                	    &EntitiesParser::parsePlayer},
		{"Zombie",                	    &EntitiesParser::parseZombie},
		{"Bullets",                	    &EntitiesParser::parseBullets},
		{"Velocity",              	    &EntitiesParser::parseVelocity},
		{"PushingVelocity",             &EntitiesParser::parsePushingVelocity},
		{"Entrance",              	    &EntitiesParser::parseEntrance},
		{"Gate",				  	    &EntitiesParser::parseGate},
		{"Lever",				  	    &EntitiesParser::parseLever},
		{"LeverListener",		  	    &EntitiesParser::parseLeverListener},
		{"CurrentGun",            	    &EntitiesParser::parseCurrentGun},
		{"CurrentMeleeWeapon",    	    &EntitiesParser::parseCurrentMeleeWeapon},
		{"GunProperties",		  	    &EntitiesParser::parseGunProperties},
		{"MeleeProperties",		  	    &EntitiesParser::parseMeleeProperties},
		{"FaceDirection",         	    &EntitiesParser::parseFaceDirection},
		{"Lifetime",			  	    &EntitiesParser::parseLifetime},
		{"Camera",                	    &EntitiesParser::parseCamera},
		{"LightSource",           	    &EntitiesParser::parseLightSource},
		{"HiddenForRenderer",	  	    &EntitiesParser::parseHiddenForRenderer},
		{"GunAttacker",           	    &EntitiesParser::parseGunAttacker},
		{"CollisionWithPlayer",   	    &EntitiesParser::parseCollisionWithPlayer},
		{"StaticCollisionBody",   	    &EntitiesParser::parseStaticCollisionBody},
		{"MultiStaticCollisionBody",    &EntitiesParser::parseMultiStaticCollisionBody},
		{"KinematicCollisionBody", 	    &EntitiesParser::parseKinematicCollisionBody},
		{"VelocityChangingEffect", 	    &EntitiesParser::parseVelocityChangingEffect},
		{"AnimationData",          	    &EntitiesParser::parseAnimationData},
		{"ParticleEmitter",             &EntitiesParser::parseParticleEmitter},
		{"MultiParticleEmitter",        &EntitiesParser::parseMultiParticleEmitter},
		{"RenderChunk",                 &EntitiesParser::parseRenderChunk},
		{"ArcadeSpawner",               &EntitiesParser::parseArcadeSpawner},
		{"LootSpawner",                 &EntitiesParser::parseLootSpawner},
		{"BulletBox",                   &EntitiesParser::parseBulletBox}
	};

	for (auto& entityComponent : entityComponents)
	{
		const std::string& componentName = entityComponent.getAttribute("name").toString();
		(this->*mComponentsMap.at(componentName))(entityComponent, entity);
	}
}

//NOTE: We get a little time penalty from using assign_or_replace. However we need it for templates that base on other templates
//		so it's probably the best option

void EntitiesParser::parseBodyRect(const Xml& entityComponentNode, entt::entity& entity)
{
	float x = entityComponentNode.getAttribute("x").toFloat();
	float y = entityComponentNode.getAttribute("y").toFloat();
	float width = entityComponentNode.getAttribute("width").toFloat();
	float height = entityComponentNode.getAttribute("height").toFloat();
	mUsedRegistry->assign_or_replace<component::BodyRect>(entity, ph::FloatRect(x, y, width, height));
}

void EntitiesParser::parseRenderQuad(const Xml& entityComponentNode, entt::entity& entity)
{
	component::RenderQuad quad;

	// parse texture
	if(entityComponentNode.hasAttribute("textureFilepath")) {
		const std::string filepath = entityComponentNode.getAttribute("textureFilepath").toString();
		if(mTextureHolder->load(filepath))
			quad.texture = &mTextureHolder->get(filepath);
		else
			PH_EXIT_GAME("EntitiesParser::parseTexture() wasn't able to load texture \"" + filepath + "\"");
	}
	else
		quad.texture = nullptr;

	// parse shader
	if(entityComponentNode.hasAttribute("shaderName")) {
		// TODO: Optimize that
		const std::string shaderName = entityComponentNode.getAttribute("shaderName").toString();

		PH_ASSERT_UNEXPECTED_SITUATION(entityComponentNode.hasAttribute("vertexShaderFilepath"), "Not specifiled vertexShaderFilepath attribute!");
		const std::string vertexShaderFilepath = entityComponentNode.getAttribute("vertexShaderFilepath").toString();

		PH_ASSERT_UNEXPECTED_SITUATION(entityComponentNode.hasAttribute("fragmentShaderFilepath"), "Not specified fragmentShaderFilepath attribute!");
		const std::string fragmentShaderFilepath = entityComponentNode.getAttribute("fragmentShaderFilepath").toString();

		auto& sl = ShaderLibrary::getInstance();
		if(sl.loadFromFile(shaderName, vertexShaderFilepath.c_str(), fragmentShaderFilepath.c_str()))
			quad.shader = sl.get(shaderName);
		else
			PH_EXIT_GAME("EntitiesParser::parseShader() wasn't able to load shader!");
	}
	else
		quad.shader = nullptr;

	// Color parsing
	if(entityComponentNode.hasAttribute("color"))
		quad.color = entityComponentNode.getAttribute("color").toColor();
	else
		quad.color = sf::Color::White;

	// parse rotation
	if(entityComponentNode.hasAttribute("rotation"))
		quad.rotation = entityComponentNode.getAttribute("rotation").toFloat();
	else
		quad.rotation = 0.f;

	// parse rotation origin
	if(entityComponentNode.hasAttribute("rotationOrigin"))
		quad.rotationOrigin = entityComponentNode.getAttribute("rotationOrigin").toVector2f();
	else
		quad.rotationOrigin = {};


	// parse z
	PH_ASSERT_UNEXPECTED_SITUATION(entityComponentNode.hasAttribute("z"), "Every RenderQuad has to have z atribute!");
	quad.z = entityComponentNode.getAttribute("z").toUnsignedChar();

	// assign component
	mUsedRegistry->assign_or_replace<component::RenderQuad>(entity, quad);
}

void EntitiesParser::parseTextureRect(const Xml& entityComponentNode, entt::entity& entity)
{
	int left = entityComponentNode.getAttribute("x").toUnsigned();
	int top = entityComponentNode.getAttribute("y").toUnsigned();
	int width = entityComponentNode.getAttribute("width").toUnsigned();
	int height = entityComponentNode.getAttribute("height").toUnsigned();
	IntRect rect(left, top, width, height);
	mUsedRegistry->assign_or_replace<component::TextureRect>(entity, rect);
}

void EntitiesParser::parseBlocksLight(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::BlocksLight>(entity);
}

void EntitiesParser::parsePushingArea(const Xml& entityComponentNode, entt::entity& entity)
{
	float directionX = entityComponentNode.getAttribute("pushForceX").toFloat();
	float directionY = entityComponentNode.getAttribute("pushForceY").toFloat();
	mUsedRegistry->assign_or_replace<component::PushingArea>(entity, sf::Vector2f(directionX, directionY));
}

void EntitiesParser::parseCharacterSpeed(const Xml& entityComponentNode, entt::entity& entity)
{
	float speed = entityComponentNode.getAttribute("speed").toFloat();
	mUsedRegistry->assign_or_replace<component::CharacterSpeed>(entity, speed);
}

void EntitiesParser::parseCollisionWithPlayer(const Xml& entityComponentNode, entt::entity& entity)
{
	float pushForce = entityComponentNode.getAttribute("pushForce").toFloat();
	mUsedRegistry->assign_or_replace<component::CollisionWithPlayer>(entity, pushForce, false);
}

void EntitiesParser::parseVelocity(const Xml& entityComponentNode, entt::entity& entity)
{
	float dx = entityComponentNode.getAttribute("dx").toFloat();
	float dy = entityComponentNode.getAttribute("dy").toFloat();
	mUsedRegistry->assign_or_replace<component::Velocity>(entity, dx, dy);
}

void EntitiesParser::parsePushingVelocity(const Xml& entityComponentNode, entt::entity& entity)
{
	sf::Vector2f vel = entityComponentNode.getAttribute("vel").toVector2f();
	mUsedRegistry->assign_or_replace<component::PushingVelocity>(entity, vel);
}

void EntitiesParser::parseHealth(const Xml& entityComponentNode, entt::entity& entity)
{
	int healthPoints = entityComponentNode.getAttribute("healthPoints").toUnsigned();
	int maxHealthPoints = entityComponentNode.getAttribute("maxHealthPoints").toUnsigned();
	mUsedRegistry->assign_or_replace<component::Health>(entity, healthPoints, maxHealthPoints);
}

void EntitiesParser::parseDamage(const Xml& entityComponentNode, entt::entity& entity)
{
	int damageDealt = entityComponentNode.getAttribute("damageDealt").toUnsigned();
	mUsedRegistry->assign_or_replace<component::Damage>(entity, damageDealt);
}

void EntitiesParser::parseMedkit(const Xml& entityComponentNode, entt::entity& entity)
{
	int addHealthPoints = entityComponentNode.getAttribute("addHealthPoints").toInt();
	mUsedRegistry->assign_or_replace<component::Medkit>(entity, addHealthPoints);
}

void EntitiesParser::parsePlayer(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::Player>(entity);
	mHasLoadedPlayer = true;
}

void EntitiesParser::parseEntrance(const Xml& entityComponentNode, entt::entity& entity)
{
	std::string entranceDestination = entityComponentNode.getAttribute("entranceDestination").toString();
	float posX = entityComponentNode.getAttribute("playerSpawnPositionX").toFloat();
	float posY = entityComponentNode.getAttribute("playerSpawnPositionY").toFloat();
	mUsedRegistry->assign_or_replace<component::Entrance>(entity, entranceDestination, sf::Vector2f(posX, posY));
}

void EntitiesParser::parseGate(const Xml& entityComponentNode, entt::entity& entity)
{
	bool isOpened = entityComponentNode.getAttribute("isOpened").toBool();
	mUsedRegistry->assign_or_replace<component::Gate>(entity, isOpened);
}

void EntitiesParser::parseLever(const Xml& entityComponentNode, entt::entity& entity)
{
	unsigned leverId = entityComponentNode.getAttribute("id").toUnsigned();
	bool isActivated = false;
	bool turnOffAfterSwitch = entityComponentNode.getAttribute("turnOffAfterSwitch").toBool();
	mUsedRegistry->assign_or_replace<component::Lever>(entity, leverId, isActivated, turnOffAfterSwitch);
}

void EntitiesParser::parseLeverListener(const Xml& entityComponentNode, entt::entity& entity)
{
	unsigned observedLeverId = entityComponentNode.getAttribute("observedLeverId").toUnsigned();
	bool isActivated = false;
	mUsedRegistry->assign_or_replace<component::LeverListener>(entity, observedLeverId, isActivated);
}

void EntitiesParser::parseVelocityChangingEffect(const Xml& entityComponentNode, entt::entity& entity)
{
	float velocityMultiplier = entityComponentNode.getAttribute("velocityMultiplier").toFloat();
	mUsedRegistry->assign_or_replace<component::AreaVelocityChangingEffect>(entity, velocityMultiplier);
}

void EntitiesParser::parseKinematicCollisionBody(const Xml& entityComponentNode, entt::entity& entity)
{
	float mass = entityComponentNode.getAttribute("mass").toFloat();
	mUsedRegistry->assign_or_replace<component::KinematicCollisionBody>(entity, mass);
}

void EntitiesParser::parseStaticCollisionBody(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::StaticCollisionBody>(entity);
}

void EntitiesParser::parseMultiStaticCollisionBody(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::MultiStaticCollisionBody>(entity);
}

void EntitiesParser::parseFaceDirection(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::FaceDirection>(entity, sf::Vector2f(0, 0));
}

void EntitiesParser::parseLifetime(const Xml& entityComponentNode, entt::entity& entity)
{
	const float entityLifetime = entityComponentNode.getAttribute("lifetime").toFloat();
	mUsedRegistry->assign_or_replace<component::Lifetime>(entity, entityLifetime);
}

void EntitiesParser::parseParticleEmitter(const Xml& entityComponentNode, entt::entity& entity)
{
	// TODO: Make them be attributes of ParticleNode and not separate nodes

	component::ParticleEmitter emitter;
	auto particleAttribs = entityComponentNode.getChildren("particleAttrib");
	bool wasEndColorAssigned = false;
	bool wasInitialVelocityRandomAssigned = false;
	for(const auto& attrib : particleAttribs)
	{
		const std::string name = attrib.getAttribute("name").toString();
		if(name == "startColor") {
			const auto r = attrib.getAttribute("r").toUnsignedChar();
			const auto g = attrib.getAttribute("g").toUnsignedChar();
			const auto b = attrib.getAttribute("b").toUnsignedChar();
			const auto a = attrib.getAttribute("a").toUnsignedChar();
			if(!wasEndColorAssigned)
				emitter.parEndColor = {r, g, b, a};
			emitter.parStartColor = {r, g, b, a};
		}
		else if(name == "endColor") {
			const auto r = attrib.getAttribute("r").toUnsignedChar();
			const auto g = attrib.getAttribute("g").toUnsignedChar();
			const auto b = attrib.getAttribute("b").toUnsignedChar();
			const auto a = attrib.getAttribute("a").toUnsignedChar();
			emitter.parEndColor = {r, g, b, a};
			wasEndColorAssigned = true;
		}
		else if(name == "texture") {
			const std::string filepath = attrib.getAttribute("filepath").toString();
			if(mTextureHolder->load(filepath))
				emitter.parTexture = &mTextureHolder->get(filepath);
			else
				PH_EXIT_GAME("EntitiesParser::parseParticleEmitter() wasn't able to load texture!");
		}
		else if(name == "spawnPositionOffset") {
			const float x = attrib.getAttribute("x").toFloat();
			const float y = attrib.getAttribute("y").toFloat();
			emitter.spawnPositionOffset = {x, y};
		}
		else if(name == "randomSpawnAreaSize") {
			const float width = attrib.getAttribute("width").toFloat();
			const float height = attrib.getAttribute("height").toFloat();
			emitter.randomSpawnAreaSize = {width, height};
		}
		else if(name == "initialVelocity") {
			const float x = attrib.getAttribute("x").toFloat();
			const float y = attrib.getAttribute("y").toFloat();
			emitter.parInitialVelocity = {x, y};
			if(!wasInitialVelocityRandomAssigned)
				emitter.parInitialVelocityRandom = emitter.parInitialVelocity;
		}
		else if(name == "initialVelocityRandom") {
			const float x = attrib.getAttribute("x").toFloat();
			const float y = attrib.getAttribute("y").toFloat();
			emitter.parInitialVelocityRandom = {x, y};
		}
		else if(name == "acceleration") {
			const float x = attrib.getAttribute("x").toFloat();
			const float y = attrib.getAttribute("y").toFloat();
			emitter.parAcceleration = {x, y};
		}
		else if(name == "size") {
			const float x = attrib.getAttribute("x").toFloat();
			const float y = attrib.getAttribute("y").toFloat();
			emitter.parSize = {x, y};
		}
		else if(name == "amount") {
			emitter.amountOfParticles = attrib.getAttribute("v").toUnsigned();
		}
		else if(name == "lifetime") {
			emitter.parWholeLifetime = attrib.getAttribute("v").toFloat();
		}
		else if(name == "isEmitting") {
			emitter.isEmitting = attrib.getAttribute("v").toBool();
		}
	}
	mUsedRegistry->assign_or_replace<component::ParticleEmitter>(entity, emitter);
}

void EntitiesParser::parseMultiParticleEmitter(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::MultiParticleEmitter>(entity);
}

void EntitiesParser::parseZombie(const Xml& entityComponentNode, entt::entity& entity)
{
	component::Zombie zombie;
	zombie.timeFromLastGrowl = Random::generateNumber(0.f, 2.5f);
	mUsedRegistry->assign_or_replace<component::Zombie>(entity, zombie);
}

void EntitiesParser::parseRenderChunk(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::RenderChunk>(entity);
}

void EntitiesParser::parseArcadeSpawner(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::ArcadeSpawner>(entity);
}

void EntitiesParser::parseLootSpawner(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::LootSpawner>(entity);
}

void EntitiesParser::parseBulletBox(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::BulletBox>(entity);
}

void EntitiesParser::parseGunAttacker(const Xml& entityComponentNode, entt::entity& entity)
{
	component::GunAttacker gunAttacker;
	gunAttacker.isTryingToAttack = entityComponentNode.getAttribute("isTryingToAttack").toBool();
	gunAttacker.timeBeforeHiding = entityComponentNode.getAttribute("timeBeforeHiding").toFloat();
	gunAttacker.timeToHide = 0.f;
	mUsedRegistry->assign_or_replace<component::GunAttacker>(entity, gunAttacker);
}

void EntitiesParser::parseMeleeProperties(const Xml& entityComponentNode, entt::entity& entity)
{
	component::MeleeProperties mp;
	mp.minHitInterval = entityComponentNode.getAttribute("minHitInterval").toFloat();
	mp.rotationSpeed = entityComponentNode.getAttribute("rotationSpeed").toFloat();
	mp.rotationRange = entityComponentNode.getAttribute("rotationRange").toFloat();
	mp.range = entityComponentNode.getAttribute("range").toFloat();
	mp.damage = entityComponentNode.getAttribute("damage").toUnsigned();
	mUsedRegistry->assign_or_replace<component::MeleeProperties>(entity, mp);
}

void EntitiesParser::parseGunProperties(const Xml& entityComponentNode, entt::entity& entity)
{
	component::GunProperties gp;

	gp.shotSoundFilepath = entityComponentNode.getAttribute("shotSoundFilepath").toString();
	gp.range = entityComponentNode.getAttribute("range").toFloat();
	gp.deflectionAngle = entityComponentNode.getAttribute("deflectionAngle").toFloat();
	gp.damage = entityComponentNode.getAttribute("damage").toUnsigned();
	gp.numberOfBullets = entityComponentNode.getAttribute("numberOfBullets").toUnsigned();
	gp.gunId = entityComponentNode.getAttribute("gunId").toUnsigned();
	
	const std::string type = entityComponentNode.getAttribute("type").toString();
	if(type == "pistol")
		gp.type = component::GunProperties::Type::Pistol;
	else if(type == "shotgun")
		gp.type = component::GunProperties::Type::Shotgun;
	else
		PH_UNEXPECTED_SITUATION("Unknown Gun type!");
	
	mUsedRegistry->assign_or_replace<component::GunProperties>(entity, gp);
}

void EntitiesParser::parseCurrentGun(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::CurrentGun>(entity);
}

void EntitiesParser::parseCurrentMeleeWeapon(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::CurrentMeleeWeapon>(entity);
}

void EntitiesParser::parseKillable(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::Killable>(entity);
}

void EntitiesParser::parseBullets(const Xml& entityComponentNode, entt::entity& entity)
{
	component::Bullets bullets;
	bullets.numOfPistolBullets = entityComponentNode.getAttribute("numOfPistolBullets").toUnsigned();
	bullets.numOfShotgunBullets = entityComponentNode.getAttribute("numOfShotgunBullets").toUnsigned();
	mUsedRegistry->assign_or_replace<component::Bullets>(entity, bullets);
}

void EntitiesParser::parseHiddenForRenderer(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::HiddenForRenderer>(entity);
}

void EntitiesParser::parseCamera(const Xml& entityComponentNode, entt::entity& entity)
{
	const float x = entityComponentNode.getAttribute("x").toFloat();
	const float y = entityComponentNode.getAttribute("y").toFloat();
	const float width = entityComponentNode.getAttribute("width").toFloat();
	const float height = entityComponentNode.getAttribute("height").toFloat();
	const unsigned priority = entityComponentNode.getAttribute("priority").toUnsigned();
	mUsedRegistry->assign_or_replace<component::Camera>(entity, Camera({x, y, width, height}), priority);
}

void EntitiesParser::parseLightSource(const Xml& entityComponentNode, entt::entity& entity)
{
	component::LightSource lightSource;
	lightSource.offset = {
		entityComponentNode.getAttribute("offsetX").toFloat(),
		entityComponentNode.getAttribute("offsetY").toFloat()
	};
	lightSource.color = entityComponentNode.getAttribute("color").toColor();
	lightSource.startAngle = entityComponentNode.hasAttribute("startAngle") ? entityComponentNode.getAttribute("startAngle").toFloat() : 0.f;
	lightSource.endAngle = entityComponentNode.hasAttribute("endAngle") ? entityComponentNode.getAttribute("endAngle").toFloat() : 360.f;
	lightSource.attenuationAddition = entityComponentNode.getAttribute("attenuationAddition").toFloat();
	lightSource.attenuationFactor = entityComponentNode.getAttribute("attenuationFactor").toFloat();
	lightSource.attenuationSquareFactor = entityComponentNode.getAttribute("attenuationSquareFactor").toFloat();
	mUsedRegistry->assign_or_replace<component::LightSource>(entity, lightSource);
}

void EntitiesParser::parseAnimationData(const Xml& entityComponentNode, entt::entity& entity)
{
	component::AnimationData animationData;

	const std::string animationStateFilepath = entityComponentNode.getAttribute("animationStatesFile").toString();
	loadAnimationStatesFromFile(animationStateFilepath);
	animationData.states = getAnimationStates(animationStateFilepath);
	
	animationData.currentStateName = entityComponentNode.getAttribute("firstStateName").toString();
	
	animationData.delay = entityComponentNode.getAttribute("delay").toFloat();
	
	mUsedRegistry->assign_or_replace<component::AnimationData>(entity, animationData);
}

}
