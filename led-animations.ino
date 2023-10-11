#define FASTLED_FORCE_SOFTWARE_SPI
#include "FastLED.h"

int gridX = 6;
int gridY = 6;

// How many leds in your strip?
#define NUM_LEDS 12

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN

#define DATA_PIN 3
#define CLOCK_PIN 4

// Define the array of leds
CRGB leds[NUM_LEDS];

int delayer = 180;
int colorCycle = 0;

//

int CORNER = 0;
float a = 0;
float _90 = PI / 2;

void setup() { 
     
      Serial.begin(9600);
      
      FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      //FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      
      clearAll();
      
      //leds[0] = 0xffffff;
      //leds[1] = 0x0000ff;
      //leds[2] = 0x00ff00;
      //leds[3] = 0xff0000;
      
      //FastLED.show();
      
      //plot(5,5, 0xffffff,1);
      //plot(5,0, 0xffffff,1);
      //plot(0,5, 0xffffff,1);
      //plot(0,0, 0xffffff,1);
      
      //Serial.println(CRGB::Green);
      
      //line2(0,0,5,5, 0x707070, 1);
}



int frame = 0;

void loop() { 
  //plot2(2, 2, 0xff0000, 1);
  //chad4();
  //chad2();
  uglyAnimation();
}

float dd = 0;
void chad4()
{
  clearAll();
  
  dd += 1;
  for(int y = 0; y < 6; y++)
  {
    int x = cos(dd + y * (PI / 3)) * 2;
    float intensity = abs(cos(dd / 10)) * 0x7e;
    int r = 0x7f + intensity;
    int g = 0x7f + intensity;
    int b = 0x7f + intensity;
    
    if(y == 0)
    Serial.println(r);
    
    plot2(3 - x, y, (r << 16) | (g << 8 ) | b, 1);
  }
  
  delay(delayer);
}

void chad3()
{
  if(frame++ < 40)
    chad();
    else if(frame < 100)
      chad2();
      else
        frame = 0;
}

void chad2()
{
  clearAll();
  
  dd += 1;
  for(int y = 0; y < 6; y++)
  {
    int x = cos(dd + y * (PI / 3)) * 2;
    plot2(3 - x, y, 0x7f0020, 1);
  }
  
  delay(delayer);
}

void chad()
{
 clearAll();
  
  float x1 = 0, y1 = 0;
  float x2 = cos(a) * 7;
  float y2 = sin(a) * 7;
  x2 = (int)(x2 > 5 ? 5 : x2);
  y2 = (int)(y2 > 5 ? 5 : y2);
  
  int red = abs(cos(a * 4)) * 255;
  int green = abs(sin(a * 4)) * 255;
  int blue = 0xff;
  int color = (red << 16 ) | (green << 8) | blue;
  
  Serial.print(red);
  Serial.print(", ");
  Serial.println(color, HEX);

  a += _90 / 10;
  if(a > _90)
  {
    a = 0;
    CORNER --;
    if(CORNER == -1)
      CORNER = 3;
  }
    
    line(x1, y1, x2, y2, color, 1);
    delay(delayer);
}

void plot2(int x, int y, int color, int showbit)
{
  switch (CORNER)
  {
    case 1:
      x = 5 - ((y % 2) ? 5 - x : x);
      break;
    case 3:
      x = 5 - ((y % 2) ? 5 - x : x);
      y = 5 - y;
      break;
    case 2:
      x = (y % 2) ? 5 - x : x;
      y = 5 - y;
      break;
    default:
      x = (y % 2) ? 5 - x : x;
  }
    
  int i = y * 6 + x;
  
  leds[i] = color;
  if (showbit) FastLED.show();
}

void line2(int x1, int y1, int x2, int y2, int color, int showbit) 
{
  float dx = x2 - x1;
  float sx = abs(dx) / dx;
  float dy = y2 - y1;
  
  int len = (int)dx;
  
  for(int i = 0; i <= len; i++)
  {
    float x = (float)i * sx + .0;
    float y = dy * (x / dx) + .0;
    plot2(x1 + (int)x, (int)y, color, 0);
    Serial.print(x);
  }
  
  if (showbit) FastLED.show();
}

void uglyAnimation() {
 
  for (int i = 0; i < NUM_LEDS ; i++) {
    
    if (i == 0) clearAll();
    
    if (colorCycle > 2) colorCycle = 0;
    if (colorCycle == 0) leds[i] = CRGB::Red;
    if (colorCycle == 1) leds[i] = CRGB( 155, 55,100);
    if (colorCycle == 2) leds[i] = CRGB::Blue;
        
    //if (i > 0) leds[i-1] = CRGB::Black;
    //if (i == 0) leds[NUM_LEDS-1] = CRGB::Black;
    
    if (i == NUM_LEDS-1) colorCycle++;
    FastLED.show();
    delay(delayer);
    
  }
  
  
}

void clearAll() {
  
  for (int i = 0; i < NUM_LEDS ; i++) {
    
    leds[i] = CRGB::Black;
       
  }
    
  FastLED.show();
}

void plot(int x, int y, int color, int showbit) {
  
  int i = (y % 2) ? ((y * gridY) + (gridX-1) - x) : ((y * gridY) + x);
  
  //Serial.println(i);
  
  leds[i] = color;
  if (showbit) FastLED.show();
  
}

void line(int x1, int y1, int x2, int y2, int color, int showbit) {
  
  int minx, miny, maxx, maxy, axis, ii;
  
  if (x1 < x2) { minx = x1; maxx = x2; } else { minx = x2; maxx = x1; }
  if (y1 < y2) { miny = y1; maxy = y2; } else { miny = y2; maxy = y1; }
  
  if ((maxx- minx) > (maxy- miny)) axis = 0; else axis = 1;
  
  if (axis == 0) {
   
    for (int i = minx; i <= maxx; i++) {
      
      ii = (maxy * i)/maxx;   
      plot2(i,ii, color, 0);
      
      
    } 
    
  } else {
    
     for (int i = miny; i <= maxy; i++) {
                 
       ii = (maxx * i)/maxy;        
       plot2(ii,i, color, 0);
        

    } 
    
    
  }
  
  if (showbit) FastLED.show();
  
}
