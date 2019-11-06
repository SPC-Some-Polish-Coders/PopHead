#include "Logs/logs.hpp"
#include "resourceHolder.hpp"

template< typename ResourceType >
bool ph::ResourceHolder<ResourceType>::load(const std::string& filePath)
{
	std::string fullFilePath = "resources/" + filePath;
	if (mResources.find(fullFilePath) != mResources.end())
		return true;
	auto resource = std::make_unique< ResourceType >();
	if (resource->loadFromFile(fullFilePath))
	{
		mResources.insert(std::make_pair(fullFilePath, std::move(resource)));
		return true;
	}
	else
	{
		PH_LOG_ERROR("unable to load file \"" + fullFilePath + "\"");
		return false;
	}
}

template< typename ResourceType >
auto ph::ResourceHolder<ResourceType>::get(const std::string& filePath) -> ResourceType &
{
	std::string fullFilePath = "resources/" + filePath;
	auto found = mResources.find(fullFilePath);
	if (found == mResources.end())
	{
		PH_LOG_ERROR("You try to get a resource that wasn't loaded: " + fullFilePath);
		throw std::runtime_error("You try to get a resource that wasn't loaded: " + fullFilePath);
	}
	return *found->second;
}

template< typename ResourceType >
bool ph::ResourceHolder<ResourceType>::free(const std::string& filePath)
{
	std::string fullFilePath = "resources/" + filePath;
	auto amountOfDeletedResources = mResources.erase(fullFilePath);   // can be equal 0 or 1

	PH_ASSERT_WARNING(amountOfDeletedResources == 1, "You try to free " + fullFilePath + ". A resource with this name does not exist.");
}
