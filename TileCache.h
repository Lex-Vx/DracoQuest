#pragma once
#include <Arduino.h>
#include "Tiles.h"

// TileCache caches tile bitmaps from PROGMEM into PSRAM for fast rendering.

class TileCache 
{
  public:
    // Initialize the cache. Returns true on full PSRAM caching success.
    // If false, no tiles will be cached and callers should use fallback logic.
    static bool Init();

    // Return pointer to PSRAM copy for the given gid (1..TILE_COUNT).
    // Returns nullptr if gid==0 or if the tile isn't cached (fallback).
    static inline uint16_t* Get(uint16_t gid) 
    {
      if (gid == 0) return nullptr;
      uint16_t idx = gid - 1;
      if (idx >= TILE_COUNT) return nullptr;
      return tiles[idx];
    }

    // Return the PROGMEM pointer for the given gid (1..TILE_COUNT).
    // Useful for fallback copying when PSRAM isn't available.
    static inline const uint16_t* GetPgmPointer(uint16_t gid) 
    {
      if (gid == 0) return nullptr;
      uint16_t idx = gid - 1;
      if (idx >= TILE_COUNT) return nullptr;
      return pgmTiles[idx];
    }

    // Free any allocated PSRAM (optional)
    static void Free();

  private:
    static uint16_t* tiles[TILE_COUNT];               // PSRAM buffers or nullptr
    static const uint16_t* pgmTiles[TILE_COUNT];      // PROGMEM tile pointers
    static bool initialized;
    static bool cachedAll;
};
