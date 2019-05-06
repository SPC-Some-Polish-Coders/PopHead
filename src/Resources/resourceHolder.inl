#include "Logs/logger.hpp"

#include <cassert>//This include must by removed when asserts are replaced by logs
#include <stdexcept>

using PopHead::Resources::ResourceHolder;

template< typename ResourceType >
void ResourceHolder<ResourceType>::load(const std::string& filepath)
{
    auto resource = std::make_unique< ResourceType >();

    if(resource->loadFromFile(filepath))
		mResources.insert(std::make_pair(filepath, std::move(resource)));
	else {
		PH_LOG(LogType::ERROR, "unable to load file " + filepath + "! Probably there is not such file.");
		throw std::runtime_error("unable to load file " + filepath + "! Probably there is not such file.");
	}
}

template< typename ResourceType >
auto ResourceHolder<ResourceType>::get(const std::string& name) -> ResourceType&
{
    auto found = mResources.find(name);
#ifndef NDEBUG
	if (found == mResources.end())
		PH_LOG(LogType::ERROR, "name");
#endif // !NDEBUG
	return *found->second;
}

template< typename ResourceType >
void ResourceHolder<ResourceType>::free(const std::string& name)
{
	for (auto it = mResources.begin(); it != mResources.end(); ++it) {
		if (it->first == name) {
			it = mResources.erase(it);
			return;
		}
	}
	PH_LOG(LogType::ERROR, "You try to get " + name + ". A resource with this name does not exist.");
}
