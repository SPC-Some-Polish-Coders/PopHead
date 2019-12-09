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
	void parseRenderQuad(const Xml& entityComponentNode, entt::entity& entity);
	void parseTextureRect(const Xml& entityComponentNode, entt::entity& entity);
	void parseArea(const Xml& entityComponentNode, entt::entity& entity);
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
	void parseEntrance(const Xml& entityComponentNode, entt::entity& entity);
	void parseVelocityChangingEffect(const Xml& entityComponentNode, entt::entity& entity);
	void parseBullet(const Xml& entityComponentNode, entt::entity& entity);
	void parsePlayerGun(const Xml& entityComponentNode, entt::entity& entity);
	void parseHiddenForRenderer(const Xml& entityComponentNode, entt::entity& entity);
	void parseKillable(const Xml& entityComponentNode, entt::entity& entity);
	void parseCamera(const Xml& entityComponentNode, entt::entity& entity);
	void parsePointLight(const Xml& entityComponentNode, entt::entity& entity);
	void parseAnimationData(const Xml& entityComponentNode, entt::entity& entity);
	void parseLifetime(const Xml& entityComponentNode, entt::entity& entity);
	void parseParticleEmitter(const Xml& entityComponentNode, entt::entity& entity);
	void parseMultiParticleEmitter(const Xml& entityComponentNode, entt::entity& entity);
	void parseZombie(const Xml& entityComponentNode, entt::entity& entity);

private:
	EntitiesTemplateStorage* mTemplateStorage;
	entt::registry* mUsedRegistry;
	TextureHolder* mTextureHolder;
};

}
