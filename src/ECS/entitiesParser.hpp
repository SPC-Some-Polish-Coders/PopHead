#pragma once

#include "entt/entity/registry.hpp"

namespace ph {

class Xml;

class EntitiesParser
{
public:
	EntitiesParser();

	void parseFile(const std::string& str);
	entt::entity getTemplate(const std::string& templateName);
	entt::registry& getTemplateRegistry();

private:
	void loadEntityTemplates(const Xml& entityTemplatesNode);
	void parseComponents(std::vector<Xml>& entityComponents, entt::entity& entity);

	void parseVelocity(const Xml& entityComponentNode, entt::entity& entity);
	void parseHealth(const Xml& entityComponentNode, entt::entity& entity);
	void parseMedkit(const Xml& entityComponentNode, entt::entity& entity);

	//NOTE: Three above methods are awkward but as long as we don't find a better solution that is the way	

private:
	entt::registry mTemplatesRegistry;
	std::unordered_map<std::string, entt::entity> mTemplatesMap;
};

}