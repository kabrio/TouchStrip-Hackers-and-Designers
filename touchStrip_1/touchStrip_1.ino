//we import the library we will use for controlling our leds
#include <FastLED.h>

//we define constants for the number of strips, the number of leds per strip and the total number of leds
#define NUM_STRIPS 1
#define NUM_LEDS_STRIP 30
#define NUM_LEDS NUM_LEDS_STRIP * NUM_STRIPS

//we define an array that will carry all the colour values for our leds
CRGB leds[NUM_LEDS_STRIP * NUM_STRIPS];

//we create a datatype to store rgb color values in
struct RGBColor {
  int r;
  int g;
  int b;
};

#include <CapacitiveSensor.h>

#define SMOOTH_ALPHA 0.4

float smoothVal = 0;
CapacitiveSensor   cs_4_2 = CapacitiveSensor(4, 2);       // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
int touchIndex = 0;
long touchStart;
int minDuration = 3333;
bool firstContact = false;
bool letGo = true;
int br = 0;
int rHue;



void setup() {
  //  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  Serial.begin(9600);

  //we make sure that the library knows where all our strips and leds are on the arduino
  FastLED.addLeds<NEOPIXEL, 10>(leds, 0, NUM_LEDS_STRIP);
  //  FastLED.addLeds<NEOPIXEL, 7>(leds, NUM_LEDS_STRIP, NUM_LEDS_STRIP);
  //  FastLED.addLeds<NEOPIXEL, 8>(leds, 2 * NUM_LEDS_STRIP, NUM_LEDS_STRIP);
  //  FastLED.addLeds<NEOPIXEL, 9>(leds, 3 * NUM_LEDS_STRIP, NUM_LEDS_STRIP);
  //  FastLED.addLeds<NEOPIXEL, 10>(leds, 4 * NUM_LEDS_STRIP, NUM_LEDS_STRIP);
  //  FastLED.addLeds<NEOPIXEL, 11>(leds, 5 * NUM_LEDS_STRIP, NUM_LEDS_STRIP);
  clearLeds();
  FastLED.show();

  randomSeed(analogRead(0));
  //  fillLeds();
}

void loop() {
  //the main loop
  //we first clear any previous frame in the leds array

  long start = millis();
  long val =  cs_4_2.capacitiveSensor(30);

//  Serial.print(millis() - start);        // check on performance in milliseconds
//  Serial.print("\t");                    // tab character for debug windown spacing
//
  Serial.print(val);                     // print sensor output 1
  Serial.print("\t");
 Serial.println(smooth(val));            // print smoothed value


  if (val > 300) {
    if (firstContact == false && letGo == true) {
      touchStart = millis();
      firstContact = true;
      letGo = false;
      Serial.println("FIRST CONTACT");
      rHue = random(255);
      br = 0;
    }
    Serial.println("TOUCHING");
    br = min(br + 10, 255);
    leds[touchIndex].setHSV(rHue, 255, br);
    if (millis() - touchStart > minDuration) {
      Serial.println("ADD TOUCH");
      addTouch(touchIndex);
      touchIndex = touchIndex + 1;
      if (touchIndex > NUM_LEDS) touchIndex = 0;
      firstContact = false;
    }
  } else if (val < 100) {
    firstContact = false;
    letGo = true;
  }

  //  clearLeds();

  showLeds();
  fadeLeds(1);

  FastLED.show();

  //the delay makes sure the current "frame" is visible for a specified amount of time
  delay(100);
}

void addTouch(int i) {
  leds[i].setHSV(random(255), 255, 255);
}


//we use clearLeds to clear the screen and put all leds to black, this will enable us to make an animation
void clearLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
  }
}

//we use clearLeds to clear the screen and put all leds to black, this will enable us to make an animation
void showLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    RGBColor c = {0, 0, 0};
    c.r = leds[i].red; c.g = leds[i].green; c.b = leds[i].blue;
    leds[i].setRGB(c.r, c.g, c.b);
  }
}

void fadeLeds(int fadeAmount) {
  for (int i = 0; i < NUM_LEDS; i++) {
    int r, g, b;
    r = leds[i].red; g = leds[i].green; b = leds[i].blue;
    r = max(r - fadeAmount, 0);
    g = max(g - fadeAmount, 0);
    b = max(b - fadeAmount, 0);
    //    Serial.println(r);
    leds[i].setRGB(r, g, b);
  }
}

void fillLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(round(random(255)), random(255), random(255));
  }
}
