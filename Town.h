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

// Town handles interior town logic and render
class Town {
  public:
    Town() = default;
    // Update may modify currentMap, usingInteriorMap, and currentState (for exiting town)
    void Update(Player& player, WorldPosition& worldPos, const MapInfo*& currentMap, bool& usingInteriorMap, GameState& currentState);
    void Render(const MapInfo* currentMap, Renderer& renderer, Camera& camera, Player& player);
};
