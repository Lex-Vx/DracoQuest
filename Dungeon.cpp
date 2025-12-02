#include "Dungeon.h"
#include <Arduino.h>

void Dungeon::Update(Player& player, WorldPosition& worldPos, const MapInfo*& currentMap, bool& usingInteriorMap, GameState& currentState)
{
  // top-down movement inside dungeon
  player.UpdateTopDown();

  // Check tile the player stands on for dungeon exit gid (originally: gid == 43)
  int tileIndex = player.pos.tileY * currentMap->width + player.pos.tileX;
  uint16_t gid  = pgm_read_word(&currentMap->tiles[tileIndex]);
  if (gid == 43)
  {
    currentMap = GetAreaMap(worldPos.zoneX, worldPos.zoneY, worldPos.areaX, worldPos.areaY);
    Serial.println("Exiting Dungeon");
    usingInteriorMap = false;
    currentState     = GS_EXPLORE;
    player.SetCurrentInteriorID(0);
    player.pos.tileX = player.pos.lastTileX - 1;
    player.pos.tileY = player.pos.lastTileY;
    player.x = player.pos.tileX * TILE_SIZE;
    player.y = player.pos.tileY * TILE_SIZE;
    player.posClean();
  }
}

void Dungeon::Render(const MapInfo* currentMap, Renderer& renderer, Camera& camera, Player& player)
{
  auto bg   = renderer.GetLayerBackground();
  auto ents = renderer.GetLayerEntities();
  auto ui   = renderer.GetLayerUI();
  drawMap(currentMap, camera, bg);
  player.Draw(ents, int(player.x - camera.pixelX), int(player.y - camera.pixelY));
  messageWindow.Update();
  messageWindow.Draw(ui);
}
