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
	//LOG: "unable to load file " + filepath + "! Probably there is not such file."
    return false;
}

template< typename ResourceType >
auto ResourceHolder<ResourceType>::get(const std::string& name) -> ResourceType&
{
    auto found = mResources.find(name);
	if (found == mResources.end()) {
		//LOG: You try to get "cat.png". A resource with this name does not exist. 
		///assert will be removed and replaced by log.
		assert((false && "resource was not found!"));
	}
	else {
	    return *found->second;
	}
}

template< typename ResourceType >
void ResourceHolder<ResourceType>::free(const std::string& name)
{
	for (auto it = mResources.begin(); it != mResources.end(); ++it) {
		if (it->first == name) {
			it = mResources.erase(it);
			//LOG: You try to free "cat.png". A resource with this name does not exist. 
			///assert will be removed and replaced by log.
			assert((false && "resource was not found!"));
			break;
		}
	}
}
