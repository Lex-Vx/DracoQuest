#pragma once
#include <Arduino.h>


// The viewport of the game. This follows the player, clamped to the boundaries of the map being currently displayed.
struct Camera
{
    float pixelX = 0.f;
    float pixelY = 0.f;
    int width  = 160;
    int height = 160;

    void Init(int vw, int vh)
    {
        width = vw;
        height = vh;
    }

    void FollowSmooth(float targetX, float targetY, int areaPixelW, int areaPixelH, float smooth = 0.15f)
    {
        float goalX = targetX - width  * 0.5f;
        float goalY = targetY - height * 0.5f;
        // clamp goal
        if (goalX < 0) goalX = 0;
        if (goalY < 0) goalY = 0;
        float maxX = areaPixelW - width;
        float maxY = areaPixelH - height;
        if (maxX < 0) maxX = 0;
        if (maxY < 0) maxY = 0;
        if (goalX > maxX) goalX = maxX;
        if (goalY > maxY) goalY = maxY;
        // smooth approach (exponential interpolation)
        pixelX += (goalX - pixelX) * smooth;
        pixelY += (goalY - pixelY) * smooth;
    }
};
