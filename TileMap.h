#pragma once
#include "Tiles.h"
#include "Maps.h"
#include "Configs.h"
#include "WorldPosition.h"
#include "Camera.h"
#include "TileCache.h"
#include "Globals.h"


// draw a single tile (gid = 1..TILE_COUNT) at pixel coords (dx,dy) into sprite 'spr'
void drawTile(LGFX_Sprite* spr, int16_t dx, int16_t dy, uint16_t gid);

// draw the visible portion of an area (map->tiles is PROGMEM pointer to width*height uint16_t GIDs)[GID = Global ID, and was a term I brought over from Tiled, because I couldn't think of a better name].
// This is as good of a place as any to mention that I constructed all of the maps in Tiled, using self drawn tiles, which I basically ripped off from Dragon Quest haha
void drawMap(const MapInfo* map, const Camera& cam, LGFX_Sprite* spr);

