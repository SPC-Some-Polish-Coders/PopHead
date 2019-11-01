#include "ECS/entitiesParser.hpp"
#include "Utilities/xml.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"

namespace ph {

EntitiesParser::EntitiesParser()
{
}

void EntitiesParser::parseFile(const std::string& filePath, entt::registry& templatesRegistry, entt::registry& gameRegistry)
{
	Xml entitiesFile;
	entitiesFile.loadFromFile(filePath);
	const Xml entityTemplatesNode = entitiesFile.getChild("entityTemplates");
	loadEntityTemplates(entityTemplatesNode);
}

entt::entity EntitiesParser::getTemplate(const std::string& templateName)
{
	return mTemplatesMap.at(templateName);
}

entt::registry& EntitiesParser::getTemplateRegistry()
{
	return mTemplatesRegistry;
}

void EntitiesParser::loadEntityTemplates(const Xml& entityTemplatesNode)
{
	std::vector<Xml> entityTemplates = entityTemplatesNode.getChildren("entityTemplate");
	for (auto& entityTemplate : entityTemplates)
	{
		auto entity = mTemplatesRegistry.create();
		if (entityTemplate.hasAttribute("sourceTemplate"))
		{
			const std::string& sourceTemplateName = entityTemplate.getAttribute("sourceTemplate").toString();
			mTemplatesRegistry.stomp(entity, getTemplate(sourceTemplateName), mTemplatesRegistry);
		}

		std::vector<Xml> entityComponents = entityTemplate.getChildren("component");
		parseComponents(entityComponents, entity);

		const std::string& templateName = entityTemplate.getAttribute("name").toString();
		mTemplatesMap.insert(std::make_pair(templateName, entity));
	}
}

void EntitiesParser::parseComponents(std::vector<Xml>& entityComponents, entt::entity& entity)
{
	if (entityComponents.empty())
		return;

	std::unordered_map<std::string, void(EntitiesParser::*)(const Xml&, entt::entity&)> mComponentsMap = {
		{"Velocity",	&EntitiesParser::parseVelocity},
		{"Health",		&EntitiesParser::parseHealth},
		{"Medkit",		&EntitiesParser::parseMedkit},
		{"Player",		&EntitiesParser::parsePlayer},
		{"Animation",	&EntitiesParser::parseAnimation},
		{"Spawner",		&EntitiesParser::parseSpawner},
		{"Shader",		&EntitiesParser::parseShader},
		{"VertexArray",	&EntitiesParser::parseVertexArray}
	};

	for (auto& entityComponent : entityComponents)
	{
		const std::string& componentName = entityComponent.getAttribute("name").toString();
		(this->*mComponentsMap.at(componentName))(entityComponent, entity);
	}
}

//NOTE: We get a little time penalty from using assign_or_replace. However we need it for templates that base on other templates
//		so it's probably the best option

void EntitiesParser::parseVelocity(const Xml& entityComponentNode, entt::entity& entity)
{
	float velocityX = entityComponentNode.getAttribute("velocityX").toFloat();
	float velocityY = entityComponentNode.getAttribute("velocityY").toFloat();
	mTemplatesRegistry.assign_or_replace<component::Velocity>(entity, velocityX, velocityY);
}

void EntitiesParser::parseHealth(const Xml& entityComponentNode, entt::entity& entity)
{
	int healthPoints = entityComponentNode.getAttribute("healthPoints").toInt();
	int maxHealthPoints = entityComponentNode.getAttribute("maxHealthPoints").toInt();
	mTemplatesRegistry.assign_or_replace<component::Health>(entity, healthPoints, maxHealthPoints);
}

void EntitiesParser::parseMedkit(const Xml& entityComponentNode, entt::entity& entity)
{
	int addHealthPoints = entityComponentNode.getAttribute("addHealthPoints").toInt();
	mTemplatesRegistry.assign_or_replace<component::Medkit>(entity, addHealthPoints);
}

void EntitiesParser::parsePlayer(const Xml& entityComponentNode, entt::entity& entity)
{
	mTemplatesRegistry.assign_or_replace<component::Player>(entity);
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
