#pragma once
#include <stdint.h>

// A chest stored in tile coordinates (tileX, tileY).
struct ChestEntry
{
    int tileX;
    int tileY;
    uint8_t itemID;
    bool opened;
};

/*
    ChestManager: per-interior chest registry
 Each interior map (by interiorID) has its own chest list.
 All lookups and additions use tile coordinates.
 */
class ChestManager
{
public:
    ChestManager() {}
    void AddChest(uint8_t interiorID, int tileX, int tileY, uint8_t item);
    ChestEntry* FindChestAt(uint8_t interiorID, int tileX, int tileY);
    bool OpenChestAt(uint8_t interiorID, int tileX, int tileY, uint8_t &itemID);

private:
    static const uint8_t MAX_INTERIORS = 128;
    static const int MAX_CHESTS_PER_INTERIOR = 8;

    ChestEntry chests[MAX_INTERIORS][MAX_CHESTS_PER_INTERIOR];
    uint32_t chestCount[MAX_INTERIORS] = {0};
};

extern ChestManager chestManager;
