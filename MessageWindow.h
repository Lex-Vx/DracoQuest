#pragma once
#include <Arduino.h>
#include <vector>
#include "Configs.h"

class MessageWindow
{
public:
    MessageWindow()
        : visible(false),
          width(132),
          height(60),
          margin(6),
          innerPadding(6)
    {}

    void Show(const String& msg);
    void Hide();
    bool IsVisible() const { return visible; }
    void Update();
    void Draw(LGFX_Sprite* ui);

    bool AdvancePage();

private:
    bool visible;

    String fullText;
    String displayedText;
    String typewriterSource;

    unsigned long lastCharTime = 0;
    uint16_t charDelay = 25;

    int width;
    int height;
    int margin;
    int innerPadding;

    bool pagesBuilt = false;
    std::vector<String> pages;
    int currentPage = 0;
    bool pageComplete = false;

    String WrapTextToWidth(LGFX_Sprite* ui, const String& s, int maxWidth);
    void BuildPages(LGFX_Sprite* ui);
};
