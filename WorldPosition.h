#pragma once
#include <stdint.h>

// The game's navigational core, it handles where you are in 3 layers
struct WorldPosition 
{
    // The world map is a 6x6[36] grid of "Zones"
    int8_t zoneX;
    int8_t zoneY;
    // Each Zone is made up of a 5x5[25] grid of "Areas"
    int8_t areaX;
    int8_t areaY;
    // Each Area is a 25x25 tile map, which is what you see in the viewport
    int8_t tileX;
    int8_t tileY;
    int8_t lastTileY;
    int8_t lastTileX;
};