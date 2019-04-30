#include <cassert>

using PopHead::Resources::ResourceHolder;

template< typename ResourceType >
bool ResourceHolder<ResourceType>::load(const std::string& filepath)
{
    auto resource = std::make_unique< ResourceType >();

    if(resource->loadFromFile(filepath) ){
		mResources.insert(std::make_pair(filepath, std::move(resource)));
        return true;
    }
	///LOG will be here "unable to load file " + filepath + "! Probably there is not such file."
    return false;
}

template< typename ResourceType >
auto ResourceHolder<ResourceType>::get( const std::string& name ) -> ResourceType&
{
    auto found = mResources.find(name);
	if (found == mResources.end()) {
		///LOG will be here later
		assert((false && "resource was not found!"));
	}
	else {
	    return *found->second;
	}
}

template< typename ResourceType >
void ResourceHolder<ResourceType>::free( const std::string& name )
{
	for (auto it = mResources.begin(); it != mResources.end();) {
		if (it->first == name)
			it = mResources.erase(it);
		else
			++it;
	}
}
