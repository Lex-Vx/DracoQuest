#pragma once
#include "Player.h"
#include "WorldPosition.h"
#include "Maps.h"
#include "Camera.h"
#include "Renderer.h"
#include "GameState.h"
#include "Configs.h"
#include "Globals.h"
#include "TileMap.h"

// Explore handles overworld movement, area/zone crossing, and interior entry logic
class Explore {
  public:
    Explore() = default;
    void Init(const MapInfo* startMap);
    // Update may modify: currentMap, usingInteriorMap, currentState, and player's pos
    void Update(Player& player, WorldPosition& worldPos, const MapInfo*& currentMap, bool& usingInteriorMap, GameState& currentState);
    void Render(const MapInfo* currentMap, Renderer& renderer, Camera& camera, Player& player);
  
  private:
    void updatePlayerPosition(Player& player, const MapInfo* currentMap);
    void handleOverworldCrossing(Player& player, WorldPosition& worldPos, const MapInfo*& currentMap, bool& areaChanged);
    void handleInteriorEntry(Player& player, WorldPosition& worldPos, const MapInfo*& currentMap, bool& usingInteriorMap, GameState& currentState);
};
