// Project made for Hackers and Designers 2019
// Wearables workshop with Eric Overmeir
// Dasha Ilina, Chuck Kuan & Jonas Bo


// TO DO
// Add at random place
// Fade present LEDs while adding new touch
// Improve
// Flicker present stars
// Build

//we import the library we will use for controlling our leds
#include <FastLED.h>

//we define constants for the number of strips, the number of leds per strip and the total number of leds
#define NUM_STRIPS 1
#define NUM_LEDS_STRIP 30
#define NUM_LEDS NUM_LEDS_STRIP * NUM_STRIPS

//we define an array that will carry all the colour values for our leds
CRGB leds[NUM_LEDS_STRIP * NUM_STRIPS];
int hues[NUM_LEDS_STRIP * NUM_STRIPS];
int brights[NUM_LEDS_STRIP * NUM_STRIPS];

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
int touchIndex;
int activeTouches = 0;
long touchStart;
int minDuration = 3333;
bool firstContact = false;
bool letGo = true;
int br = 0;
int rHue;
int SAT = 255;

float fadeInAmt = 1;



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
  for (int i = 0; i < NUM_LEDS; i++) { // Set all brightnesses to 0
    brights[i] = 0;
  }
}

void loop() {
  //the main loop
  //we first clear any previous frame in the leds array

  long start = millis();
  long val =  cs_4_2.capacitiveSensor(30);

  //  Serial.print(millis() - start);        // check on performance in milliseconds
  //  Serial.print("\t");                    // tab character for debug windown spacing
  //
  //  Serial.print(val);                     // print sensor output 1
  //  Serial.print("\t");
  //  Serial.println(smooth(val));            // print smoothed value


  if (val > 300) {
    if (firstContact == false && letGo == true) {
      touchStart = millis();
      firstContact = true;
      letGo = false;
      Serial.println("FIRST CONTACT");
      touchIndex = findSpot();
      rHue = random(255);
    }

    // FADING IN
    Serial.println("TOUCHING");
    brights[touchIndex] = min(brights[touchIndex] + fadeInAmt, 255);
    leds[touchIndex].setHSV(rHue, SAT, brights[touchIndex]);

    if (millis() - touchStart > minDuration) {
      Serial.println("ADD TOUCH");
      addTouch(touchIndex);
      activeTouches = activeTouches + 1;
      if (touchIndex > NUM_LEDS) touchIndex = 0;
      firstContact = false;
    }
  } else if (val < 100 && firstContact == true) {
    Serial.println("LETTING GO");
    firstContact = false;
    letGo = true;
  }

  //  clearLeds();

  updateLeds();
  fadeLeds(-1);

  FastLED.show();

  //the delay makes sure the current "frame" is visible for a specified amount of time
  delay(1);
}

int findSpot() {
  bool found = false;
  int randomL;
  if (activeTouches < NUM_LEDS) {
    while (!found) {
      randomL = random(NUM_LEDS);
      for (int i = 0; i < NUM_LEDS; i++) {
        if (brights[i] <= 0) {
          found == true;
          Serial.print("Found random spot: ");
          Serial.println(randomL);
        }
      }
    }
    return randomL;
  }


}

void addTouch(int i) {
  hues[i] = random(255);
  brights[i] = 255;
  leds[i].setHSV(hues[i], SAT, brights[i]);
}


//we use clearLeds to clear the screen and put all leds to black, this will enable us to make an animation
void clearLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
  }
}

void updateLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(hues[i], SAT, brights[i]);
  }
}

void fadeLeds(float fadeAmount) {
  for (int i = 0; i < NUM_LEDS; i++) {
    brights[i] = constrain(brights[i] + fadeAmount, 0, 255);
    //    Serial.println(brights[i]);
    leds[i].setHSV(hues[i], SAT, brights[i]);
    if (brights[i] <= 0) activeTouches--;
  }
}

void fillLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setHSV(brights[random(255)], SAT, 255);
  }
}
