#include "Game.h"

// Initialize the globals
ChestManager chestManager;
Inventory inventory;
MessageWindow messageWindow;
DeltaTime dTime;

Game::Game() : renderer(), player("Hero", 1, 1, 1, 1, 1, 1)
{
  screenW = 160;
  screenH = 160;
}

void Game::Init()
{
  renderer.Init(screenW, screenH);
  camera.Init(screenW, screenH);
  gamepad.Init();
  if (!TileCache::Init())
  {
    Serial.println("TileCache: PSRAM allocation failed — using PROGMEM fallback.");
  } else
  {
    Serial.println("TileCache: tiles cached to PSRAM.");
  }

  worldPos = {0, 0, 0, 0, 12, 12};
  InitAllMaps();
  currentMap = GetAreaMap(worldPos.zoneX, worldPos.zoneY, worldPos.areaX, worldPos.areaY);
  usingInteriorMap = false;
  // set player's world tile pos
  player.pos.tileX = worldPos.tileX;
  player.pos.tileY = worldPos.tileY;
  player.x = player.pos.tileX * TILE_SIZE;
  player.y = player.pos.tileY * TILE_SIZE;
  player.posClean();
  exploreMode.Init(currentMap);
}


void Game::Update()
{
  gamepad.update();
  switch (currentState)
  {
    case GS_BEGIN:
      handleBegin();
      break;

    case GS_EXPLORE:
      // Explore is responsible for overworld walking & entering interiors
      exploreMode.Update(player, worldPos, currentMap, usingInteriorMap, currentState);
      break;

    case GS_DUNGEON:
      //Dungeons and their different floors are handled here
      dungeonMode.Update(player, worldPos, currentMap, usingInteriorMap, currentState);
      break;

    case GS_COMBAT:
      handleCombat();
      break;

    case GS_TOWN:
      //Towns, homes, shops, and friendly castles handled here
      townMode.Update(player, worldPos, currentMap, usingInteriorMap, currentState);
      break;

    default:
      break;
  }
  // Always update camera to follow player inside the current area bounds
  if (currentMap)
  {
    camera.FollowSmooth(player.x, player.y, currentMap->width * TILE_SIZE, currentMap->height * TILE_SIZE);
  }
}

void Game::Render()
{
  renderer.BeginFrame();
  switch (currentState)
  {
    case GS_EXPLORE:
      exploreMode.Render(currentMap, renderer, camera, player);
      break;
    case GS_DUNGEON:
      dungeonMode.Render(currentMap, renderer, camera, player);
      break;
    case GS_COMBAT:
      renderCombat();
      break;
    case GS_TOWN:
      townMode.Render(currentMap, renderer, camera, player);
      break;
    default:
      renderBegin();
      break;
  }
  renderer.EndFrame();
}

// small placeholders to be factored into appropriate state classes - TO DO
void Game::handleBegin() {}
void Game::handleCombat() {}
void Game::renderBegin() {}
void Game::renderCombat() {}
