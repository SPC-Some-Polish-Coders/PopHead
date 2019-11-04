#include "ECS/entitiesParser.hpp"
#include "Utilities/xml.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "ECS/entitiesTemplateStorage.hpp"

namespace ph {

EntitiesParser::EntitiesParser()
	:mTemplateStorage(nullptr)
	,mUsedRegistry(nullptr)
{
}

void EntitiesParser::parseFile(const std::string& filePath, EntitiesTemplateStorage& templateStorage, entt::registry& gameRegistry)
{
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
		{"CharacterSpeed",		   &EntitiesParser::parseCharacterSpeed},
		{"Shader",                 &EntitiesParser::parseShader},
		{"Health",	               &EntitiesParser::parseHealth},
		{"Medkit",	               &EntitiesParser::parseMedkit},
		{"Player",                 &EntitiesParser::parsePlayer},
		{"Bullet",                 &EntitiesParser::parseBullet},
		{"Spawner",                &EntitiesParser::parseSpawner},
		{"Position",               &EntitiesParser::parsePosition},
		{"Size",                   &EntitiesParser::parseSize},
		{"Velocity",               &EntitiesParser::parseVelocity},
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

void EntitiesParser::parsePosition(const Xml& entityComponentNode, entt::entity& entity)
{
	float x = entityComponentNode.getAttribute("x").toFloat();
	float y = entityComponentNode.getAttribute("y").toFloat();
	mUsedRegistry->assign_or_replace<component::Position>(entity, x, y);
}

void EntitiesParser::parseSize(const Xml& entityComponentNode, entt::entity& entity)
{
	float width = entityComponentNode.getAttribute("width").toFloat();
	float height = entityComponentNode.getAttribute("height").toFloat();
	mUsedRegistry->assign_or_replace<component::Size>(entity, width, height);
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

void EntitiesParser::parseShader(const Xml& entityComponentNode, entt::entity& entity)
{

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

