#include <FastLED.h>

#define LED_PIN  3

#define COLOR_ORDER GRB


#define CHIPSET     WS2811

#define BRIGHTNESS 64

CRGB branco = CRGB ( 255, 255, 255);

CRGB vermelho = CRGB ( 255, 0, 0);                        

const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 8;


const bool    kMatrixSerpentineLayout = false;
const bool    kMatrixVertical = false;


uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if( kMatrixSerpentineLayout == false) {
    if (kMatrixVertical == false) {
      i = (y * kMatrixWidth) + x;
    } else {
      i = kMatrixHeight * (kMatrixWidth - (x+1))+y;
    }
  }

  if( kMatrixSerpentineLayout == true) {
    if (kMatrixVertical == false) {
      if( y & 0x01) {
        // Odd rows run backwards
        uint8_t reverseX = (kMatrixWidth - 1) - x;
        i = (y * kMatrixWidth) + reverseX;
      } else {
        // Even rows run forwards
        i = (y * kMatrixWidth) + x;
      }
    } else { // vertical positioning
      if ( x & 0x01) {
        i = kMatrixHeight * (kMatrixWidth - (x+1))+y;
      } else {
        i = kMatrixHeight * (kMatrixWidth - x) - (y+1);
      }
    }
  }

  return i;
}

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);

uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if( x >= kMatrixWidth) return -1;
  if( y >= kMatrixHeight) return -1;
  return XY(x,y);
}


void loop()
{
    
    for( int x = 0; x < 8; x++){
      for( int y = 0; y < 8; y++){
        uint8_t corazul = 255;
        uint8_t corred = 5 * x * y + 10;
        uint8_t corverde = 5 * (7 - x) * (7 - y) + 10;
        leds[XY(x, y)] = CRGB ( corred, corverde, corazul);
      }
    }
    FastLED.show();
    delay(300);
    
     for( int x = 0; x < 8; x++){
      for( int y = 0; y < 8; y++){
        uint8_t corazul = 255;
        uint8_t corred = 5 * (7 - x) * y + 10;
        uint8_t corverde = 5 * x * (7 - y) + 10;
        leds[XY(x, y)] = CRGB ( corred, corverde, corazul);
      }
    }
    FastLED.show();
    delay(300);

    for( int x = 0; x < 8; x++){
      for( int y = 0; y < 8; y++){
        
        uint8_t corazul = 255;
        uint8_t corred = 5 * (7 - x) * (7 - y) + 10;
        uint8_t corverde = 5 * x * y + 10;
        leds[XY(x, y)] = CRGB ( corred, corverde, corazul);
      }
    }
    FastLED.show();
    delay(300);

    for( int x = 0; x < 8; x++){
      for( int y = 0; y < 8; y++){
        uint8_t corazul = 255;
        uint8_t corred = 5 * x * (7 - y) + 10;
        uint8_t corverde = 5 * (7 - x) * y + 10;
        leds[XY(x, y)] = CRGB ( corred, corverde, corazul);
      }
    }
    FastLED.show();
    delay(300);
    
}


void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
}
