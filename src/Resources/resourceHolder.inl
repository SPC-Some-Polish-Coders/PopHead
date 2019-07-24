#include "Logs/logs.hpp"
#include "resourceHolder.hpp"

template< typename ResourceType >
void ph::ResourceHolder<ResourceType>::load(const std::string& filePath)
{
	std::string fullFilePath = "resources/" + filePath;
	auto resource = std::make_unique< ResourceType >();
	if (resource->loadFromFile(fullFilePath))
		mResources.insert(std::make_pair(fullFilePath, std::move(resource)));
	else
		PH_EXCEPTION("unable to load file " + fullFilePath + "! Probably there is not such file.");
}

template< typename ResourceType >
auto ph::ResourceHolder<ResourceType>::get(const std::string& filePath) -> ResourceType &
{
	std::string fullFilePath = "resources/" + filePath;
	auto found = mResources.find(fullFilePath);
	if (found == mResources.end())
		PH_EXCEPTION("Resource \"" + fullFilePath + "\" was not found!");
	return *found->second;
}

template< typename ResourceType >
void ph::ResourceHolder<ResourceType>::free(const std::string& filePath)
{
	std::string fullFilePath = "resources/" + filePath;
	auto amountOfDeletedResources = mResources.erase(fullFilePath);   // can be equal 0 or 1

	if (amountOfDeletedResources == 0)
		PH_LOG(LogLevel::Error, "You try to free " + fullFilePath + ". A resource with this name does not exist.");
}
