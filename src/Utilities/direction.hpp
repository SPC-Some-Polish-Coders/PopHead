#pragma once

#define PH_NORTH Vec2(0.f, -1.f) 
#define PH_SOUTH Vec2(0.f, 1.f) 
#define PH_EAST Vec2(1.f, 0.f) 
#define PH_WEST Vec2(-1.f, 0.f) 
#define PH_NORTH_EAST Vec2(0.7f, -0.7f) 
#define PH_NORTH_WEST Vec2(-0.7f, -0.7f) 
#define PH_SOUTH_EAST Vec2(0.7f, 0.7f) 
#define PH_SOUTH_WEST Vec2(-0.7f, 0.7f) 
#define PH_NULL_DIRECTION Vec2(0.f, 0.f) 
#define PH_IS_DIAGONAL(direction) direction == PH_NORTH_EAST ||  direction == PH_NORTH_WEST || direction == PH_SOUTH_EAST || direction == PH_SOUTH_WEST
#define PH_IS_X_AXIS_POSITIVE(direction) direction == PH_EAST ||  direction == PH_NORTH_EAST || direction == PH_SOUTH_EAST  
#define PH_IS_X_AXIS_NEGATIVE(direction) direction == PH_WEST ||  direction == PH_NORTH_WEST || direction == PH_SOUTH_WEST  
#define PH_IS_Y_AXIS_POSITIVE(direction) direction == PH_SOUTH ||  direction == PH_SOUTH_WEST || direction == PH_SOUTH_EAST
#define PH_IS_Y_AXIS_NEGATIVE(direction) direction == PH_NORTH ||  direction == PH_NORTH_WEST || direction == PH_NORTH_EAST
