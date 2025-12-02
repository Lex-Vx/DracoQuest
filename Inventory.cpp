#include "Inventory.h"

void Inventory::Init()
{
    for (uint8_t i = 0; i < MAX_ITEMS; ++i)
        slots[i].id = ITEM_NONE;
}

bool Inventory::AddItem(uint8_t id)
{
    if (id == ITEM_GOLD) return true; // gold not stored in inventory

    for (uint8_t i = 0; i < MAX_ITEMS; ++i)
    {
        if (slots[i].id == ITEM_NONE)
        {
            slots[i].id = id;
            return true;
        }
    }
    return false; // full
}

bool Inventory::RemoveItem(uint8_t id)
{
    for (uint8_t i = 0; i < MAX_ITEMS; ++i)
    {
        if (slots[i].id == id)
        {
            slots[i].id = ITEM_NONE;
            return true;
        }
    }
    return false;
}

bool Inventory::HasItem(uint8_t id) const
{
    for (uint8_t i = 0; i < MAX_ITEMS; ++i)
    {
        if (slots[i].id == id)
            return true;
    }
    return false;
}

uint8_t Inventory::GetItemAt(uint8_t index) const
{
    if (index >= MAX_ITEMS) return ITEM_NONE;
    return slots[index].id;
}

bool Inventory::IsFull() const
{
    for (uint8_t i = 0; i < MAX_ITEMS; ++i)
        if (slots[i].id == ITEM_NONE) return false;
    return true;
}

uint8_t Inventory::Count() const
{
    uint8_t cnt = 0;
    for (uint8_t i = 0; i < MAX_ITEMS; ++i)
        if (slots[i].id != ITEM_NONE) cnt++;
    return cnt;
}

