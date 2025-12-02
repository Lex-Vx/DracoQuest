#include "TileMap.h"
#include <pgmspace.h>


static uint16_t tileBuf[TILE_PIXELS];

void drawTile(LGFX_Sprite* spr, int16_t dx, int16_t dy, uint16_t gid) 
{
    if (!spr) return;
    if (gid == 0) return;
    uint16_t* cached = TileCache::Get(gid);
    if (cached) 
    {
        spr->pushImage(dx, dy, TILE_SIZE, TILE_SIZE, cached);
        return;
    }
    const uint16_t* pgmPtr = TileCache::GetPgmPointer(gid);
    if (!pgmPtr) return;
    for (size_t i = 0; i < TILE_PIXELS; ++i) 
    {
        tileBuf[i] = (uint16_t)pgm_read_word(&pgmPtr[i]);
    }
    spr->pushImage(dx, dy, TILE_SIZE, TILE_SIZE, tileBuf);
}

void drawMap(const MapInfo* map, const Camera& cam, LGFX_Sprite* spr) 
{
    if (!map || !spr) return;

    const int TILE = TILE_SIZE;
    const int MAP_W = map->width;
    const int MAP_H = map->height;

    int startTileX = cam.pixelX / TILE;
    int startTileY = cam.pixelY / TILE;
    int offsetX = -(int)fmodf(cam.pixelX, TILE);
    int offsetY = -(int)fmodf(cam.pixelY, TILE);
    int viewTilesX = (cam.width + TILE - 1) / TILE;
    int viewTilesY = (cam.height + TILE - 1) / TILE;

    for (int ty = 0; ty <= viewTilesY; ++ty) 
    {
        int tileY = startTileY + ty;
        if (tileY < 0 || tileY >= MAP_H) continue;
        int drawY = offsetY + ty * TILE;

        for (int tx = 0; tx <= viewTilesX; ++tx) 
        {
            int tileX = startTileX + tx;
            if (tileX < 0 || tileX >= MAP_W) continue;

            int index = tileY * MAP_W + tileX;
            uint16_t gid = pgm_read_word(&map->tiles[index]);

            // Skip empty tiles
            if (gid == 0) continue;
            
            //Check if any chests have been opened, and draw over them
            ChestEntry* chest = chestManager.FindChestAt(currentInterior, tileX, tileY);
            if (chest && chest->opened)
            {
                gid = 33; // floor tile
            }

            int drawX = offsetX + tx * TILE;
            drawTile(spr, drawX, drawY, gid);
        }
    }
}

