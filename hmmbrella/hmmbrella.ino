// Project made for Hackers and Designers 2019
// Wearables workshop with Eric Overmeir
// Dasha Ilina, Chuck Kuan & Jonas Bo


// TO DO
// Add ToDo list

//we import the library we will use for controlling our leds
#include <FastLED.h>

//we define constants for the number of strips, the number of leds per strip and the total number of leds
#define NUM_STRIPS 1
#define NUM_LEDS_STRIP 60
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
int minDuration = 200;
bool firstContact = false;
bool letGo = true;
int br = 0;
int rHue;
float fadeInAmt = 5, fadeOutAmt = 5;



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

  //  long start = millis();
  long val =  cs_4_2.capacitiveSensor(30);

  //  Serial.print(millis() - start);        // check on performance in milliseconds
  //  Serial.print("\t");                    // tab character for debug windown spacing
  //
  //  Serial.print(val);                     // print sensor output 1
  //  Serial.print("\t");
  //  Serial.println(smooth(val));            // print smoothed value

  if (val > 300) { // set random color for each new touch
    if (firstContact == false && letGo == true) {
      touchStart = millis();
      firstContact = true;
      letGo = false;
      Serial.println("FIRST CONTACT");
      rHue = random(25,64);
      //      br = 0;
    }

    // FADE IN
    if (br < 255-fadeInAmt) br += fadeInAmt;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].setHSV(rHue, 255, br);
//      Serial.println(br);
    }
    FastLED.show();

  } else if (val < 300) {
    Serial.println("LETTING GO");
    firstContact = false;
    letGo = true;

    // FADE OUT
    if (br > 0) br -= fadeOutAmt;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].setHSV(rHue, 255, br);
//      Serial.println(br);

    }
    FastLED.show();
  }

  //the delay makes sure the current "frame" is visible for a specified amount of time
//  delay(2);
}

void addTouch(int i) {
  leds[i].setHSV(random(0,64), 255, 255);
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

void fadeLeds(float fadeAmount) {
  for (int i = 0; i < NUM_LEDS; i++) {
    float r, g, b;
    r = leds[i].red; g = leds[i].green; b = leds[i].blue;
    r = constrain(r + fadeAmount, 0, 255);
    g = constrain(g + fadeAmount, 0, 255);
    b = constrain(b + fadeAmount, 0, 255);
    //    Serial.println(r);
    leds[i].setRGB(r, g, b);
  }
}

void fillLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(round(random(255)), random(255), random(255));
  }
}
