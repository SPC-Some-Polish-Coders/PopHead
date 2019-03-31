#include "resourceHolder.hpp"

using PopHead::Resources::ResourceHolder;

template<typename Resource>
inline bool ResourceHolder<Resource>::has()
{
    if(mResources.find(name)==mResources.end())
    {
        return 0;
    }
    return 1;
}
inline void free( const std::string& name )
{
    for(auto it = mResources.begin(); it != mResources.end(); )
        if(it->first == name)
            it = mResources.erase(it);
        else
            ++it;
}
inline auto get( const std::string& name ) -> Resource&
{
    auto found = mResources.find(name);
    return *found->second;
}
