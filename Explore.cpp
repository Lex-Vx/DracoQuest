#include "Explore.h"
#include "Renderer.h"
#include <Arduino.h>

void Explore::Init(const MapInfo* startMap)
{
  (void)startMap;
}

void Explore::Update(Player& player, WorldPosition& worldPos, const MapInfo*& currentMap, bool& usingInteriorMap, GameState& currentState)
{
  if (!currentMap) {
    Serial.println("Explore::Update: currentMap is nullptr, skipping update.");
    return;
  }
  // handle player's tile movement updates
  player.UpdateTopDown();
  // sync player tile pos into worldPos
  worldPos.tileX = player.pos.tileX;
  worldPos.tileY = player.pos.tileY;
  // clamp within map bounds
  if (worldPos.tileX < 0) worldPos.tileX = 0;
  if (worldPos.tileY < 0) worldPos.tileY = 0;
  if (worldPos.tileX >= currentMap->width)  worldPos.tileX = currentMap->width - 1;
  if (worldPos.tileY >= currentMap->height) worldPos.tileY = currentMap->height - 1;
  // check area/zone crossing (overworld movement)
  bool areaChanged = false;
  handleOverworldCrossing(player, worldPos, currentMap, areaChanged);
  // when area changed, update zone and reload area map
  if (areaChanged)
  {
    if (worldPos.areaX < 0)
    {
      worldPos.zoneX -= 1;
      worldPos.areaX = 4;
      Serial.printf("Zone crossed left: zoneX=%d\n", worldPos.zoneX);
    }
    else if (worldPos.areaX > 4)
    {
      worldPos.zoneX += 1;
      worldPos.areaX = 0;
      Serial.printf("Zone crossed right: zoneX=%d\n", worldPos.zoneX);
    }
    if (worldPos.areaY < 0)
    {
      worldPos.zoneY -= 1;
      worldPos.areaY = 4;
      Serial.printf("Zone crossed top: zoneY=%d\n", worldPos.zoneY);
    }
    else if (worldPos.areaY > 4)
    {
      worldPos.zoneY += 1;
      worldPos.areaY = 0;
      Serial.printf("Zone crossed bottom: zoneY=%d\n", worldPos.zoneY);
    }

    currentMap = GetAreaMap(worldPos.zoneX, worldPos.zoneY, worldPos.areaX, worldPos.areaY);
    if (!currentMap)
    {
      Serial.println("Explore::Update: Failed to load new area map, using default.");
      currentMap = &default_map_info;
    }

    // Re-sync tile coords
    player.pos.tileX = player.x / TILE_SIZE;
    player.pos.tileY = player.y / TILE_SIZE;
    worldPos.tileX    = player.pos.tileX;
    worldPos.tileY    = player.pos.tileY;
    player.posClean();
  }

  // If we are in overworld (not interior), check for entering interiors (houses/dungeons)
  if (!usingInteriorMap)
  {
    worldPos.tileX = constrain(worldPos.tileX, 0, currentMap->width  - 1);
    worldPos.tileY = constrain(worldPos.tileY, 0, currentMap->height - 1);
    int tileIndex = worldPos.tileY * currentMap->width + worldPos.tileX;
    uint16_t gid  = pgm_read_word(&currentMap->tiles[tileIndex]);

    // Store overworld return point
    player.pos.lastTileX = player.pos.tileX;
    player.pos.lastTileY = player.pos.tileY;

    uint8_t interiorID = LookupInteriorID(worldPos.zoneX, worldPos.zoneY, worldPos.areaX, worldPos.areaY, gid);

    if (interiorID != 0)
    {
      // Enter interior
      Serial.printf("Entering interior ID %d\n", interiorID);
      const MapInfo* newMap = GetInterior(interiorID);
      if (!newMap)
      {
        Serial.println("Explore::Update ERROR: interior map missing!");
        return;
      }
      currentMap = newMap;
      usingInteriorMap = true;
      player.SetCurrentInteriorID(interiorID);
      currentInterior = player.GetCurrentInteriorID();
      // Set correct state based on interior type
      if (currentMap->type == 1)
      {
        currentState = GS_TOWN;
      }
      else if (currentMap->type == 2)
      {
        currentState = GS_DUNGEON;
      }
      else
      {
        currentState = GS_EXPLORE;
      }

      // interior spawn point
      player.pos.tileX = 1;
      player.pos.tileY = 2;
      player.x = player.pos.tileX * TILE_SIZE;
      player.y = player.pos.tileY * TILE_SIZE;
      player.posClean();
    }
  }
}

void Explore::handleOverworldCrossing(Player& player, WorldPosition& worldPos, const MapInfo*& currentMap, bool& areaChanged)
{
  const int mapPixelW = currentMap->width * TILE_SIZE;
  const int mapPixelH = currentMap->height * TILE_SIZE;
  if (player.x < 0)
  {
    worldPos.areaX -= 1;
    player.x += mapPixelW;
    areaChanged = true;
    Serial.printf("Crossed left: new areaX=%d\n", worldPos.areaX);
  }
  else if (player.x >= mapPixelW)
  {
    worldPos.areaX += 1;
    player.x -= mapPixelW;
    areaChanged = true;
    Serial.printf("Crossed right: new areaX=%d\n", worldPos.areaX);
  }
  if (player.y < 0)
  {
    worldPos.areaY -= 1;
    player.y += mapPixelH;
    areaChanged = true;
    Serial.printf("Crossed top: new areaY=%d\n", worldPos.areaY);
  }
  else if (player.y >= mapPixelH)
  {
    worldPos.areaY += 1;
    player.y -= mapPixelH;
    areaChanged = true;
    Serial.printf("Crossed bottom: new areaY=%d\n", worldPos.areaY);
  }
}

void Explore::Render(const MapInfo* currentMap, Renderer& renderer, Camera& camera, Player& player)
{
  auto bg   = renderer.GetLayerBackground();
  auto ents = renderer.GetLayerEntities();
  auto ui   = renderer.GetLayerUI();
  drawMap(currentMap, camera, bg);
  player.Draw(ents, int(player.x - camera.pixelX), int(player.y - camera.pixelY));
  messageWindow.Update();
  messageWindow.Draw(ui);
}
