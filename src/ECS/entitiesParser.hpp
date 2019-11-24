#pragma once

#include "entt/entity/registry.hpp"
#include "Resources/resourceHolder.hpp"

namespace ph {

class Xml;
class EntitiesTemplateStorage;

class EntitiesParser
{
public:
	EntitiesParser();

	void parseFile(const std::string& filePath, EntitiesTemplateStorage& templateStorage, entt::registry& usedRegistry, TextureHolder&);

private:
	void parseTemplates(const Xml& entityTemplatesNode);
	void parseEntities(const Xml& entitiesNode);
	void parseComponents(std::vector<Xml>& entityComponents, entt::entity& entity);

	void parseBodyRect(const Xml& entityComponentNode, entt::entity& entity);
	void parseCharacterSpeed(const Xml& entityComponentNode, entt::entity& entity);
	void parseCollisionWithPlayer(const Xml& entityComponentNode, entt::entity& entity);
	void parseVelocity(const Xml& entityComponentNode, entt::entity& entity);
	void parseHealth(const Xml& entityComponentNode, entt::entity& entity);
	void parseDamage(const Xml& entityComponentNode, entt::entity& entity);
	void parseMedkit(const Xml& entityComponentNode, entt::entity& entity);
	void parseKinematicCollisionBody(const Xml& entityComponentNode, entt::entity& entity);
	void parseStaticCollisionBody(const Xml& entityComponentNode, entt::entity& entity);
	void parseGunAttacker(const Xml& entityComponentNode, entt::entity& entity);
	void parseFaceDirection(const Xml& entityComponentNode, entt::entity& entity);
	void parseMeleeAttacker(const Xml& entityComponentNode, entt::entity& entity);
	void parsePlayer(const Xml& entityComponentNode, entt::entity& entity);
	void parseBullet(const Xml& entityComponentNode, entt::entity& entity);
	void parseTexture(const Xml& entityComponentNode, entt::entity& entity);
	void parsePlayerGun(const Xml& entityComponentNode, entt::entity& entity);
	void parseTextureRect(const Xml& entityComponentNode, entt::entity& entity);
	void parseHiddenForRenderer(const Xml& entityComponentNode, entt::entity& entity);
	void parseKillable(const Xml& entityComponentNode, entt::entity& entity);
	void parseShader(const Xml& entityComponentNode, entt::entity& entity);
	void parseColor(const Xml& entityComponentNode, entt::entity& entity);
	void parseZ(const Xml& entityComponentNode, entt::entity& entity);
	void parseRotation(const Xml& entityComponentNode, entt::entity& entity);
	void parseCamera(const Xml& entityComponentNode, entt::entity& entity);
	void parseAnimationData(const Xml& entityComponentNode, entt::entity& entity);
	void parseLifetime(const Xml& entityComponentNode, entt::entity& entity);
	
private:
	EntitiesTemplateStorage* mTemplateStorage;
	entt::registry* mUsedRegistry;
	TextureHolder* mTextureHolder;
};

}

