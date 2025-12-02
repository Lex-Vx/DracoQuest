#pragma once
#include "Configs.h"

#define BTN_BODY 0x08e5
#define BTN_TRIM 0xc54a
#define BKDRP 0x12345
#define TFT_GRAY  0x8410
#define TFT_BROWN 0xA145


struct Button 
{
  int type, x, y, w, h;
  String label;
  uint16_t baseColor;
  uint16_t currentColor;
  bool pressed;
  void draw(LGFX& tft);
  bool contains(int tx, int ty, int radius = 24);
  void press(LGFX& tft);
  void release(LGFX& tft);
};

class VirtualGamepad 
{
  public:
    VirtualGamepad(LGFX& display);
    void Init();
    void addButton(int type, int x, int y, int w, int h, const String& label, uint16_t color);
    void drawAll(); 
    void update();    // poll touch + update button states
    bool isPressed(const String& label);

  private:
    LGFX& tft;
    static const int MAX_BUTTONS = 8;
    Button buttons[MAX_BUTTONS];
    int numButtons = 0;
};

extern VirtualGamepad gamepad;