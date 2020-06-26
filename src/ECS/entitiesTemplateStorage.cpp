#include "pch.hpp"
#include "entitiesTemplateStorage.hpp"
#include "ECS/Components/debugComponents.hpp"

namespace ph {

entt::registry& EntitiesTemplateStorage::getTemplateRegistry()
{
	return mTemplatesRegistry;
}

entt::entity EntitiesTemplateStorage::create(const std::string& templateName)
{
	auto newTemplateEntity = mTemplatesRegistry.create();
	mTemplatesMap.insert(std::make_pair(templateName, newTemplateEntity));

	#ifndef PH_DISTRIBUTION
	auto& debugName = mTemplatesRegistry.assign<component::DebugName>(newTemplateEntity);
	memcpy(debugName.name, templateName.c_str(), templateName.length()); 
	#endif

	return newTemplateEntity;
}

entt::entity EntitiesTemplateStorage::createCopy(const std::string& templateName, entt::registry& gameRegistry)
{
	return gameRegistry.create(getTemplate(templateName), mTemplatesRegistry);
}

entt::entity EntitiesTemplateStorage::getTemplate(const std::string& templateName)
{
	return mTemplatesMap.at(templateName);
}

void EntitiesTemplateStorage::clearStorage()
{
	mTemplatesMap.clear();
	mTemplatesRegistry.reset();
}

void EntitiesTemplateStorage::stomp(const entt::entity dst, const std::string& templateName)
{
	mTemplatesRegistry.stomp(dst, getTemplate(templateName), mTemplatesRegistry);
}

void EntitiesTemplateStorage::stomp(const entt::entity dst, const std::string& templateName, entt::registry& gameRegistry)
{
	gameRegistry.stomp(dst, getTemplate(templateName), mTemplatesRegistry);
}

}
