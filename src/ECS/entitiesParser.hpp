#pragma once

#include "entt/entity/registry.hpp"

namespace ph {

class Xml;
class EntitiesTemplateStorage;

class EntitiesParser
{
public:
	EntitiesParser();

	void parseFile(const std::string& filePath, EntitiesTemplateStorage& templateStorage, entt::registry& usedRegistry);

private:
	void parseTemplates(const Xml& entityTemplatesNode);
	void parseEntities(const Xml& entitiesNode);
	void parseComponents(std::vector<Xml>& entityComponents, entt::entity& entity);

	void parseVelocity(const Xml& entityComponentNode, entt::entity& entity);
	void parseHealth(const Xml& entityComponentNode, entt::entity& entity);
	void parseMedkit(const Xml& entityComponentNode, entt::entity& entity);
	void parseKinematicCollisionBody(const Xml& entityComponentNode, entt::entity& entity);
	void parseStaticCollisionBody(const Xml& entityComponentNode, entt::entity& entity);
	void parseGunAttacker(const Xml& entityComponentNode, entt::entity& entity);
	void parseMeleeAttacker(const Xml& entityComponentNode, entt::entity& entity);
	void parsePlayer(const Xml& entityComponentNode, entt::entity& entity);
	void parseBullet(const Xml& entityComponentNode, entt::entity& entity);
	void parseShader(const Xml& entityComponentNode, entt::entity& entity);
	void parseAnimation(const Xml& entityComponentNode, entt::entity& entity);
	void parseSpawner(const Xml& entityComponentNode, entt::entity& entity);
	void parseVertexArray(const Xml& entityComponentNode, entt::entity& entity);

	//NOTE: above methods are awkward but as long as we don't find a better solution that is the way	

private:
	EntitiesTemplateStorage* mTemplateStorage;
	entt::registry* mGameRegistry;
};

}

