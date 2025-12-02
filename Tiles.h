#pragma once
#include <Arduino.h>
#include <pgmspace.h>

extern const uint16_t tile_0[256] PROGMEM; //Barricade
extern const uint16_t tile_1[256] PROGMEM; //Bridge
extern const uint16_t tile_2[256] PROGMEM; //Castle1
extern const uint16_t tile_3[256] PROGMEM; //Chest1
extern const uint16_t tile_4[256] PROGMEM; //CoastCorner1
extern const uint16_t tile_5[256] PROGMEM; //CoastCorner2
extern const uint16_t tile_6[256] PROGMEM; //CoastCorner3
extern const uint16_t tile_7[256] PROGMEM; //CoastCorner4
extern const uint16_t tile_8[256] PROGMEM; //CoastEdge1
extern const uint16_t tile_9[256] PROGMEM; //CoastEdge2
extern const uint16_t tile_10[256] PROGMEM; //CoastEdge3
extern const uint16_t tile_11[256] PROGMEM; //CoastEdge4
extern const uint16_t tile_12[256] PROGMEM; //CoastInlet
extern const uint16_t tile_13[256] PROGMEM; //Door
extern const uint16_t tile_14[256] PROGMEM; //DownStairs
extern const uint16_t tile_15[256] PROGMEM; //Dungeon
extern const uint16_t tile_16[256] PROGMEM; //Flowers1
extern const uint16_t tile_17[256] PROGMEM; //Flowers2
extern const uint16_t tile_18[256] PROGMEM; //Flowers3
extern const uint16_t tile_19[256] PROGMEM; //Flowers4
extern const uint16_t tile_20[256] PROGMEM; //Grass1
extern const uint16_t tile_21[256] PROGMEM; //Grass2
extern const uint16_t tile_22[256] PROGMEM; //Grass3
extern const uint16_t tile_23[256] PROGMEM; //Grass4
extern const uint16_t tile_24[256] PROGMEM; //Hills
extern const uint16_t tile_25[256] PROGMEM; //Inn
extern const uint16_t tile_26[256] PROGMEM; //Marsh1
extern const uint16_t tile_27[256] PROGMEM; //Marsh2
extern const uint16_t tile_28[256] PROGMEM; //Marsh3
extern const uint16_t tile_29[256] PROGMEM; //Marsh4
extern const uint16_t tile_30[256] PROGMEM; //MerchantCounter
extern const uint16_t tile_31[256] PROGMEM; //Mountain
extern const uint16_t tile_32[256] PROGMEM; //Path
extern const uint16_t tile_33[256] PROGMEM; //Sand1
extern const uint16_t tile_34[256] PROGMEM; //Sand2
extern const uint16_t tile_35[256] PROGMEM; //Sand3
extern const uint16_t tile_36[256] PROGMEM; //Sand4
extern const uint16_t tile_37[256] PROGMEM; //Shop
extern const uint16_t tile_38[256] PROGMEM; //StoneBarrier
extern const uint16_t tile_39[256] PROGMEM; //Stream
extern const uint16_t tile_40[256] PROGMEM; //Town1
extern const uint16_t tile_41[256] PROGMEM; //Trees
extern const uint16_t tile_42[256] PROGMEM; //UpStaris
extern const uint16_t tile_43[256] PROGMEM; //Water1
extern const uint16_t tile_44[256] PROGMEM; //Water2
extern const uint16_t tile_45[256] PROGMEM; //Water3
extern const uint16_t tile_46[256] PROGMEM; //Water4

extern const uint8_t SOLID_TILES[];
extern const uint8_t SOLID_TILE_COUNT;

constexpr uint16_t TILE_COUNT = 47;
constexpr uint16_t TILE_SIZE = 16;   // pixels
constexpr uint16_t TILE_PIXELS = 256; // TILE_SIZE*TILE_SIZE