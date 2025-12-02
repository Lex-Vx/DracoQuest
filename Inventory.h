#pragma once
#include <stdint.h>
#include "Items.h"

class Inventory
{
public:
    static constexpr uint8_t MAX_ITEMS = 8;

    struct Slot
    {
        uint8_t id = ITEM_NONE;
    };

    void Init();

    bool AddItem(uint8_t id);      // Returns false if inventory full
    bool RemoveItem(uint8_t id);   // Removes first matching item
    bool HasItem(uint8_t id) const;

    uint8_t GetItemAt(uint8_t index) const;
    bool IsFull() const;

    uint8_t Count() const;         // count used slots

private:
    Slot slots[MAX_ITEMS];
};

// single global instance (definition in Inventory.cpp)
extern Inventory inventory;
