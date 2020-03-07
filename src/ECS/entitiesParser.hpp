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

	void parseBodyRect(const Xml& entityComponentNode, entt::entity& entity);
	void parseRenderQuad(const Xml& entityComponentNode, entt::entity& entity);
	void parseTextureRect(const Xml& entityComponentNode, entt::entity& entity);
	void parseLightWall(const Xml& entityComponentNode, entt::entity& entity);
	void parsePushingArea(const Xml& entityComponentNode, entt::entity& entity);
	void parseHint(const Xml& entityComponentNode, entt::entity& entity);
	void parseCharacterSpeed(const Xml& entityComponentNode, entt::entity& entity);
	void parseCollisionWithPlayer(const Xml& entityComponentNode, entt::entity& entity);
	void parseVelocity(const Xml& entityComponentNode, entt::entity& entity);
	void parsePushingForces(const Xml& entityComponentNode, entt::entity& entity);
	void parseHealth(const Xml& entityComponentNode, entt::entity& entity);
	void parseDamage(const Xml& entityComponentNode, entt::entity& entity);
	void parseMedkit(const Xml& entityComponentNode, entt::entity& entity);
	void parseKinematicCollisionBody(const Xml& entityComponentNode, entt::entity& entity);
	void parseStaticCollisionBody(const Xml& entityComponentNode, entt::entity& entity);
	void parseMultiStaticCollisionBody(const Xml& entityComponentNode, entt::entity& entity);
	void parseGunAttacker(const Xml& entityComponentNode, entt::entity& entity);
	void parseMeleeProperties(const Xml& entityComponentNode, entt::entity& entity);
	void parseGunProperties(const Xml& entityComponentNode, entt::entity& entity);
	void parseFaceDirection(const Xml& entityComponentNode, entt::entity& entity);
	void parsePlayer(const Xml& entityComponentNode, entt::entity& entity);
	void parseEntrance(const Xml& entityComponentNode, entt::entity& entity);
	void parseGate(const Xml& entityComponentNode, entt::entity& entity);
	void parseLever(const Xml& entityComponentNode, entt::entity& entity);
	void parseLeverListener(const Xml& entityComponentNode, entt::entity& entity);
	void parseVelocityChangingEffect(const Xml& entityComponentNode, entt::entity& entity);
	void parseBullets(const Xml& entityComponentNode, entt::entity& entity);
	void parseCurrentGun(const Xml& entityComponentNode, entt::entity& entity);
	void parseCurrentMeleeWeapon(const Xml& entityComponentNode, entt::entity& entity);
	void parseHiddenForRenderer(const Xml& entityComponentNode, entt::entity& entity);
	void parseKillable(const Xml& entityComponentNode, entt::entity& entity);
	void parseCamera(const Xml& entityComponentNode, entt::entity& entity);
	void parseLightSource(const Xml& entityComponentNode, entt::entity& entity);
	void parseAnimationData(const Xml& entityComponentNode, entt::entity& entity);
	void parseLifetime(const Xml& entityComponentNode, entt::entity& entity);
	void parseParticleEmitter(const Xml& entityComponentNode, entt::entity& entity);
	void parseMultiParticleEmitter(const Xml& entityComponentNode, entt::entity& entity);
	void parseZombie(const Xml& entityComponentNode, entt::entity& entity);
	void parseRenderChunk(const Xml& entityComponentNode, entt::entity& entity);
	void parseArcadeSpawner(const Xml& entityComponentNode, entt::entity& entity);
	void parseLootSpawner(const Xml& entityComponentNode, entt::entity& entity);
	void parseBulletBox(const Xml& entityComponentNode, entt::entity& entity);
	void parseCar(const Xml& entityComponentNode, entt::entity& entity);
	void parseCutScene(const Xml& entityComponentNode, entt::entity& entity);

private:
	EntitiesTemplateStorage* mTemplateStorage;
	entt::registry* mUsedRegistry;
};

}
