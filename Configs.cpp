
#include "Configs.h"



LGFX tft;
esp_expander::CH422G expander(I2C_NUM_1, I2C_CH422G_ADDRESS, TOUCH_SCL, TOUCH_SDA);

void InitSD() 
{
  Serial.begin(115200);
  Serial.println("Hello from ESP32-S3!");
  expander.init();
  expander.begin();
  expander.multiPinMode(TP_RST | LCD_BL | LCD_RST | SD_CS | USB_SEL, OUTPUT);
  // Set backlight HIGH manually
  expander.digitalWrite(LCD_BL, HIGH);
  expander.digitalWrite(TP_RST | LCD_RST | USB_SEL, HIGH);
  // SD card init
  expander.digitalWrite(SD_CS, LOW); // select SD via EXIO
  SPI.setHwCs(false);
  SPI.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS)) 
  {
    Serial.println("SD card mount failed!");
  } else 
    {
      Serial.println("SD card initialized OK.");
      uint8_t cardType = SD.cardType();
      Serial.print("SD card type: ");
      if (cardType == CARD_NONE) Serial.println("None");
      else if (cardType == CARD_SD) Serial.println("SDSC");
      else if (cardType == CARD_SDHC) Serial.println("SDHC");
      else Serial.println("Unknown");
  }

}
