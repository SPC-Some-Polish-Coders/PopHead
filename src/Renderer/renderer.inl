#include "Renderer/renderer.hpp"


auto Renderer::getWindow() const -> const sf::Window&
{ return mWindow; }

auto Renderer::getLayer( LayerID id ) -> Layer&
{ return mLayers[id]; }

auto Renderer::getLayer( LayerID id ) const -> const Layer&
{ return mLayers[id]; }

auto Renderer::getCameraPosition() const -> const sf::Vector2f&
{ return mCamera.getCenter(); }

auto Renderer::getCameraSize() const -> const sf::Vector2f&
{ return mCamera.getSize(); }

void Renderer::setCameraPosition( sf::Vector2f center )
{ mCamera.setCenter( center ); }

void Renderer::setCameraSize( sf::Vector2f size )
{ mCamera.setSize( size ); }
