#pragma once

#include "entt/entity/registry.hpp"

namespace ph {

class EntitiesTemplateStorage {
public:
	EntitiesTemplateStorage();

	entt::entity getTemplate(const std::string& templateName);

	entt::entity create(const std::string& templateName);
	entt::entity createCopy(const std::string& templateName, entt::registry& gameRegistry);
	void stomp(const entt::entity dstEntity, const std::string& templateName);

	template<typename T, typename... Args>
	void assign(const std::string& templateName, Args&&... arguments);
	template<typename T, typename... Args>
	void assign(entt::entity& templateEntity, Args&&... arguments);

	template<typename T, typename... Args>
	void assign_or_replace(const std::string& templateName, Args&&... arguments);
	template<typename T, typename... Args>
	void assign_or_replace(entt::entity& templateEntity, Args&&... arguments);

	template<typename T>
	bool has(const std::string& templateName);
	template<typename T>
	bool has(entt::entity& templateEntity);

	template<typename T>
	T get(const std::string& templateName);
	template<typename T>
	T get(entt::entity& templateEntity);

private:
	std::unordered_map<std::string, entt::entity> mTemplatesMap;
	entt::registry mTemplatesRegistry;
};

}

#include "ECS/entitiesTemplateStorage.inl"
