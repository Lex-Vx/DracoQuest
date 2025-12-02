#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "WorldPosition.h"
#include "Items.h"
#include "ChestManager.h"

// ------------------------
// MapInfo structure
// ------------------------
struct MapInfo 
{
    const uint16_t* tiles; 
    uint16_t width;
    uint16_t height;
    uint8_t type;
};

struct AreaToInterior 
{
    uint8_t zoneX;
    uint8_t zoneY;
    uint8_t areaX;
    uint8_t areaY;
    uint16_t entranceGID; // tile GID that triggers interior
    uint16_t interiorID;   // interior map to load
};

// Lookup table
extern const AreaToInterior areaTransitions[];
extern const size_t areaTransitionsCount;
uint8_t LookupInteriorID(uint8_t zx, uint8_t zy, uint8_t ax, uint8_t ay, uint16_t gid);

// ------------------------
// Map registry system
// ------------------------
void RegisterArea(uint8_t zoneX, uint8_t zoneY, uint8_t areaX, uint8_t areaY, const MapInfo* map);

const MapInfo* GetAreaMap(uint8_t zoneX, uint8_t zoneY, uint8_t areaX, uint8_t areaY);


// Interior / Town / Dungeon registry
void RegisterInterior(uint8_t id, const MapInfo* map);

const MapInfo* GetInterior(uint8_t id);

// Optional initializer for all maps (overworld + interiors)
void InitAllMaps();

// Default map (safe fallback)
extern const MapInfo default_map_info;

// map declarations (automatically registered in InitAllMaps)
    //ZONE 0
extern const MapInfo z00a00_map_info; extern const MapInfo z00a01_map_info; extern const MapInfo z00a02_map_info; extern const MapInfo z00a03_map_info; extern const MapInfo z00a04_map_info;
extern const MapInfo z00a05_map_info; extern const MapInfo z00a06_map_info; extern const MapInfo z00a07_map_info; extern const MapInfo z00a08_map_info; extern const MapInfo z00a09_map_info;
extern const MapInfo z00a10_map_info; extern const MapInfo z00a11_map_info; extern const MapInfo z00a12_map_info; extern const MapInfo z00a13_map_info; extern const MapInfo z00a14_map_info;
extern const MapInfo z00a15_map_info; extern const MapInfo z00a16_map_info; extern const MapInfo z00a17_map_info; extern const MapInfo z00a18_map_info; extern const MapInfo z00a19_map_info;
extern const MapInfo z00a20_map_info; extern const MapInfo z00a21_map_info; extern const MapInfo z00a22_map_info; extern const MapInfo z00a23_map_info; extern const MapInfo z00a24_map_info;
    //Z0 interiors
extern const MapInfo DungeonNW_map_info;
extern const MapInfo town_01_map_info;

