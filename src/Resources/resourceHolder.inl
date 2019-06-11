#include "Utilities/debug.hpp"
#include "resourceHolder.hpp"

using ph::Resources::ResourceHolder;

template< typename ResourceType >
void ResourceHolder<ResourceType>::load(const std::string& filePath)
{
	std::string fullFilePath = "resources/" + filePath;
    auto resource = std::make_unique< ResourceType >();
    if(resource->loadFromFile(fullFilePath))
		mResources.insert(std::make_pair(fullFilePath, std::move(resource)));
	else
		PH_EXCEPTION("unable to load file " + fullFilePath + "! Probably there is not such file.");
}

template< typename ResourceType >
auto ResourceHolder<ResourceType>::get(const std::string& filePath) -> ResourceType&
{
	std::string fullFilePath = "resources/" + filePath;
    auto found = mResources.find(fullFilePath);
	PH_ASSERT(found != mResources.end(), "Resource \"" + fullFilePath + "\" was not found!");
	return *found->second;
}

template< typename ResourceType >
void ResourceHolder<ResourceType>::free(const std::string& filePath)
{
	std::string fullFilePath = "resources/" + filePath;
	for (auto it = mResources.begin(); it != mResources.end(); ++it) {
		if (it->first == fullFilePath) {
			it = mResources.erase(it);
			return;
		}
	}
	PH_LOG(LogType::Error, "You try to free " + fullFilePath + ". A resource with this name does not exist.");
}
