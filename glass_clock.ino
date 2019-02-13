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

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define MESSAGE_HEADER "X" // Header tag for color messages
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

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


uint8_t text_columns[12];

void shift_text_columns(uint8_t* buf, uint8_t len, uint8_t next)
{
  for(uint8_t i=0; i<len-1; i++)
  {
    buf[i] = buf[i+1];
  }
  buf[len-1] = next;
}

void draw_text_columns(uint8_t* buf, uint8_t n_cols, uint8_t n_rows, CRGB* color)
{
  // TODO: implement me
}

void display_message(String* msg)
{
  // TODO: parse color
  // TODO: while next int in String
  //  push to end of text (shift)
  //  display
  //  wait
  // for i in range(12):
  // shift 0
  // display
  //  wait
}

 
void setup() {
    Serial.begin(9600);
    while (!Serial) ; // Needed for Leonardo only
    setSyncProvider( requestSync);  //set function to call when sync required
    Serial.println("Waiting for sync message");
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    //setTime(14,28,0,23,1,2019);
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
 
 void loop(){    
  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
  }
}


void parse_color(String* msg, int rgb_out[])
{
  String color= *msg;
  int Gdot = color.indexOf('.');
  int Bdot = color.substring(Gdot+1).indexOf('.')+Gdot+1;
  int G = color.substring(0,Gdot).toInt();
  int R = color.substring(Gdot+1, Bdot).toInt();
  int B = color.substring(Bdot+1).toInt();
  rgb_out[0] = R;
  rgb_out[1] = G;
  rgb_out[2] = B;
}
 
 void colorBlink(String msg){
    //on LEDs used Green and Red are swapped!
    String color=msg.substring(1);
    int Gdot = color.indexOf('.');
    int Bdot = color.substring(Gdot+1).indexOf('.')+Gdot+1;
    int G = color.substring(0,Gdot).toInt();
    int R = color.substring(Gdot+1, Bdot).toInt();
    int B = color.substring(Bdot+1).toInt();
    for(int n=0; n<10; n++)
    {
      for (int i=0; i<n_steps_pulse; i++)
            {
              for(int led = 0; led<72; led++) {
                leds[led] = CRGB( R * brightness_sec[i], G * brightness_sec[i], B * brightness_sec[i]);
              }
              delay(1000/n_steps_pulse);
              FastLED.show();
            }
    }
    for(int led = 0; led<72; led++) {
                leds[led] = CRGB::Black;
              }
              FastLED.show();
  }
 
 void digitalClockDisplay() {
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
          
          for (int i=0; i<n_steps_pulse; i++)
          {
            leds[mincoords[minute()]] = CRGB( 255 * brightness_sec[i], 255 * brightness_sec[i], 255 * brightness_sec[i]);
            delay(1000/n_steps_pulse);
            FastLED.show();
          }
          
          }
        count_down = 1;
        }
      
      }

 void processSyncMessage() {
  unsigned long pctime;
  String msg;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
  msg = Serial.readString();
  
  if(msg.startsWith(MESSAGE_HEADER)) {
    colorBlink(msg);
    Serial.println(msg);
    }
    
  if(msg.startsWith(TIME_HEADER)) {
    pctime = msg.substring(1).toInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime);
       Serial.println(pctime);// Sync Arduino clock to the time received on the serial port
        
     }
    }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}
