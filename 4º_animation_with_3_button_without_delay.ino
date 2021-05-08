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

int tempoamais = 0;

unsigned long tempoAnterior = 0;  

const long intervalo = 200;  


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
    unsigned long tempoAtual = millis();

    for( int tempo = 0; tempo < 255; tempo = tempo + 8) {
      for( int x = 0; x < 8; x++){
        for( int y = 0; y < 8; y++){
          uint8_t cor = tempo - abs(cos(2 * PI * x / 7) + cos(2 * PI * y / 7)) * 127; 
          leds[XY(x, y)] = CHSV ( cor, 255, 255);
        }
      }
      int sensorPause = digitalRead(8);
      if (sensorPause == HIGH) {
       FastLED.show();
      } else {
        
      }
      int sensorVelocidadeMais = digitalRead(9);
      if (sensorVelocidadeMais == LOW) {
        if (tempoAtual - tempoAnterior >= intervalo) {
          tempoAnterior = tempoAtual;
          tempoamais = tempoamais + 1; 
          if (tempoamais > 33){
            tempoamais = 33;
          }
        }
      }
      int sensorVelocidadeMenos = digitalRead(10);
      if (sensorVelocidadeMenos == LOW) {
        if (tempoAtual - tempoAnterior >= intervalo) {
          tempoAnterior = tempoAtual;
          tempoamais = tempoamais - 1;
          if (tempoamais < 0){
            tempoamais = 0;
          }
        }
      }
      Serial.println(tempoamais);
      tempo = tempo + tempoamais;
  }
}


void setup() {
  pinMode(10, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
}
