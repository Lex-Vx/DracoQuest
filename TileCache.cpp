#include "TileCache.h"
#include <pgmspace.h>
#include <esp_heap_caps.h> // for heap_caps_malloc / MALLOC_CAP_SPIRAM
#include <cstring>

uint16_t* TileCache::tiles[TILE_COUNT] = { nullptr };
bool TileCache::initialized = false;
bool TileCache::cachedAll = false;

// Initialize the PGMTILE pointer table
// NOTE: order must match tile_0 .. tile_17 in Tiles.h
const uint16_t* TileCache::pgmTiles[TILE_COUNT] = 
{
    tile_0, tile_1, tile_2, tile_3, tile_4, tile_5, tile_6, tile_7,
    tile_8, tile_9, tile_10, tile_11, tile_12, tile_13, tile_14, tile_15,
    tile_16, tile_17, tile_18, tile_19, tile_20, tile_21, tile_22, tile_23, tile_24, tile_25, 
    tile_26, tile_27, tile_28, tile_29, tile_30, tile_31, tile_32, tile_33, tile_34, tile_35, tile_36, 
    tile_37, tile_38, tile_39, tile_40, tile_41, tile_42, tile_43, tile_44, tile_45, tile_46
};

bool TileCache::Init() 
{
  if (initialized) return cachedAll;
  initialized = true;
  cachedAll = true;

  const size_t bytes = (size_t)TILE_PIXELS * sizeof(uint16_t);

  for (uint16_t i = 0; i < TILE_COUNT; ++i) 
  {
    // attempt to allocate in SPIRAM
    uint16_t* buf = (uint16_t*) heap_caps_malloc(bytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (!buf) 
    {
      // try without the MALLOC_CAP_8BIT flag (some cores/configs differ)
      buf = (uint16_t*) heap_caps_malloc(bytes, MALLOC_CAP_SPIRAM);
    }

    if (!buf) 
    {
      // Allocation failed — clean up what we already allocated and mark fallback
      for (uint16_t j = 0; j < i; ++j) 
      {
        if (tiles[j]) 
        {
          heap_caps_free(tiles[j]);
          tiles[j] = nullptr;
        }
      }
      cachedAll = false;
      return false; // caller should fall back
    }

    // Copy tile pixels from PROGMEM -> PSRAM
    const uint16_t* pgmPtr = pgmTiles[i];
    for (size_t p = 0; p < TILE_PIXELS; ++p) 
    {
      buf[p] = (uint16_t)pgm_read_word(&pgmPtr[p]);
    }

    tiles[i] = buf;
  }

  return true;
}

void TileCache::Free() 
{
  for (uint16_t i = 0; i < TILE_COUNT; ++i) 
  {
    if (tiles[i]) 
    {
      heap_caps_free(tiles[i]);
      tiles[i] = nullptr;
    }
  }
  cachedAll = false;
  initialized = false;
}
