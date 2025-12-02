#pragma once
#include <stdint.h>

enum ItemID : uint8_t 
{
    ITEM_NONE = 0,

    // Consumables
    ITEM_HERB = 1,
    ITEM_POTION,
    ITEM_ANTIDOTE,
    ITEM_WING,

    // Key items
    ITEM_FAIRY_FLUTE,
    ITEM_SUN_STONE,
    ITEM_RAINBOW_DROP,

    // Equipment
    ITEM_CLUB,
    ITEM_COPPER_SWORD,
    ITEM_BROAD_SWORD,
    ITEM_FLAME_SWORD,

    ITEM_LEATHER_ARMOR,
    ITEM_CHAIN_MAIL,
    ITEM_FULL_PLATE,

    ITEM_SMALL_SHIELD,
    ITEM_LARGE_SHIELD,
    ITEM_DRAGON_SHIELD,

    // Currency-type item (not in inventory, handled specially)
    ITEM_GOLD,

    ITEM__COUNT // keep last
};

enum ItemType : uint8_t
{
    ITEMTYPE_NONE = 0,
    ITEMTYPE_CONSUMABLE,
    ITEMTYPE_KEYITEM,
    ITEMTYPE_WEAPON,
    ITEMTYPE_ARMOR,
    ITEMTYPE_SHIELD,
    ITEMTYPE_MONEY
};

struct ItemInfo
{
    const char* name;
    ItemType type;
};

inline const ItemInfo& GetItemInfo(uint8_t id)
{
    static const ItemInfo table[] = 
    {
        { "None", ITEMTYPE_NONE },
        { "Herb", ITEMTYPE_CONSUMABLE },
        { "Potion", ITEMTYPE_CONSUMABLE },
        { "Antidote", ITEMTYPE_CONSUMABLE },
        { "Wing", ITEMTYPE_CONSUMABLE },

        { "Fairy Flute", ITEMTYPE_KEYITEM },
        { "Sun Stone", ITEMTYPE_KEYITEM },
        { "Rainbow Drop", ITEMTYPE_KEYITEM },

        { "Club", ITEMTYPE_WEAPON },
        { "Copper Sword", ITEMTYPE_WEAPON },
        { "Broad Sword", ITEMTYPE_WEAPON },
        { "Flame Sword", ITEMTYPE_WEAPON },

        { "Leather Armor", ITEMTYPE_ARMOR },
        { "Chain Mail", ITEMTYPE_ARMOR },
        { "Full Plate", ITEMTYPE_ARMOR },

        { "Small Shield", ITEMTYPE_SHIELD },
        { "Large Shield", ITEMTYPE_SHIELD },
        { "Dragon Shield", ITEMTYPE_SHIELD },

        { "Gold", ITEMTYPE_MONEY },
    };

    return table[id];
}

inline const char* ItemName(uint8_t id)
{
    return GetItemInfo(id).name;
}

inline ItemType ItemCategory(uint8_t id)
{
    return GetItemInfo(id).type;
}
