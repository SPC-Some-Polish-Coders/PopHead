#ifndef POPHEAD_RENDERER_LAYERID_H_
#define POPHEAD_RENDERER_LAYERID_H_

namespace PopHead {
namespace Renderer {

enum class LayerID
{
    floorEntities,
    staticEntities,
    kinematicEntities,
    airEntities,
    collisionDebug,
    GUI
};

}}

#endif // !POPHEAD_RENDERER_LAYERID_H_
