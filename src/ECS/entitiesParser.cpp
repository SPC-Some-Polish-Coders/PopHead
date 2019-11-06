#include "ECS/entitiesParser.hpp"
#include "Utilities/xml.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "ECS/entitiesTemplateStorage.hpp"
#include "Renderer/Shaders/shaderLibary.hpp"

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
		{"CharacterSpeed",		   &EntitiesParser::parseCharacterSpeed},
		{"Health",	               &EntitiesParser::parseHealth},
		{"Damage",	               &EntitiesParser::parseDamage},
		{"Medkit",	               &EntitiesParser::parseMedkit},
		{"Player",                 &EntitiesParser::parsePlayer},
		{"Bullet",                 &EntitiesParser::parseBullet},
		{"Spawner",                &EntitiesParser::parseSpawner},
		{"Velocity",               &EntitiesParser::parseVelocity},
		{"Texture",                &EntitiesParser::parseTexture},
		{"TextureRect",            &EntitiesParser::parseTextureRect},
		{"Shader",                 &EntitiesParser::parseShader},
		{"Color",                  &EntitiesParser::parseColor},
		{"Rotation",               &EntitiesParser::parseRotation},
		{"Animation",              &EntitiesParser::parseAnimation},
		{"GunAttacker",            &EntitiesParser::parseGunAttacker},
		{"VertexArray",            &EntitiesParser::parseVertexArray},
		{"MeleeAttacker",          &EntitiesParser::parseMeleeAttacker},
		{"StaticCollisionBody",    &EntitiesParser::parseStaticCollisionBody},
		{"KinematicCollisionBody", &EntitiesParser::parseKinematicCollisionBody}
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

void EntitiesParser::parseCharacterSpeed(const Xml& entityComponentNode, entt::entity& entity)
{
	float speed = entityComponentNode.getAttribute("speed").toFloat();
	mUsedRegistry->assign_or_replace<component::CharacterSpeed>(entity, speed);
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

void EntitiesParser::parseKinematicCollisionBody(const Xml& entityComponentNode, entt::entity& entity)
{
	float mass = entityComponentNode.getAttribute("mass").toFloat();
	mUsedRegistry->assign_or_replace<component::KinematicCollisionBody>(entity);
}

void EntitiesParser::parseStaticCollisionBody(const Xml& entityComponentNode, entt::entity& entity)
{
	mUsedRegistry->assign_or_replace<component::StaticCollisionBody>(entity);
}

void EntitiesParser::parseGunAttacker(const Xml& entityComponentNode, entt::entity& entity)
{
	float minSecondsInterval = entityComponentNode.getAttribute("minSecondsInterval").toFloat();
	unsigned bullets = entityComponentNode.getAttribute("bullets").toUnsigned();
	bool isTryingToAttack = entityComponentNode.getAttribute("isTryingToAttack").toBool();
	mUsedRegistry->assign_or_replace<component::GunAttacker>(entity, minSecondsInterval, bullets, isTryingToAttack);
}

void EntitiesParser::parseMeleeAttacker(const Xml& entityComponentNode, entt::entity& entity)
{
	float minSecondsInterval = entityComponentNode.getAttribute("minSecondsInterval").toFloat();
	bool isTryingToAttack = entityComponentNode.getAttribute("isTryingToAttack").toBool();
	mUsedRegistry->assign_or_replace<component::MeleeAttacker>(entity, minSecondsInterval, isTryingToAttack);
}

void EntitiesParser::parseBullet(const Xml& entityComponentNode, entt::entity& entity)
{
	int numOfBullets = entityComponentNode.getAttribute("numOfBullets").toInt();
	mUsedRegistry->assign_or_replace<component::Bullet>(entity, numOfBullets);
}

void EntitiesParser::parseTexture(const Xml& entityComponentNode, entt::entity& entity)
{
	const std::string filepath = entityComponentNode.getAttribute("filepath").toString();
	if(mTextureHolder->load(filepath))
	{
		auto& texture = mTextureHolder->get(filepath);
		mUsedRegistry->assign_or_replace<component::TexturePtr>(entity, &texture);
	}
	else
		PH_EXIT_GAME("EntitiesParser::parseTexture() wasn't able to load texture!");
}

void EntitiesParser::parseTextureRect(const Xml& entityComponentNode, entt::entity& entity)
{
	const int x = entityComponentNode.getAttribute("x").toInt();
	const int y = entityComponentNode.getAttribute("y").toInt();
	const int width = entityComponentNode.getAttribute("width").toInt();
	const int height = entityComponentNode.getAttribute("height").toInt();
	mUsedRegistry->assign_or_replace<component::TextureRect>(entity, IntRect(x, y, width, height));
}

void EntitiesParser::parseColor(const Xml& entityComponentNode, entt::entity& entity)
{
	const auto r = entityComponentNode.getAttribute("r").toUnsignedChar();
	const auto g = entityComponentNode.getAttribute("g").toUnsignedChar();
	const auto b = entityComponentNode.getAttribute("b").toUnsignedChar();
	const auto a = entityComponentNode.getAttribute("a").toUnsignedChar();
	mUsedRegistry->assign_or_replace<component::Color>(entity, sf::Color(r, g, b, a));
}

void EntitiesParser::parseRotation(const Xml& entityComponentNode, entt::entity& entity)
{
	const float angle = entityComponentNode.getAttribute("angle").toFloat();
	mUsedRegistry->assign_or_replace<component::Rotation>(entity, angle);
}

void EntitiesParser::parseShader(const Xml& entityComponentNode, entt::entity& entity)
{
	const std::string shaderName = entityComponentNode.getAttribute("shaderName").toString();
	const std::string vertexShaderFilepath = entityComponentNode.getAttribute("vertexShaderFilepath").toString();
	const std::string fragmentShaderFilepath = entityComponentNode.getAttribute("fragmentShaderFilepath").toString();

	auto& sl = ShaderLibrary::getInstance();
	if(sl.loadFromFile(shaderName, vertexShaderFilepath.c_str(), fragmentShaderFilepath.c_str())) {
		auto* shader = sl.get(shaderName);
		mUsedRegistry->assign_or_replace<component::ShaderPtr>(entity, shader);
	}
	else
		PH_EXIT_GAME("EntitiesParser::parseShader() wasn't able to load shader!");
}

void EntitiesParser::parseAnimation(const Xml& entityComponentNode, entt::entity& entity)
{

}

void EntitiesParser::parseSpawner(const Xml& entityComponentNode, entt::entity& entity)
{

}

void EntitiesParser::parseVertexArray(const Xml& entityComponentNode, entt::entity& entity)
{

}

}

