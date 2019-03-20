#include "Renderer/Renderer.hpp"

#include "World/Entity/Object.hpp"

using PopHead::Renderer::Renderer; 
using PopHead::Renderer::Layer;
using PopHead::Renderer::LayerID;

Renderer::Renderer()
        : mCamera { { 0, 0, 100, 100 } } 
        // std::vector< sf::ViewPort > mViewports;
        , mWindow { sf::VideoMode::getDesktopMode(), "test" }
        , mLayers {}
{

};

void Renderer::draw() const
{
    for( const auto& layer : mLayers )
        for( const auto& object : layer.second )
            object->draw();
};
