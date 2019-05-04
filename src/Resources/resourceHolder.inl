#include "Logs.hpp"

#include <cassert>//This include must by removed when asserts are replaced by logs

using PopHead::Resources::ResourceHolder;

template< typename ResourceType >
bool ResourceHolder<ResourceType>::load(const std::string& filepath)
{
    auto resource = std::make_unique< ResourceType >();

    if(resource->loadFromFile(filepath) ){
		mResources.insert(std::make_pair(filepath, std::move(resource)));
        return true;
    }
	PopHead::LOG(LogType::ERROR, ModuleID::Resources, "unable to load file " + filepath + "! Probably there is not such file.");
    return false;
}

template< typename ResourceType >
auto ResourceHolder<ResourceType>::get(const std::string& name) -> ResourceType&
{
    auto found = mResources.find(name);
	if (found == mResources.end())
		PopHead::LOG(LogType::ERROR, ModuleID::Resources, "You try to get " + name + ". A resource with this name does not exist.");
	else
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
	PopHead::LOG(LogType::ERROR, ModuleID::Resources, "You try to get " + name + ". A resource with this name does not exist.");
}
