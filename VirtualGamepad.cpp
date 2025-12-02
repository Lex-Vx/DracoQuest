#include "VirtualGamepad.h"

VirtualGamepad gamepad(tft);
VirtualGamepad::VirtualGamepad(LGFX& display) : tft(display) 
{}

//This is all pretty simple stuff to make the controller UI, I'll add a prettier skin to this eventually


void Button::draw(LGFX& tft) 
{
  tft.setFreeFont(&FreeMono9pt7b);
  tft.fillRoundRect(x, y, w, h, 8, currentColor);
  tft.drawRoundRect(x+1, y+1, w-1, h-1, 4, BTN_TRIM);
  tft.setTextDatum(middle_center);
  tft.setTextColor(TFT_BLACK);
  if (type == 1) 
  {
    tft.drawString(label, x + w/2, y + h/2);
  }
}

bool Button::contains(int tx, int ty, int radius) 
{
  return (tx >= x - radius && tx <= x + w + radius &&
          ty >= y - radius && ty <= y + h + radius);
}

void Button::press(LGFX& tft) 
{
  if (!pressed) 
  {
    uint8_t r = (baseColor >> 11) & 0x1F;
    uint8_t g = (baseColor >> 5) & 0x3F;
    uint8_t b = baseColor & 0x1F;
    r = (r * 70) / 100;
    g = (g * 70) / 100;
    b = (b * 70) / 100;
    currentColor = (r << 11) | (g << 5) | b;
    pressed = true;
    draw(tft);
  }
}

void Button::release(LGFX& tft) 
{
  if (pressed) 
  {
    currentColor = baseColor;
    pressed = false;
    draw(tft);
  }
}

void VirtualGamepad::addButton(int type, int x, int y, int w, int h, const String& label, uint16_t color) 
{
  if (numButtons < MAX_BUTTONS) 
  {
    buttons[numButtons] = {type, x, y, w, h, label, color, color, false};
    numButtons++;
  }
}

void VirtualGamepad::Init() 
{
  addButton(0, 700, 232, 90, 90, "A", BTN_BODY);
  addButton(0, 590, 232, 90, 90, "B", BTN_BODY);
  addButton(0, 75, 192, 70, 55, "UP", BTN_BODY);
  addButton(0, 10, 252, 70, 55, "LEFT", BTN_BODY);
  addButton(0, 75, 317, 70, 55, "DOWN", BTN_BODY);
  addButton(0, 140, 252, 70, 55, "RIGHT", BTN_BODY);
  addButton(0, 260, 410, 90, 40, "SELECT", BTN_BODY);
  addButton(0, 450, 410, 90, 40, "START", BTN_BODY);
  drawAll();
}

void VirtualGamepad::drawAll() 
{
  for (int i = 0; i < numButtons; i++) 
  {
    buttons[i].draw(tft);
  }
}

bool VirtualGamepad::isPressed(const String& label) 
{
  for (int i = 0; i < numButtons; i++) 
  {
    if (buttons[i].label == label) 
    {
      return buttons[i].pressed;
    }
  }
  return false;
}

void VirtualGamepad::update() 
{
  // track if each button was touched this frame
  bool touched[MAX_BUTTONS] = {false};
  // multi-touch: GT911 supports up to 5 points
  for (int id = 0; id < 5; id++) 
  {
    int tx, ty;
    if (tft.getTouch(&tx, &ty, id)) 
    {
      for (int i = 0; i < numButtons; i++) 
      {
        if (buttons[i].contains(tx, ty, 24)) 
        {
          touched[i] = true;
          buttons[i].press(tft);
        }
      }
    }
  }

  // release untouched buttons
  for (int i = 0; i < numButtons; i++) 
  {
    if (!touched[i]) 
    {
      buttons[i].release(tft);
    }
  }
}
