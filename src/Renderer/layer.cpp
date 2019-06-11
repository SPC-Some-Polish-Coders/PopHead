#include "Renderer/layer.hpp"

#include "Renderer/layerID.hpp"

#include "World/Entity/object.hpp"

void ph::Layer::addObject(Object* const object)
{
    mObjects.emplace_back( object );
}

void ph::Layer::removeObject(const Object* const object)
{
    for(auto it = mObjects.begin(); it != mObjects.end(); ++it )
        if(*it == object)
        {
            mObjects.erase(it);
            return;
        }
}

void ph::Layer::removeObject(std::string name)
{
    for(auto it = mObjects.begin(); it != mObjects.end(); ++it )
        if((*it)->getName() == name)
            mObjects.erase(it);
}
