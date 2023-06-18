#if 0
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_SPI pn532spi(SPI, 10);
NfcAdapter nfc = NfcAdapter(pn532spi);
#else

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
#endif

void setup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  Serial.begin(9600);
  Serial.println("NDEF Writer");
  nfc.begin();
  display.clearDisplay();
}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 29);            // Start at top-left corner
  display.println(F("Listo para Escribir!"));
  display.setCursor(0, 38);            // Start at top-left corner
  display.println(F("Coloca la tarjeta"));
  display.setCursor(64, 46);            // Start at top-left corner
  display.println(F("v"));
  display.setCursor(64, 56);            // Start at top-left corner
  display.println(F("V"));

  display.display();
  Serial.println("\nPlace a formatted Mifare Classic NFC tag on the reader.");
  if (nfc.tagPresent()) {
    NdefMessage message = NdefMessage();
    message.addUriRecord("https://www.facebook.com/Cronicasdeunmago");

    bool success = nfc.write(message);
    if (success) {
      display.clearDisplay();
      display.setTextSize(1);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(3, 32);            // Start at top-left corner
      display.println(F("Copiado Exitoso!"));
      display.display();
      Serial.println("Success. Try reading this tag with your phone.");
    } else {
      display.clearDisplay();
      display.setTextSize(1);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0, 32);            // Start at top-left corner
      display.println(F("Proceso Fallido!"));
      display.display();
      Serial.println("Write failed.");
    }
  }
  delay(5000);
}
