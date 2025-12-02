#pragma once
#include "Configs.h"

class Renderer
{
  public:
    Renderer();
    ~Renderer();

    void Init(int width, int height);
    void BeginFrame();
    void EndFrame();

    LGFX_Sprite* GetLayerBackground();
    LGFX_Sprite* GetLayerEntities();
    LGFX_Sprite* GetLayerUI();
    LGFX_Sprite* GetLayerDialog();

  private:
    LGFX_Sprite frame;
    int width = 0;
    int height = 0;
};
