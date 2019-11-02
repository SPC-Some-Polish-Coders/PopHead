#include "entitiesTemplateStorage.hpp"

namespace ph {

EntitiesTemplateStorage::EntitiesTemplateStorage()
{
}

entt::entity EntitiesTemplateStorage::create(const std::string& templateName)
{
	auto newEntityTemplate = mTemplatesRegistry.create();
	mTemplatesMap.insert(std::make_pair(templateName, newEntityTemplate));
	return newEntityTemplate;
}

entt::entity EntitiesTemplateStorage::createCopy(const std::string& templateName, entt::registry& gameRegistry)
{
	return gameRegistry.create(getTemplate(templateName), mTemplatesRegistry);
}

entt::entity EntitiesTemplateStorage::getTemplate(const std::string& templateName)
{
	return mTemplatesMap.at(templateName);
}

}
