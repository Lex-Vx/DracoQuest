#include "Game.h"
Game game;

void setup() 
{
  Serial.begin(115200);
  game.Init();
}

void loop() 
{
    game.Update();
    game.Render();
}

