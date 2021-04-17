#include <FastLED.h>

#define LED_PIN  3

#define COLOR_ORDER GRB


#define CHIPSET     WS2811

#define BRIGHTNESS 64

CRGB branco = CRGB ( 255, 255, 255);

CRGB preto = CRGB ( 0, 0, 0);

CRGB verde = CRGB ( 0, 30, 0);

CRGB verde_claro = CRGB ( 100, 255, 100);

CRGB rosa = CRGB ( 255, 0, 255);

CRGB amarelo = CRGB ( 255, 255, 0);

CRGB vermelho = CRGB ( 255, 0, 0);

CRGB bege = CRGB ( 240, 150, 70);

CRGB amarelo_escuro = CRGB ( 85, 45, 0);

CRGB amarelo_mais_escuro = CRGB ( 40, 20, 0);

CRGB roxo_claro = CRGB ( 109, 0, 92 );

CRGB roxo = CRGB ( 23, 0, 28 );

uint8_t enderman[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  1, 2, 1, 0, 0, 1, 2, 1,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

uint8_t pikachu[] = {
  0, 2, 2, 0, 0, 0, 0, 2,
  0, 0, 3, 4, 0, 0, 0, 4,
  0, 0, 0, 3, 3, 3, 3, 4,
  4, 4, 0, 3, 2, 3, 3, 2,
  4, 4, 0, 6, 3, 3, 3, 4,
  0, 7, 0, 3, 4, 4, 4, 0,
  0, 7, 3, 4, 3, 4, 3, 0,
  0, 0, 3, 4, 7, 7, 4, 0
};

uint8_t pig[] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 4, 1, 1, 1, 1, 4, 0,
  1, 1, 2, 2, 2, 2, 1, 1,
  1, 1, 3, 2, 2, 3, 1, 1,
  1, 1, 2, 2, 2, 2, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1
};

uint8_t zombie[] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 2, 2, 1, 1,
  1, 2, 2, 2, 2, 2, 2, 1,
  2, 2, 2, 2, 2, 2, 2, 1,
  2, 0, 0, 2, 2, 0, 0, 2,
  1, 2, 2, 1, 1, 2, 2, 1,
  1, 2, 1, 2, 2, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 1, 1
};

uint8_t ovelha[] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 2, 2, 2, 2, 2, 2, 1,
  1, 0, 1, 2, 2, 1, 0, 1,
  1, 2, 2, 2, 2, 2, 2, 1,
  1, 1, 2, 3, 3, 2, 1, 1,
  1, 1, 2, 3, 3, 2, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1
};                          

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
    /*uint32_t ms = millis();
    int32_t yHueDelta32 = ((int32_t)cos16( ms * (27/1) ) * (350 / kMatrixWidth));
    int32_t xHueDelta32 = ((int32_t)cos16( ms * (39/1) ) * (310 / kMatrixHeight));
    DrawOneFrame( ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
    if( ms < 5000 ) {
      FastLED.setBrightness( scale8( BRIGHTNESS, (ms * 256) / 5000));
    } else {
      FastLED.setBrightness(BRIGHTNESS);
    }*/

    /*for( int led = 0; led < 4; led++) {
      leds[ vermelho[led]] = CRGB( 255, 0 ,0);
    }*/

    for( int led = 0; led < 64; led++){
      if ( zombie[led] == 0 ) {
        leds[ led ] = preto;
      }
      if ( zombie[led] == 1 ) {
        leds[ led ] = verde;
      }
      if ( zombie[led] == 2 ) {
        leds[ led ] = verde_claro;
      }
    }
    FastLED.show();

    delay(5000);

   for( int led = 0; led < 64; led++){
      if ( pig[led] == 0 ) {
        leds[ led ] = preto;
      }
      if ( pig[led] == 1 ) {
        leds[ led ] = rosa;
      }
      if ( pig[led] == 2 ) {
        leds[ led ] = amarelo;
      }
      if ( pig[led] == 3 ) {
        leds[ led ] = vermelho;
      }
      if ( pig[led] == 4 ) {
        leds[ led ] = branco;
      }
   }

   FastLED.show();

  delay(5000);
   
  for( int led = 0; led < 64; led++){
      if ( ovelha[led] == 0 ) {
          leds[ led ] = preto;
      }
      if ( ovelha[led] == 1 ) {
          leds[ led ] = branco;
      }
      if ( ovelha[led] == 2 ) {
          leds[ led ] = bege;
      }
      if ( ovelha[led] == 3 ) {
          leds[ led ] = rosa;
      }
  }
  
    FastLED.show();

    delay(5000);

    for( int led = 0; led < 64; led++){
      if ( pikachu[led] == 0 ) {
          leds[ led ] = branco;
      }
      if ( pikachu[led] == 2 ) {
          leds[ led ] = preto;
      }
      if ( pikachu[led] == 3 ) {
          leds[ led ] = amarelo;
      }
      if ( pikachu[led] == 4 ) {
          leds[ led ] = amarelo_escuro;
      }
      if ( pikachu[led] == 6 ) {
          leds[ led ] = vermelho;
      }
      if ( pikachu[led] == 7 ) {
          leds[ led ] = amarelo_mais_escuro;
      }
  }
  
    FastLED.show();

    delay(5000);

    for( int led = 0; led < 64; led++){
      if ( enderman[led] == 0 ) {
          leds[ led ] = preto;
      }
      if ( enderman[led] == 1 ) {
          leds[ led ] = roxo_claro;
      }
      if ( enderman[led] == 2 ) {
          leds[ led ] = roxo;
      }
  }
  
    FastLED.show();

    delay(5000);
}

void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;
  for( byte y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;     
    for( byte x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( 0, 0, 255);
    }
  }
}

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
}
