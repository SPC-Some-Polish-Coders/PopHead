#include "Renderer/layer.hpp"

#include "Renderer/layerID.hpp"

#include "World/Entity/object.hpp"

using PopHead::Renderer::Layer;
using PopHead::Renderer::LayerID;

void Layer::addObject( PopHead::World::Entity::Object* const object )
{
    mObjects.emplace_back( object );
}

void Layer::removeObject( const PopHead::World::Entity::Object* const object )
{
    for(auto it = mObjects.begin(); it != mObjects.end(); ++it )
        if(*it == object)
        {
            mObjects.erase(it);
            return;
        }
}

void Layer::removeObject( std::string name )
{
    for(auto it = mObjects.begin(); it != mObjects.end(); ++it )
        if((*it)->getName() == name)
            mObjects.erase(it);
}
