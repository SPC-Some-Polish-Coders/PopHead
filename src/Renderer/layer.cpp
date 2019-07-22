#include "Renderer/layer.hpp"

#include "Renderer/layerID.hpp"

#include "GameObjects/drawableGameObject.hpp"

namespace ph {

void Layer::addObject(DrawableGameObject* const object)
{
    mObjects.emplace_back( object );
}

void Layer::removeObject(const DrawableGameObject* const object)
{
    for(auto it = mObjects.begin(); it != mObjects.end(); ++it )
        if(*it == object)
        {
            mObjects.erase(it);
            return;
        }
}

void Layer::removeObject(std::string name)
{
    for(auto it = mObjects.begin(); it != mObjects.end(); ++it )
        if((*it)->getName() == name)
            mObjects.erase(it);
}

}