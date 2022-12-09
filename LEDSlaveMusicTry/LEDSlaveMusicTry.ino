#include <Wire.h>
#include <FastLED.h>
#include <Key.h>
#include <Keypad.h>

//Notes
//Přepsat efekty s volitelnými barvami, aby se měnilo myHue + 30

//I2C
#define MasterAddress 1
char MasterData;

//Keypad
const byte rows = 1;
const byte cols = 4;
char keys[rows][cols] = {
  {'1', '2', '3', '4'},
};
byte rowPins[rows] = {7};
byte colPins[cols] = {5, 6, 3, 4};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols );

//LED
#define NUM_LEDS 240
#define LEDPin 2

//Loop
int Delay = 200;
int LoopDelay = 75;

//Keys
byte LEDMode = 1;
byte LEDColor = 2;
int LineBPM = 7.5;
byte LEDBrightness = 255;

int beatsinCorrection = -65536 / 4;
byte myHue = 0;

CHSV breathColor;
CHSV stillColor;
CHSV gradientFirst;
CHSV gradientSecond;

CRGB leds[NUM_LEDS];
void setup() {
  pinMode(LEDPin, OUTPUT);
  FastLED.addLeds<NEOPIXEL, LEDPin>(leds, NUM_LEDS);
  FastLED.setBrightness(LEDBrightness);
  FastLED.setTemperature(ClearBlueSky);
  FastLED.setCorrection(TypicalPixelString);
  Wire.begin(MasterAddress);
  Wire.onReceive(receiveEvent);
  ResetLED();
  Transition();
}
void loop () {
  char key = keypad.getKey();
  switch (key) {
    case '1': {
        LEDMode += 1;
        LEDColor = 2;
        if (LEDMode < 12) {
          LEDMode = 1;
        }
        ResetLED();
        Transition();
        break;
      }
    case '2': {
        LEDColor += 1;
        if (LEDColor > 9) {
          LEDColor = 1;
        }
        ResetLED();
        break;
      }
    case '3': {
        LineBPM += 1.5;
        if (LineBPM > 20) {
          LineBPM = 1.5;
        }
        break;
      }
    case '4': {
        LEDBrightness += 25;
        if (LEDBrightness > 250) {
          LEDBrightness = 25;
        }
        break;
      }
  }
  switch (LEDMode) {
    case 1: {
        RGBContinuous();
        break;
      }
    case 2: {
        Breath();
        break;
      }
    case 3: {
        RainbowPulse();
        break;
      }
    case 4: {
        Pulse();
        break;
      }
    case 5: {
        Juggle();
        break;
      }
    case 6: {
        LineOneColor();
        break;
      }
    case 7: {
        LineRainbow();
        break;
      }
    case 8: {
        LineCollision();
        break;
      }
    case 9: {
        StillRainbow();
        break;
      }
    case 10: {
        StillSolid();
        break;
      }
    case 11: {
        Gradient();
        break;
      }
    case 12: {
        Music();
        break;
      }
  }
  FastLED.show();
}
void ResetLED() {
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
}
void Transition() {
  int j = NUM_LEDS / 2;
  for (uint8_t i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
    leds[i] = CHSV(millis() / 10, 255, LEDBrightness);
    leds[j] = CHSV(millis() / 10, 255, LEDBrightness);
    j--;
    fadeToBlackBy(leds, NUM_LEDS, 4 * LineBPM);
    FastLED.show();
    delay(10);
  }
  delay(5000);
  ResetLED();
}
void receiveEvent(int howMany)
{
  while (Wire.available()) // loop through all but the last
  {
    MasterData = Wire.read();
    if (MasterData == '1') {
      LEDMode += 1;
      if (LEDMode > 11) {
        LEDMode = 1;
        Transition();
      }
    }
  }
}
/* EFFECTS */

void RGBContinuous() {
  EVERY_N_MILLISECONDS(LoopDelay) {
    fill_solid(leds, NUM_LEDS, CHSV(myHue, 255, LEDBrightness));
    FastLED.show();
    myHue++;
  }
}
void RGBMoving() {
  FastLED.setBrightness(LEDBrightness);
  fill_rainbow(leds, NUM_LEDS, millis() / (2 * LineBPM), 1);
}

void Breath() {
  float breath = beatsin16(LineBPM, 64, 255, 0, beatsinCorrection);
  float breathSpecial = beatsin16(LineBPM, 64, 190, 0, beatsinCorrection);
  CHSV breathWhite = CHSV(0, 0, breathSpecial);
  CHSV breathRed = CHSV(0, 255, breath);
  CHSV breathGreen = CHSV(85, 255, breath);
  CHSV breathBlue = CHSV (170, 255, breath);
  CHSV breathYellow = CHSV(42.5 , 255, breath);
  CHSV breathCyan = CHSV(127.5 , 255, breath);
  CHSV breathPurple = CHSV(205 , 255, breath);
  CHSV breathPink = CHSV(227, 255, breath);
  CHSV breathOrange = CHSV(15, 255, breath);
  switch (LEDColor) {
    case 1: {
        breathColor = breathWhite;
        break;
      }
    case 2: {
        breathColor = breathRed;
        break;
      }
    case 3: {
        breathColor = breathGreen;
        break;
      }
    case 4: {
        breathColor = breathBlue;
        break;
      }
    case 5: {
        breathColor = breathYellow;
        break;
      }
    case 6: {
        breathColor = breathCyan;
        break;
      }
    case 7: {
        breathColor = breathPurple;
        break;
      }
    case 8: {
        breathColor = breathPink;
        break;
      }
    case 9: {
        breathColor = breathOrange;
        break;
      }
  }
  fill_solid(leds, NUM_LEDS, breathColor);
}
void Pulse() {
  int j = NUM_LEDS / 2;
  for (uint8_t i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
    leds[i] = CHSV(millis() / 20, 255, LEDBrightness);
    leds[j] = CHSV(millis() / 20, 255, LEDBrightness);
    j--;
    fadeToBlackBy(leds, NUM_LEDS, 4 * LineBPM);
    FastLED.show();
    delay(20);
  }
}

void RainbowPulse()
{
  int pos = beatsin16(LineBPM, 0, NUM_LEDS - 1, 0, beatsinCorrection);
  leds[pos] = CHSV (myHue, 255, LEDBrightness);
  EVERY_N_MILLISECONDS(NUM_LEDS / 10) {
    myHue++;
  }
}

void Juggle() {
  byte juggleColorsNum = 8;
  FastLED.setBrightness(LEDBrightness);
  fadeToBlackBy(leds, NUM_LEDS, juggleColorsNum * (juggleColorsNum / 2));
  for (int i = 1; i <= juggleColorsNum; i++) {
    leds[beatsin16(i + 3 , 0, NUM_LEDS - 1, 0, beatsinCorrection )] = CHSV(i * 255 / 8 , 255, LEDBrightness);
  }
}
/*void Firework()
  {
  FastLED.setBrightness(LEDBrightness);
  EVERY_N_MILLISECONDS(random8(30, 70)) {
    fadeToBlackBy(leds, NUM_LEDS, 80);
    int pos = random16(0, NUM_LEDS);
    int fireworkColor = random16(0, 255);
    for (int i = 0; i < random(1, 6); i++) {
      leds[pos + i] += ColorFromPalette(jugglePalette, fireworkColor);
    }
  }
  }*/

void LineOneColor()
{
  CHSV stillWhite = CHSV(0, 0, LEDBrightness - 100);
  CHSV stillRed = CHSV(0, 255, LEDBrightness);
  CHSV stillGreen = CHSV(85, 255, LEDBrightness);
  CHSV stillBlue = CHSV (170, 255, LEDBrightness);
  CHSV stillYellow = CHSV(42.5 , 255, LEDBrightness);
  CHSV stillCyan = CHSV(127.5 , 255, LEDBrightness);
  CHSV stillPurple = CHSV(205 , 255, LEDBrightness);
  CHSV stillPink = CHSV(227, 255, LEDBrightness);
  CHSV stillOrange = CHSV(15, 255, LEDBrightness);
  switch (LEDColor) {
    case 1: {
        stillColor = stillWhite;
        break;
      }
    case 2: {
        stillColor = stillRed;
        break;
      }
    case 3: {
        stillColor = stillGreen;
        break;
      }
    case 4: {
        stillColor = stillBlue;
        break;
      }
    case 5: {
        stillColor = stillYellow;
        break;
      }
    case 6: {
        stillColor = stillCyan;
        break;
      }
    case 7: {
        stillColor = stillPurple;
        break;
      }
    case 8: {
        stillColor = stillPink;
        break;
      }
    case 9: {
        stillColor = stillOrange;
        break;
      }
    case 10: {
        LEDColor = 1;
        break;
      }
  }
  int pos = beatsin16(LineBPM, 0, NUM_LEDS - 1, 0, beatsinCorrection);
  fadeToBlackBy(leds, NUM_LEDS, 2 * LineBPM);
  leds[pos] = stillColor;
}

void LineRainbow()
{
  int pos = beatsin16(LineBPM, 0, NUM_LEDS - 1, 0, beatsinCorrection);
  fadeToBlackBy(leds, NUM_LEDS, 2 * LineBPM);
  leds[pos] = CHSV (myHue, 255, LEDBrightness);
  EVERY_N_MILLISECONDS(LineBPM * (NUM_LEDS / 100)) {
    myHue++;
  }
}

void LineCollision()
{
  int pos = beatsin16(LineBPM, 0, NUM_LEDS - 1, 0, beatsinCorrection);
  fadeToBlackBy(leds, NUM_LEDS, 2 * LineBPM);
  leds[pos] = CHSV (myHue, 255, LEDBrightness);
  if ((pos == NUM_LEDS - 1) || (pos == 0)) {
    if (LineBPM <= 5) {
      delay(LineBPM * LineBPM * LineBPM);
    }
    else {
      delay(LineBPM * (LineBPM - (LineBPM / 2)));
    }
    myHue = myHue + LineBPM;
  }
}

void Music() {
  while (Wire.available()) // loop through all but the last
  {
    MasterData = Wire.read();
    switch (MasterData) {
      case 'a': {
        Serial.println('a');
          int j = NUM_LEDS / 2;
          for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
            leds[i] = CHSV(0, 0, 0);
            leds[j] = CHSV(0, 0, 0);
            j--;
            FastLED.show();
          }
          delay(50);
          break;
        }
      case 'b': {
          int j = NUM_LEDS / 2;
          for (int i = NUM_LEDS / 2; i < NUM_LEDS - 105; i++) {
            leds[i] = CHSV(NUM_LEDS / 8, 255, LEDBrightness);
            leds[j] = CHSV(NUM_LEDS / 8, 255, LEDBrightness);
            j--;
            FastLED.show();
          }
          break;
        }
      case 'c': {
          int j = NUM_LEDS / 2;
          for (int i = NUM_LEDS / 2; i < NUM_LEDS - 90; i++) {
            leds[i] = CHSV(2 * NUM_LEDS / 8, 255, LEDBrightness);
            leds[j] = CHSV(2 * NUM_LEDS / 8, 255, LEDBrightness);
            j--;
            FastLED.show();
          }
          break;
        }
      case 'd': {
          int j = NUM_LEDS / 2;
          for (int i = NUM_LEDS / 2; i < NUM_LEDS - 75; i++) {
            leds[i] = CHSV(3 * NUM_LEDS / 8, 255, LEDBrightness);
            leds[j] = CHSV(3 * NUM_LEDS / 8, 255, LEDBrightness);
            j--;
            FastLED.show();
          }
          break;
        }
      case 'e': {
          int j = NUM_LEDS / 2;
          for (int i = NUM_LEDS / 2; i < NUM_LEDS - 60; i++) {
            leds[i] = CHSV(4 * NUM_LEDS / 8, 255, LEDBrightness);
            leds[j] = CHSV(4 * NUM_LEDS / 8, 255, LEDBrightness);
            j--;
            FastLED.show();
          }
          break;
        }
      case 'f': {
          int j = NUM_LEDS / 2;
          for (int i = NUM_LEDS / 2; i < NUM_LEDS - 45; i++) {
            leds[i] = CHSV(5 * NUM_LEDS / 8, 255, LEDBrightness);
            leds[j] = CHSV(5 * NUM_LEDS / 8, 255, LEDBrightness);
            j--;
            FastLED.show();
          }
          break;
        }
      case 'g': {
          int j = NUM_LEDS / 2;
          for (int i = NUM_LEDS / 2; i < NUM_LEDS - 30; i++) {
            leds[i] = CHSV(6 * NUM_LEDS / 8, 255, LEDBrightness);
            leds[j] = CHSV(6 * NUM_LEDS / 8, 255, LEDBrightness);
            j--;
            FastLED.show();
          }
          break;
        }
      case 'h': {
          int j = NUM_LEDS / 2;
          for (int i = NUM_LEDS / 2; i < NUM_LEDS - 15; i++) {
            leds[i] = CHSV(7 * NUM_LEDS / 8, 255, LEDBrightness);
            leds[j] = CHSV(7 * NUM_LEDS / 8, 255, LEDBrightness);
            j--;
            FastLED.show();
          }
          break;
        }
      case 'i': {
          int j = NUM_LEDS / 2;
          for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
            leds[i] = CHSV(NUM_LEDS, 255, LEDBrightness);
            leds[j] = CHSV(NUM_LEDS, 255, LEDBrightness);
            j--;
            FastLED.show();
          }
          break;
        }
    }
  }
}

/* MY STILL COLORS */
void StillRainbow()
{
  FastLED.setBrightness(LEDBrightness);
  fill_rainbow(leds, NUM_LEDS, 0, 255 / NUM_LEDS);
}

void StillSolid() {
  CHSV stillWhite = CHSV(0, 0, LEDBrightness - 50);
  CHSV stillRed = CHSV(0, 255, LEDBrightness);
  CHSV stillGreen = CHSV(85, 255, LEDBrightness);
  CHSV stillBlue = CHSV (170, 255, LEDBrightness);
  CHSV stillYellow = CHSV(42.5 , 255, LEDBrightness);
  CHSV stillCyan = CHSV(127.5 , 255, LEDBrightness);
  CHSV stillPurple = CHSV(205 , 255, LEDBrightness);
  CHSV stillPink = CHSV(227, 255, LEDBrightness);
  CHSV stillOrange = CHSV(15, 255, LEDBrightness);
  switch (LEDColor) {
    case 1: {
        stillColor = stillWhite;
        break;
      }
    case 2: {
        stillColor = stillRed;
        break;
      }
    case 3: {
        stillColor = stillGreen;
        break;
      }
    case 4: {
        stillColor = stillBlue;
        break;
      }
    case 5: {
        stillColor = stillYellow;
        break;
      }
    case 6: {
        stillColor = stillCyan;
        break;
      }
    case 7: {
        stillColor = stillPurple;
        break;
      }
    case 8: {
        stillColor = stillPink;
        break;
      }
    case 9: {
        stillColor = stillOrange;
        break;
      }
    case 10: {
        LEDColor = 1;
        break;
      }
  }
  fill_solid(leds, NUM_LEDS, stillColor);
}

/*MY GRADIENT COLORS*/

void Gradient() {
  CHSV gradientRed = CHSV(0, 255, LEDBrightness);
  CHSV gradientGreen = CHSV(85, 255, LEDBrightness);
  CHSV gradientBlue = CHSV(170, 255, LEDBrightness);
  switch (LEDColor) {
    case 1: {
        gradientFirst = gradientRed;
        gradientSecond = gradientGreen;
        break;
      }
    case 2: {
        gradientFirst = gradientRed;
        gradientSecond = gradientBlue;
        break;
      }
    case 3: {
        gradientFirst = gradientGreen;
        gradientSecond = gradientBlue;
        break;
      }
    case 4: {
        LEDColor = 1;
        break;
      }
  }
  fill_gradient_RGB(leds, 0, gradientFirst, NUM_LEDS - 1, gradientSecond);
}
