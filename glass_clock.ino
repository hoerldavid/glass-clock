#include <FastLED.h>
#include <TimeLib.h>
#include <math.h>

#define LED_PIN     5
#define NUM_LEDS    72
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.



CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;



const int n_steps_pulse = 50;
float brightness_sec[n_steps_pulse];

void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    setTime(14,28,0,23,1,2019);
    hourFormat12();
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

    // precompute sine wave for pulsing second led
    for (int i=0; i<n_steps_pulse; i++)
    {
      brightness_sec[i] = sin(M_PI * ((float) i / n_steps_pulse));
    }
    
}




int count_down = 0;
int mincoords[] = {67, 68, 69, 70, 71, 
                62, 63, 64, 65, 66,
                57, 58, 59, 60, 61,
                52, 53, 54, 55, 56,
                47, 48, 49, 50, 51,
                42, 43, 44, 45, 46,
                37, 38, 39, 40, 41, 
                32, 33, 34, 35, 36,
                27, 28, 29, 30, 31,
                22, 23, 24, 25, 26,
                17, 18, 19, 20, 21,
                12, 13, 14, 15, 16};
 void loop() {
      if (minute()!=0){
        count_down = 0;
        for(int led = 0; led < minute(); led++) {
          leds[mincoords[led]] = CRGB::White;
        }
        
          if (hourFormat12() == 1){
            for(int led = 0; led < 12; led++) { 
              leds[led] = CRGB::Black;
            }
          }
          for(int led = 0; led < hourFormat12(); led++) { 
            leds[led] = CRGB( 100, 100, 255);
          }

          for (int i=0; i<n_steps_pulse; i++)
          {
            leds[mincoords[minute()]] = CRGB( 255 * brightness_sec[i], 255 * brightness_sec[i], 255 * brightness_sec[i]);
            delay(1000/n_steps_pulse);
            FastLED.show();
          }

          /*
           * leds[mincoords[minute()]] = CRGB::White;
          FastLED.show();
          delay(500);
          leds[mincoords[minute()]] = CRGB::Black;
          FastLED.show();
          delay(500);
          */
          
      }
      else {
        if (count_down == 0){
          for(int dot = 59; dot >= 0; dot--) { 
            leds[mincoords[dot]] = CRGB::White;
            FastLED.show();
            // clear this led for the next time around the loop
            leds[mincoords[dot]] = CRGB::Black;
            delay(50);
        }
        }
        else {
          leds[mincoords[minute()]] = CRGB::White;
          if (hourFormat12() == 1){
            for(int led = 0; led < 12; led++) { 
              leds[led] = CRGB::Black;
            }
          }
            for(int led = 0; led < hourFormat12(); led++) { 
              leds[led] = CRGB( 100, 100, 255);
            }
          FastLED.show();
          delay(500);
          leds[mincoords[minute()]] = CRGB::Black;
          FastLED.show();
          delay(500);
          }
        count_down = 1;
        }
      
      }
    
