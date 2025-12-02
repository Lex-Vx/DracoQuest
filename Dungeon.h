#pragma once
#include "Player.h"
#include "WorldPosition.h"
#include "Maps.h"
#include "Renderer.h"
#include "Camera.h"
#include "GameState.h"
#include "Configs.h"
#include "Globals.h"
#include "TileMap.h"
#include "Globals.h"

// Dungeon handles interior dungeon logic and render
class Dungeon {
  public:
    Dungeon() = default;
    // Update may modify currentMap, usingInteriorMap, and currentState (for exiting dungeon)
    void Update(Player& player, WorldPosition& worldPos, const MapInfo*& currentMap, bool& usingInteriorMap, GameState& currentState);
    void Render(const MapInfo* currentMap, Renderer& renderer, Camera& camera, Player& player);
};
