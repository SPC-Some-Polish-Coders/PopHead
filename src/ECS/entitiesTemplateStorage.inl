namespace ph {

template<typename T, typename... Args>
void EntitiesTemplateStorage::assign(const std::string& templateName, Args&&... arguments)
{
	auto entityTemplate = mTemplatesMap.at(templateName);
	mTemplatesRegistry.assign<T>(entityTemplate, arguments...);
}

template<typename T, typename... Args>
void EntitiesTemplateStorage::assign(entt::entity& templateEntity, Args&&... arguments)
{
	mTemplatesRegistry.assign<T>(templateEntity, arguments...);
}

template<typename T, typename... Args>
void EntitiesTemplateStorage::assign_or_replace(const std::string& templateName, Args&&... arguments)
{
	auto entityTemplate = mTemplatesMap.at(templateName);
	mTemplatesRegistry.assign_or_replace<T>(entityTemplate, arguments...);
}

template<typename T, typename... Args>
void EntitiesTemplateStorage::assign_or_replace(entt::entity& templateEntity, Args&&... arguments)
{
	mTemplatesRegistry.assign_or_replace<T>(templateEntity, arguments...);
}

template<typename T>
bool EntitiesTemplateStorage::has(entt::entity& templateEntity)
{
	return mTemplatesRegistry.has<T>(templateEntity);
}

template<typename T>
bool EntitiesTemplateStorage::has(const std::string& templateName)
{
	auto templateEntity = mTemplatesMap.at(templateName);
	return mTemplatesRegistry.has<T>(templateEntity);
}

template<typename T>
T EntitiesTemplateStorage::get(const std::string& templateName)
{
	auto entityTemplate = mTemplatesMap.at(templateName);
	return mTemplatesRegistry.get<T>(entityTemplate);
}

template<typename T>
T EntitiesTemplateStorage::get(entt::entity& templateEntity)
{
	return mTemplatesRegistry.get<T>(templateEntity);
}

}
