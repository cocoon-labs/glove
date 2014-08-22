#define PIN_LIGHTS 0
#define NUM_LIGHTS 24

#define NUM_MODES 1 // number of program modes
#define M_GRAD 0

#define NUM_SCHEMES 13 // number of color schemes
#define S_RAINBOW 0
#define S_CRISPY 1
#define S_SNOWSKIRT 2
#define S_ROYAL 3
#define S_GLEAM 4
#define S_FIRE 5
#define S_SPORKLE 6
#define S_DORK 7
#define S_KELPY 8
#define S_JUNGLE 9
#define S_AMERICA 10
#define S_GOBLIN 11
#define S_SEVENS 12

#include <Adafruit_NeoPixel.h>

using namespace std;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_LIGHTS, NEO_GRB + NEO_KHZ400);

uint8_t mode = 0;   // Program mode selected
uint8_t scheme = S_ROYAL; // Color scheme selected

uint8_t shiftBy = 20;
uint8_t wheelPos = 0;
uint8_t updateInterval = 200; // Milliseconds between color updates

uint8_t chance = 100;
uint8_t fadeFactor = 3;
uint8_t fadePos = 0;
uint8_t fadeDir = 0;

void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'.
  randomSeed(analogRead(0));
  //Serial.begin(9600);
  delay(10);
}

void loop()
{
  stepPattern();
  delay(updateInterval);
}

void stepPattern() {
  randomize();
  switch (mode) {
    case M_GRAD: // Gradient fade
      gradient();
      break;
  }
}

void randomize () {
  if (!random(chance)) {
    mode = random(NUM_MODES);
  }
  if (!random(chance )) {
    scheme = random(NUM_SCHEMES);
  }
  if (!random(chance)) {
    shiftBy = random(60);
  }
  if (!random(chance)) {
    updateInterval = 20 + random(200);
  }
  if (!random(chance)) {
    fadeFactor = 2 + random(5);
  }
  if (!random(chance)) {
    fadeDir = random(3);
  }
}

uint32_t fadeFactory(uint32_t color) {
  uint32_t r = (color >> 16) * 2 / fadeFactor;
  uint32_t g = ((color >> 8) & 255) * 2 / fadeFactor;
  uint32_t b = (color & 255) * 2 / fadeFactor;
  return (r << 16) | (g << 8) | b;
}  

// Fill all the dots with a color.
void gradient() {
  uint32_t color;
  uint8_t i;
  int j;

  for(i=0 ; i<NUM_LIGHTS ; i++) {
    strip.setPixelColor(i, Wheel(wheelPos & 255));
  }
  
  for(i=0 ; i<NUM_LIGHTS ; i++) {
    j = (fadePos - i) / (NUM_LIGHTS / 4);
    j = abs(j);
    color = strip.getPixelColor(i);
    for( ; j>0 ; j--) {
      color = fadeFactory(color);
    }
    strip.setPixelColor(i, color);
  }
  if(fadeDir == 0) fadePos = (fadePos + 1) % NUM_LIGHTS;
  else if(fadeDir == 1) fadePos = (fadePos + NUM_LIGHTS - 1) % NUM_LIGHTS;
  
  strip.show();
  wheelPos += shiftBy;
}

// Input a value 0 to 255 to get a color value.
uint32_t Wheel(byte WheelPos) {
  // Use selected color scheme.
  switch (scheme) {
    case S_RAINBOW: // Rainbow
      return generalWheel(WheelPos, strip.Color(255,0,0), strip.Color(0,255,0), strip.Color(0,0,255));
    case S_CRISPY: //
      return generalWheel(WheelPos, strip.Color(2,255,222), strip.Color(229,149,217), strip.Color(210,255,0));
    case S_SNOWSKIRT: //
      return generalWheel(WheelPos, strip.Color(229,255,46), strip.Color(240,23,0), strip.Color(147,0,131));
    case S_ROYAL: //
      return generalWheel(WheelPos, strip.Color(0,0,0), strip.Color(128,0,255), strip.Color(128,0,128));
    case S_GLEAM: //
      return generalWheel(WheelPos, strip.Color(0,0,0), strip.Color(23,7,0), strip.Color(85,60,0));
    case S_FIRE: //
      return generalWheel(WheelPos, strip.Color(255,0,0), strip.Color(255,100,0), strip.Color(232,120,0));
    case S_SPORKLE: //
      return generalWheel(WheelPos, strip.Color(0,0,0), strip.Color(80,0,230), strip.Color(0,0,0));
    case S_DORK: //
      return generalWheel(WheelPos, strip.Color(0,0,0), strip.Color(196,0,255), strip.Color(209,209,209));
    case S_KELPY: //
      return generalWheel(WheelPos, strip.Color(0,0,133), strip.Color(0,196,255), strip.Color(0,240,44));
    case S_JUNGLE: //
        return generalWheel(WheelPos, strip.Color(214,185,0), strip.Color(0,255,17), strip.Color(60,138,80));
    case S_AMERICA: //
      return generalWheel(WheelPos, strip.Color(255,0,0), strip.Color(209,209,209), strip.Color(0,0,255));
    case S_GOBLIN: //
      return generalWheel(WheelPos, strip.Color(0,255,0), strip.Color(52,146,52), strip.Color(17,181,119));
    case S_SEVENS: //
      return generalWheel(WheelPos, strip.Color(117,0,177), strip.Color(77,17,71), strip.Color(247,77,7));
  }
}

uint32_t generalWheel(byte WheelPos, uint32_t C0, uint32_t C1, uint32_t C2) {
  uint8_t r0 = C0 >> 16;
  uint8_t g0 = (C0 >> 8) & 255;
  uint8_t b0 = C0 & 255;
  uint8_t r1 = C1 >> 16;
  uint8_t g1 = (C1 >> 8) & 255;
  uint8_t b1 = C1 & 255;
  uint8_t r2 = C2 >> 16;
  uint8_t g2 = (C2 >> 8) & 255;
  uint8_t b2 = C2 & 255;
  
  if(WheelPos < 85) {
   return strip.Color(r0 + (WheelPos * (r1 - r0) / 85), g0 + (WheelPos * (g1 - g0) / 85), b0 + (WheelPos * (b1 - b0) / 85));
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(r1 + (WheelPos * (r2 - r1) / 85), g1 + (WheelPos * (g2 - g1) / 85), b1 + (WheelPos * (b2 - b1) / 85));
  } else {
   WheelPos -= 170;
   return strip.Color(r2 + (WheelPos * (r0 - r2) / 85), g2 + (WheelPos * (g0 - g2) / 85), b2 + (WheelPos * (b0 - b2) / 85));
  }
}
