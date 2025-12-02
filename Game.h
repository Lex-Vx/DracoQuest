#pragma once
#include "Renderer.h"
#include "Player.h"
#include "Camera.h"
#include "WorldPosition.h"
#include "Maps.h"
#include "VirtualGamepad.h"
#include "Configs.h"
#include "Globals.h"
#include "GameState.h"
#include "Explore.h"
#include "Town.h"
#include "Dungeon.h"

class Game 
{
  public:
    Game();
    void Init();
    void Update();
    void Render();
  private:
    // placeholders
    void handleBegin();
    void handleCombat();
    void renderBegin();
    void renderCombat();

    // state
    GameState currentState = GS_EXPLORE;

    // core systems
    Renderer renderer;
    Player player;
    Camera camera;
    int screenW = 240;
    int screenH = 240;

    // world and map
    WorldPosition worldPos;
    const MapInfo* currentMap = nullptr;
    bool usingInteriorMap = false;

    // mode handlers
    Explore exploreMode;
    Town townMode;
    Dungeon dungeonMode;
};
