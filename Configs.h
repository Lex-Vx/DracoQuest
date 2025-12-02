#pragma once

// I wouldn't be a C++/microcontroller programmer if I didn't have some big messy backend file that holds it all together, welcome to Configs.h
// This is where we do a boot sequence configuration of the board's LCD driver(st7262) and touch driver(gt911) pins, and the SD card reader pins.
// We also have to initialize the CH422g i2c pin expander here, because of a design choice by the manufacturers.
// After this, we configure all the pixel clock pulse timings etc.
// TLDR; this makes LovyanGFX usable for the rest of the game, and makes this board useable by LovyanGFX


#define LGFX_USE_V1
#include <Wire.h>
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <SD.h>
#include <SPI.h>
#include <esp_io_expander.hpp>

#define TFT_HOR_RES   800
#define TFT_VER_RES   480

#define TOUCH_SDA 8
#define TOUCH_SCL 9
#define TOUCH_INT 4
#define TOUCH_RST -1
#define SD_MOSI 11
#define SD_MISO 13
#define SD_CLK  12

// CH422G expander pins (EXIO0..EXIO7)
const uint8_t I2C_CH422G_ADDRESS = 0x24;
#define TP_RST 1
#define LCD_BL 2
#define LCD_RST 3
#define SD_CS 4
#define USB_SEL 5

void InitSD();
// ---------------------- Backlight via CH422G ----------------------
class IoExpandedBacklight : public lgfx::ILight 
{
  public:
    struct config_t 
    {
      int i2c_port = 0;     // 0 -> Wire, 1 -> Wire1
      uint8_t pin_bl = LCD_BL;
      uint8_t i2c_addr = I2C_CH422G_ADDRESS;
    };

  void config(const config_t& cfg) 
  {
    _cfg = cfg;
    _wire = (_cfg.i2c_port == 1) ? &Wire1 : &Wire;
  }

  bool init(uint8_t brightness) override 
  {
    setBrightness(brightness);
    return true;
  }

  void setBrightness(uint8_t brightness) override 
  {
    if (!_wire) return;
    uint8_t data = (brightness > 0) ? 0xFF : 0x00;
    _wire->beginTransmission(_cfg.i2c_addr);
    _wire->write(data);
    _wire->endTransmission();
  }

  private:
    config_t _cfg{};
    TwoWire* _wire = nullptr;
};
// -------------------------------------------------------------------

class LGFX : public lgfx::LGFX_Device 
{
  public:
    lgfx::Bus_RGB       _bus_instance;
    lgfx::Panel_RGB     _panel_instance;
    IoExpandedBacklight _light_instance;
    lgfx::Touch_GT911   _touch_instance;
    LGFX(void) 
    {
      { // Panel config
        auto cfg = _panel_instance.config();
        cfg.memory_width  = TFT_HOR_RES;
        cfg.memory_height = TFT_VER_RES;
        cfg.panel_width   = TFT_HOR_RES;
        cfg.panel_height  = TFT_VER_RES;
        cfg.offset_x = 0;
        cfg.offset_y = 0;
        _panel_instance.config(cfg);
      }

      { // Extra panel options
        auto cfg = _panel_instance.config_detail();
        cfg.use_psram = 1;
        _panel_instance.config_detail(cfg);
      }
      { // RGB bus
        auto cfg = _bus_instance.config();
        cfg.panel = &_panel_instance;
        cfg.pin_d0  = 14;
        cfg.pin_d1  = 38;
        cfg.pin_d2  = 18;
        cfg.pin_d3  = 17;
        cfg.pin_d4  = 10;
        cfg.pin_d5  = 39;
        cfg.pin_d6  = 0;
        cfg.pin_d7  = 45;
        cfg.pin_d8  = 48;
        cfg.pin_d9  = 47;
        cfg.pin_d10 = 21;
        cfg.pin_d11 = 1;
        cfg.pin_d12 = 2;
        cfg.pin_d13 = 42;
        cfg.pin_d14 = 41;
        cfg.pin_d15 = 40;
        cfg.pin_henable = 5;
        cfg.pin_vsync   = 3;
        cfg.pin_hsync   = 46;
        cfg.pin_pclk    = 7;
        cfg.freq_write = 12000000;

        cfg.hsync_polarity = 0;
        cfg.hsync_front_porch = 8;
        cfg.hsync_pulse_width = 4;
        cfg.hsync_back_porch = 8;

        cfg.vsync_polarity = 0;
        cfg.vsync_front_porch = 8;
        cfg.vsync_pulse_width = 4;
        cfg.vsync_back_porch = 8;

        cfg.pclk_active_neg = 1;
        cfg.de_idle_high = 0;
        cfg.pclk_idle_high = 0;
        _bus_instance.config(cfg);
        _bus_instance.config(cfg);
      }
      _panel_instance.setBus(&_bus_instance);

      { // Touch
        auto cfg = _touch_instance.config();
        cfg.x_min = 0;
        cfg.x_max = TFT_HOR_RES - 1;
        cfg.y_min = 0;
        cfg.y_max = TFT_VER_RES - 1;
        cfg.pin_int = TOUCH_INT;
        cfg.pin_rst = TOUCH_RST;
        cfg.bus_shared = false;
        cfg.offset_rotation = 0;
        cfg.i2c_port = 1; 
        cfg.pin_sda  = TOUCH_SDA;
        cfg.pin_scl  = TOUCH_SCL;
        cfg.freq     = 400000;
        cfg.i2c_addr = 0x14;
        _touch_instance.config(cfg);
        _panel_instance.setTouch(&_touch_instance);
      }
      { // Backlight
        IoExpandedBacklight::config_t blcfg;
        blcfg.i2c_port = 1;
        blcfg.pin_bl   = LCD_BL;
        blcfg.i2c_addr = I2C_CH422G_ADDRESS;
        _light_instance.config(blcfg);
        _panel_instance.setLight(&_light_instance);
      }
      setPanel(&_panel_instance);
    }
};
extern LGFX tft;
extern esp_expander::CH422G expander;