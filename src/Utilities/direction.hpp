#pragma once

#define PH_NORTH sf::Vector2f(0.f, -1.f) 
#define PH_SOUTH sf::Vector2f(0.f, 1.f) 
#define PH_EAST sf::Vector2f(1.f, 0.f) 
#define PH_WEST sf::Vector2f(-1.f, 0.f) 
#define PH_NORTH_EAST sf::Vector2f(0.7f, -0.7f) 
#define PH_NORTH_WEST sf::Vector2f(-0.7f, -0.7f) 
#define PH_SOUTH_EAST sf::Vector2f(0.7f, 0.7f) 
#define PH_SOUTH_WEST sf::Vector2f(-0.7f, 0.7f) 
#define PH_NULL_DIRECTION sf::Vector2f(0.f, 0.f) 
#define PH_IS_DIAGONAL(direction) direction == PH_NORTH_EAST ||  direction == PH_NORTH_WEST || direction == PH_SOUTH_EAST || direction == PH_SOUTH_WEST
#define PH_IS_X_AXIS_POSITIVE(direction) direction == PH_EAST ||  direction == PH_NORTH_EAST || direction == PH_SOUTH_EAST  
#define PH_IS_X_AXIS_NEGATIVE(direction) direction == PH_WEST ||  direction == PH_NORTH_WEST || direction == PH_SOUTH_WEST  
#define PH_IS_Y_AXIS_POSITIVE(direction) direction == PH_SOUTH ||  direction == PH_SOUTH_WEST || direction == PH_SOUTH_EAST
#define PH_IS_Y_AXIS_NEGATIVE(direction) direction == PH_NORTH ||  direction == PH_NORTH_WEST || direction == PH_NORTH_EAST
