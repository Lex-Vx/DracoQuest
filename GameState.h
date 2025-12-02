#pragma once

// Simple shared GameState enum to avoid circular includes
enum GameState {
  GS_BEGIN,
  GS_EXPLORE,
  GS_COMBAT,
  GS_TOWN,
  GS_DUNGEON,
  GS_DEAD
};
