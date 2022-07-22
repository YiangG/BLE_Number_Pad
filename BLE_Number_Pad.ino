//Author: Yiang Gong
//7/21/2022
//BLE Number Pad

#include "Adafruit_NeoTrellis.h"
#include <BleKeyboard.h>

Adafruit_NeoTrellis trellis;
BleKeyboard bleKeyboard;

// define a callback for key presses
TrellisCallback blink(keyEvent evt)
{
  // Check is the pad pressed?
  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
  {
    trellis.pixels.setPixelColor(evt.bit.NUM, Wheel(map(evt.bit.NUM, 0, trellis.pixels.numPixels(), 0, 255))); // on rising
    Serial.println(evt.bit.NUM);
    keypadAction(evt.bit.NUM);
  }
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING)
  {
    // or is the pad released?
    trellis.pixels.setPixelColor(evt.bit.NUM, 0); // off falling
  }

  // Turn on/off the neopixels!
  trellis.pixels.show();

  return 0;
}

void setup()
{
  Serial.begin(115200);
  // while(!Serial) delay(1);
  bleKeyboard.begin();

  if (!trellis.begin())
  {
    Serial.println("Could not start trellis, check wiring?");
    while (1)
      delay(1);
  }
  else
  {
    Serial.println("NeoPixel Trellis started");
  }

  // activate all keys and set callbacks
  for (int i = 0; i < NEO_TRELLIS_NUM_KEYS; i++)
  {
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING);
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_FALLING);
    trellis.registerCallback(i, blink);
  }

  // do a little animation to show we're on
  for (uint16_t i = 0; i < trellis.pixels.numPixels(); i++)
  {
    trellis.pixels.setPixelColor(i, Wheel(map(i, 0, trellis.pixels.numPixels(), 0, 255)));
    trellis.pixels.show();
    delay(50);
  }
  for (uint16_t i = 0; i < trellis.pixels.numPixels(); i++)
  {
    trellis.pixels.setPixelColor(i, 0x000000);
    trellis.pixels.show();
    delay(50);
  }
}

void loop()
{
  if (bleKeyboard.isConnected())
  {
    trellis.read(); // interrupt management does all the work! :)
  }
  delay(20); // the trellis has a resolution of around 60hz
}

/******************************************/

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85)
  {
    return trellis.pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return trellis.pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return trellis.pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}

void keypadAction(int key)
{
  switch (key)
  {
  case 0:
    bleKeyboard.write(KEY_NUM_7);
    break;
  case 1:
    bleKeyboard.write(KEY_NUM_8);
    break;
  case 2:
    bleKeyboard.write(KEY_NUM_9);
    break;
  case 3:
    bleKeyboard.write(KEY_NUM_ASTERISK);
    break;
  case 4:
    bleKeyboard.write(KEY_NUM_4);
    break;
  case 5:
    bleKeyboard.write(KEY_NUM_5);
    break;
  case 6:
    bleKeyboard.write(KEY_NUM_6);
    break;
  case 7:
    bleKeyboard.write(KEY_NUM_MINUS);
    break;
  case 8:
    bleKeyboard.write(KEY_NUM_1);
    break;
  case 9:
    bleKeyboard.write(KEY_NUM_2);
    break;
  case 10:
    bleKeyboard.write(KEY_NUM_3);
    break;
  case 11:
    bleKeyboard.write(KEY_NUM_PLUS);
    break;
  case 12:
    bleKeyboard.write(KEY_NUM_0);
    break;
  case 13:
    bleKeyboard.write(KEY_NUM_PERIOD);
    break;
  case 14:
    bleKeyboard.write(KEY_NUM_SLASH);
    break;
  case 15:
    bleKeyboard.write(KEY_NUM_ENTER);
    break;
  }
}
