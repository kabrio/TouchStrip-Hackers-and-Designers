// Project made for Hackers and Designers 2019
// Wearables workshop with Eric Overmeir
// Dasha Ilina, Chuck Kuan & Jonas Bo
//
// Every touch lights one random dark LED. It brightens while you hold the
// touch. After minDuration ms it is "added" at full brightness and then fades
// out slowly, so touches add up to a field of fading stars. Let go earlier and
// the LED fades out from where it was.
//
// Board: Arduino Uno

// TO DO
// Flicker present stars
// Build

//we import the library we will use for controlling our leds
#include <FastLED.h>

//we define constants for the number of strips, the number of leds per strip and the total number of leds
#define NUM_STRIPS 1
#define NUM_LEDS_STRIP 30
#define NUM_LEDS (NUM_LEDS_STRIP * NUM_STRIPS)

//we define an array that will carry all the colour values for our leds
CRGB leds[NUM_LEDS];
int hues[NUM_LEDS];
float brights[NUM_LEDS];

//we create a datatype to store rgb color values in
struct RGBColor {
  int r;
  int g;
  int b;
};

#include <CapacitiveSensor.h>

#define SMOOTH_ALPHA 0.4

float smoothVal = 0;
CapacitiveSensor cs_4_2 = CapacitiveSensor(4, 2);       // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

#define TOUCH_ON 300    // sensor value above this counts as a touch
#define TOUCH_OFF 100   // sensor value below this counts as letting go

int touchIndex = -1;            // LED of the current touch, -1 = none
long touchStart;
unsigned long minDuration = 3333; // ms a touch must last to be added
bool touching = false;          // a touch is fading in right now
bool letGo = true;              // the hand left the sensor since the last touch
int SAT = 255;
float fadeOutAmt = 1;           // brightness taken away per loop



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

  randomSeed(analogRead(0));
  for (int i = 0; i < NUM_LEDS; i++) { // all LEDs start dark
    hues[i] = 0;
    brights[i] = 0;
  }
  clearLeds();
  FastLED.show();
}

void loop() {
  long val = cs_4_2.capacitiveSensor(30);

  //  Serial.print(val);                     // print sensor output
  //  Serial.print("\t");
  //  Serial.println(smooth(val));            // print smoothed value

  if (val > TOUCH_ON) {
    if (!touching && letGo) {
      // a new touch: pick a dark LED and a colour for it
      touchIndex = findSpot();
      touching = true;
      letGo = false;
      touchStart = millis();
      hues[touchIndex] = random(256);
      Serial.print("FIRST CONTACT, LED ");
      Serial.println(touchIndex);
    }

    if (touching) {
      // FADING IN: reaches full brightness after minDuration
      unsigned long held = millis() - touchStart;
      float target = 255.0 * held / minDuration;
      if (target > brights[touchIndex]) brights[touchIndex] = min(target, 255.0);

      if (held >= minDuration) {
        Serial.println("ADD TOUCH");
        addTouch(touchIndex);
        touching = false;   // the next touch needs a new contact
      }
    }
  } else if (val < TOUCH_OFF && !letGo) {
    Serial.println("LETTING GO");
    touching = false;
    letGo = true;
  }

  fadeLeds(-fadeOutAmt);
  showLeds();
  FastLED.show();

  //the delay makes sure the current "frame" is visible for a specified amount of time
  delay(1);
}

// returns a random dark LED, or the dimmest LED when none is dark
int findSpot() {
  int dark = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    if (brights[i] <= 0) dark++;
  }

  if (dark == 0) {
    int dimmest = 0;
    for (int i = 1; i < NUM_LEDS; i++) {
      if (brights[i] < brights[dimmest]) dimmest = i;
    }
    return dimmest;
  }

  // take the n-th dark LED, n chosen at random
  int n = random(dark);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (brights[i] <= 0) {
      if (n == 0) return i;
      n--;
    }
  }
  return 0; // not reached
}

// number of LEDs that are still lit
int activeTouches() {
  int count = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    if (brights[i] > 0) count++;
  }
  return count;
}

void addTouch(int i) {
  brights[i] = 255; // keeps the hue it faded in with
}


//we use clearLeds to clear the screen and put all leds to black, this will enable us to make an animation
void clearLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
  }
}

// copies hues and brightnesses to the LEDs
void showLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setHSV(hues[i], SAT, (uint8_t)brights[i]);
  }
}

// fades every LED except the one that is fading in right now
void fadeLeds(float fadeAmount) {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (touching && i == touchIndex) continue;
    brights[i] = constrain(brights[i] + fadeAmount, 0, 255);
  }
}

// fills the strip with random colours at full brightness (for testing)
void fillLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    hues[i] = random(256);
    brights[i] = 255;
  }
}
