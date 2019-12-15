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
#include "Renderer/Shaders/shaderLibary.hpp"
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

	mUsedRegistry = &gameRegistry;
	const Xml entitiesNode = entitiesFile.getChild("entities");
	parseEntities(entitiesNode);

	mTemplateStorage = nullptr;
	mUsedRegistry = nullptr;
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
		{"BodyRect",			   &EntitiesParser::parseBodyRect},
		{"RenderQuad",			   &EntitiesParser::parseRenderQuad},
		{"TextureRect",			   &EntitiesParser::parseTextureRect},
		{"Area",				   &EntitiesParser::parseArea},
		{"CharacterSpeed",		   &EntitiesParser::parseCharacterSpeed},
		{"Killable",			   &EntitiesParser::parseKillable},
		{"Health",	               &EntitiesParser::parseHealth},
		{"Damage",	               &EntitiesParser::parseDamage},
		{"Medkit",	               &EntitiesParser::parseMedkit},
		{"Player",                 &EntitiesParser::parsePlayer},
		{"Zombie",                 &EntitiesParser::parseZombie},
		{"Bullet",                 &EntitiesParser::parseBullet},
		{"Velocity",               &EntitiesParser::parseVelocity},
		{"Entrance",               &EntitiesParser::parseEntrance},
		{"Lever",				   &EntitiesParser::parseLever},
		{"CurrentGun",             &EntitiesParser::parseCurrentGun},
		{"CurrentMeleeWeapon",     &EntitiesParser::parseCurrentMeleeWeapon},
		{"FaceDirection",          &EntitiesParser::parseFaceDirection},
		{"Lifetime",			   &EntitiesParser::parseLifetime},
		{"Camera",                 &EntitiesParser::parseCamera},
		{"PointLight",             &EntitiesParser::parsePointLight},
		{"HiddenForRenderer",	   &EntitiesParser::parseHiddenForRenderer},
		{"GunAttacker",            &EntitiesParser::parseGunAttacker},
		{"MeleeAttacker",          &EntitiesParser::parseMeleeAttacker},
		{"CollisionWithPlayer",    &EntitiesParser::parseCollisionWithPlayer},
		{"StaticCollisionBody",    &EntitiesParser::parseStaticCollisionBody},
		{"KinematicCollisionBody", &EntitiesParser::parseKinematicCollisionBody},
		{"VelocityChangingEffect", &EntitiesParser::parseVelocityChangingEffect},
		{"AnimationData",          &EntitiesParser::parseAnimationData},
		{"ParticleEmitter",        &EntitiesParser::parseParticleEmitter},
		{"MultiParticleEmitter",   &EntitiesParser::parseMultiParticleEmitter}
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

	// parse z
	PH_ASSERT_UNEXPECTED_SITUATION(entityComponentNode.hasAttribute("z"), "Every RenderQuad has to have z atribute!");
	quad.z = entityComponentNode.getAttribute("z").toChar();

	// parse blocks light
	PH_ASSERT_UNEXPECTED_SITUATION(entityComponentNode.hasAttribute("blocksLight"), "Every RenderQuad has to have blockLight atribute!");
	quad.blocksLight = entityComponentNode.getAttribute("blocksLight").toBool();

	// assign component
	mUsedRegistry->assign_or_replace<component::RenderQuad>(entity, quad);
}

void EntitiesParser::parseTextureRect(const Xml& entityComponentNode, entt::entity& entity)
{
	int left = entityComponentNode.getAttribute("x").toInt();
	int top = entityComponentNode.getAttribute("y").toInt();
	int width = entityComponentNode.getAttribute("width").toInt();
	int height = entityComponentNode.getAttribute("height").toInt();
	IntRect rect(left, top, width, height);
	mUsedRegistry->assign_or_replace<component::TextureRect>(entity, rect);
}

void EntitiesParser::parseArea(const Xml& entityComponentNode, entt::entity& entity)
{
	float x = entityComponentNode.getAttribute("x").toFloat();
	float y = entityComponentNode.getAttribute("y").toFloat();
	float width = entityComponentNode.getAttribute("width").toFloat();
	float height = entityComponentNode.getAttribute("height").toFloat();
	mUsedRegistry->assign_or_replace<component::Area>(entity, ph::FloatRect(x, y, width, height));
}

void EntitiesParser::parseCharacterSpeed(const Xml& entityComponentNode, entt::entity& entity)
{
	float speed = entityComponentNode.getAttribute("speed").toFloat();
	mUsedRegistry->assign_or_replace<component::CharacterSpeed>(entity, speed);
}

void EntitiesParser::parseCollisionWithPlayer(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::CollisionWithPlayer>(entity, false);
}

void EntitiesParser::parseVelocity(const Xml& entityComponentNode, entt::entity& entity)
{
	float dx = entityComponentNode.getAttribute("dx").toFloat();
	float dy = entityComponentNode.getAttribute("dy").toFloat();
	mUsedRegistry->assign_or_replace<component::Velocity>(entity, dx, dy);
}

void EntitiesParser::parseHealth(const Xml& entityComponentNode, entt::entity& entity)
{
	int healthPoints = entityComponentNode.getAttribute("healthPoints").toInt();
	int maxHealthPoints = entityComponentNode.getAttribute("maxHealthPoints").toInt();
	mUsedRegistry->assign_or_replace<component::Health>(entity, healthPoints, maxHealthPoints);
}

void EntitiesParser::parseDamage(const Xml& entityComponentNode, entt::entity& entity)
{
	int damageDealt = entityComponentNode.getAttribute("damageDealt").toInt();
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
}

void EntitiesParser::parseEntrance(const Xml& entityComponentNode, entt::entity& entity)
{
	std::string entranceDestination = entityComponentNode.getAttribute("entranceDestination").toString();
	float posX = entityComponentNode.getAttribute("playerSpawnPositionX").toFloat();
	float posY = entityComponentNode.getAttribute("playerSpawnPositionY").toFloat();
	mUsedRegistry->assign_or_replace<component::Entrance>(entity, entranceDestination);
}

void EntitiesParser::parseLever(const Xml& entityComponentNode, entt::entity& entity)
{
	bool isActivated = entityComponentNode.getAttribute("isActivated").toBool();
	float activationCooldown = entityComponentNode.getAttribute("minActivationInterval").toFloat();
	mUsedRegistry->assign_or_replace<component::Lever>(entity, isActivated, activationCooldown);
}

void EntitiesParser::parseVelocityChangingEffect(const Xml& entityComponentNode, entt::entity& entity)
{
	float velocityMultiplier = entityComponentNode.getAttribute("velocityMultiplier").toFloat();
	mUsedRegistry->assign_or_replace<component::AreaVelocityChangingEffect>(entity, velocityMultiplier);
}

void EntitiesParser::parseKinematicCollisionBody(const Xml& entityComponentNode, entt::entity& entity)
{
	float mass = entityComponentNode.getAttribute("mass").toFloat();
	mUsedRegistry->assign_or_replace<component::KinematicCollisionBody>(entity);
}

void EntitiesParser::parseStaticCollisionBody(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::StaticCollisionBody>(entity);
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

void EntitiesParser::parseGunAttacker(const Xml& entityComponentNode, entt::entity& entity)
{
	float minSecondsInterval = entityComponentNode.getAttribute("minSecondsInterval").toFloat();
	unsigned bullets = entityComponentNode.getAttribute("bullets").toUnsigned();
	bool isTryingToAttack = entityComponentNode.getAttribute("isTryingToAttack").toBool();
	const float cooldown = 0.f;
	bool canAttack = true;
	mUsedRegistry->assign_or_replace<component::GunAttacker>(entity, minSecondsInterval, cooldown, bullets, isTryingToAttack, canAttack);
}

void EntitiesParser::parseCurrentGun(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::CurrentGun>(entity, 2.f, 0.f);
}

void EntitiesParser::parseCurrentMeleeWeapon(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::CurrentMeleeWeapon>(entity);
}

void EntitiesParser::parseMeleeAttacker(const Xml& entityComponentNode, entt::entity& entity)
{
	float minSecondsInterval = entityComponentNode.getAttribute("minSecondsInterval").toFloat();
	bool isTryingToAttack = entityComponentNode.getAttribute("isTryingToAttack").toBool();
	const float cooldown = 0.f;
	mUsedRegistry->assign_or_replace<component::MeleeAttacker>(entity, minSecondsInterval, cooldown, isTryingToAttack);
}

void EntitiesParser::parseKillable(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::Killable>(entity);
}

void EntitiesParser::parseBullet(const Xml& entityComponentNode, entt::entity& entity)
{
	int numOfBullets = entityComponentNode.getAttribute("numOfBullets").toInt();
	mUsedRegistry->assign_or_replace<component::Bullet>(entity, numOfBullets);
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

void EntitiesParser::parsePointLight(const Xml& entityComponentNode, entt::entity& entity)
{
	component::PointLight pointLight;
	pointLight.offset = {
		entityComponentNode.getAttribute("offsetX").toFloat(),
		entityComponentNode.getAttribute("offsetY").toFloat()
	};
	pointLight.color = {
		entityComponentNode.getAttribute("r").toUnsignedChar(),
		entityComponentNode.getAttribute("g").toUnsignedChar(),
		entityComponentNode.getAttribute("b").toUnsignedChar()
	};
	pointLight.range = entityComponentNode.getAttribute("range").toFloat();
	mUsedRegistry->assign_or_replace<component::PointLight>(entity, pointLight);
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
