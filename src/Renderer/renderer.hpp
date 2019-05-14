#ifndef POPHEAD_RENDERER_RENDERER_H_
#define POPHEAD_RENDERER_RENDERER_H_

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include "Renderer/layer.hpp"
#include "Renderer/layerID.hpp"
#include "Renderer/camera.hpp"

namespace PopHead {
namespace Renderer {


class Renderer
{
public:
    Renderer();
    ~Renderer();
    Renderer( Renderer& ) = delete;
    Renderer& operator=( Renderer& ) = delete;

    enum Viewports
    {
        FullScreenViewport
    };

	void update(sf::Time delta);
    void draw() const;

    void addObject(World::Entity::Object* const);
    void addObject(World::Entity::Object* const, LayerID);

    void removeObject(const World::Entity::Object* const);
    void removeObject(std::string name, LayerID);
    void removeAllObjectsFromLayer(LayerID);

	void startShaking(float shakeStrength) { mCamera.setShakeStrength(shakeStrength); }
	void moveCamera(sf::Vector2f center, float speed) { mCamera.move(center, speed); }

    auto getWindow() const -> sf::RenderWindow& { return mWindow; }
	auto getCamera() -> Camera& { return mCamera; }

private:
    void setPositionOfStaticObjectsToCamera();

private:
    Camera mCamera;
    const std::map< Viewports, sf::Rect< float > > mViewports;
    mutable sf::RenderWindow mWindow;
    std::map< LayerID, Layer > mLayers;
};

}}

#endif // !POPHEAD_RENDERER_RENDERER_H_
