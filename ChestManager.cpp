#include "ChestManager.h"

void ChestManager::AddChest(uint8_t interiorID, int tileX, int tileY, uint8_t item)
{
    if (interiorID >= MAX_INTERIORS) return;
    uint32_t &count = chestCount[interiorID];
    if (count >= MAX_CHESTS_PER_INTERIOR) return;

    chests[interiorID][count++] = { tileX, tileY, item, false };
}

ChestEntry* ChestManager::FindChestAt(uint8_t interiorID, int tileX, int tileY)
{
    if (interiorID >= MAX_INTERIORS) return nullptr;
    int count = chestCount[interiorID];

    for (int i = 0; i < count; ++i)
    {
        if (chests[interiorID][i].tileX == tileX && chests[interiorID][i].tileY == tileY)
        {
            return &chests[interiorID][i];
        }
    }
    return nullptr;
}

bool ChestManager::OpenChestAt(uint8_t interiorID, int tileX, int tileY, uint8_t &itemID)
{
    ChestEntry* chest = FindChestAt(interiorID, tileX, tileY);
    if (!chest) return false;
    if (chest->opened) return false;
    chest->opened = true;
    itemID = chest->itemID;
    return true;
}


