#include "Town.h"
#include <Arduino.h>

void Town::Update(Player& player, WorldPosition& worldPos, const MapInfo*& currentMap, bool& usingInteriorMap, GameState& currentState)
{
  player.UpdateTopDown();
  int maxPx = currentMap->width  * TILE_SIZE;
  int maxPy = currentMap->height * TILE_SIZE;
  if (player.x < 0 || player.x >= maxPx || player.y < 0 || player.y >= maxPy)
  {
    Serial.printf("Exiting town\n");
    currentMap = GetAreaMap(worldPos.zoneX, worldPos.zoneY, worldPos.areaX, worldPos.areaY);
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

void Town::Render(const MapInfo* currentMap, Renderer& renderer, Camera& camera, Player& player)
{
  auto bg   = renderer.GetLayerBackground();
  auto ents = renderer.GetLayerEntities();
  auto ui   = renderer.GetLayerUI();
  drawMap(currentMap, camera, bg);
  player.Draw(ents, int(player.x - camera.pixelX), int(player.y - camera.pixelY));
  messageWindow.Update();
  messageWindow.Draw(ui);
}
