#include "Renderer.h"

Renderer::Renderer() : frame(&tft)
{
}

Renderer::~Renderer()
{
  frame.deleteSprite();
}

void Renderer::Init(int w, int h)
{
  width = w;
  height = h;
  tft.init();
  tft.setRotation(0);
  frame.setPsram(true);                // put buffer in PSRAM
  frame.setTextColor(TFT_WHITE);       // default text color
  frame.setFreeFont(&FreeMonoBold9pt7b);
  frame.createSprite(width, height);
  frame.setTextDatum(top_left);
  frame.setSwapBytes(true);
  // ensure the sprite exists
  if (!frame.getBuffer()) 
  {
    Serial.println("Renderer::Init - failed to allocate frame sprite.");
  }
}


void Renderer::BeginFrame()
{
  frame.fillSprite(TFT_TRANSPARENT);
}

//nearest-neighbor upscale, gives a 2:1 upscaling
void Renderer::EndFrame()
{
  const int SCALE = 2; // set 2, 3, etc.

  // source sprite (your framebuffer)
  const int srcW = frame.width();
  const int srcH = frame.height();

  // scaled dimensions
  const int dstW = srcW * SCALE;
  const int dstH = srcH * SCALE;

  // create a temporary output sprite in PSRAM
  LGFX_Sprite out(&tft);
  out.setPsram(true);
  if (!out.createSprite(dstW, dstH)) {
    Serial.println("Renderer::EndFrame - failed to create scaled sprite");
    frame.pushSprite(0, 0); // best-effort fallback
    return;
  }

  // get raw RGB565 buffers
  // getBuffer() returns the internal buffer pointer
  uint16_t *srcBuf = (uint16_t*)frame.getBuffer();
  uint16_t *dstBuf = (uint16_t*)out.getBuffer();

  // temporary row buffer 
  uint16_t *rowBuf = new uint16_t[dstW];
  if (!rowBuf) {
    Serial.println("Renderer::EndFrame - rowBuf alloc failed");
    out.deleteSprite();
    frame.pushSprite(0, 0);
    return;
  }

  // For each source row, expand horizontally into rowBuf, then replicate vertically SCALE times.
  for (int sy = 0; sy < srcH; sy++) {
    uint16_t *srcRow = srcBuf + sy * srcW;

    // Build the scaled row
    int dx = 0;
    for (int sx = 0; sx < srcW; sx++) {
      uint16_t px = srcRow[sx];
      for (int k = 0; k < SCALE; k++) {
        rowBuf[dx++] = px;
      }
    }
    int dstRowIndex = (sy * SCALE) * dstW;
    for (int vy = 0; vy < SCALE; vy++) {
      // copy rowBuf into dstBuf + dstRowIndex
      memcpy(dstBuf + dstRowIndex, rowBuf, dstW * sizeof(uint16_t));
      dstRowIndex += dstW;
    }
  }

  // free temp row
  delete[] rowBuf;

  // now push the scaled sprite to the real display
  int posX = 240;
  int posY = 60;
  out.pushSprite(posX, posY);

  // clean up
  out.deleteSprite();
}



LGFX_Sprite* Renderer::GetLayerBackground() { return &frame; }
LGFX_Sprite* Renderer::GetLayerEntities()  { return &frame; }
LGFX_Sprite* Renderer::GetLayerUI()        { return &frame; }
LGFX_Sprite* Renderer::GetLayerDialog()     { return &frame; }
