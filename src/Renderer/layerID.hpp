#ifndef POPHEAD_RENDERER_LAYERID_H_
#define POPHEAD_RENDERER_LAYERID_H_

namespace ph {

enum class LayerID
{
    floorEntities,
    staticEntities,
    kinematicEntities,
    airEntities,
    collisionDebug,
    gui,
	cmd
};

}

#endif // !POPHEAD_RENDERER_LAYERID_H_
