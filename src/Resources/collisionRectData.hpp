#ifndef POPHEAD_RESOURCE_COLLISIONRECTDATA_H_
#define POPHEAD_RESOURCE_COLLISIONRECTDATA_H_

namespace PopHead {
namespace Resources {

/// OFFSET_LEFT: number of pixels from left bound of the sprite to the left bound of the actual collision rect
/// OFFSET_TOP: number of pixels from top bound of the sprite to the top bound of the actual collision rect

namespace CollisionRectData
{
	static constexpr unsigned PLAYER_OFFSET_LEFT = 9;
	static constexpr unsigned PLAYER_OFFSET_TOP = 4;
	static constexpr unsigned PLAYER_WIDTH = 27;
	static constexpr unsigned PLAYER_HEIGHT = 44;
}


}}

#endif //!POPHEAD_RESOURCE_COLLISIONRECTDATA_H_