# TouchStrip – Hackers and Designers 2019

An LED strip that reacts to touch, made with a capacitive sensor and an Arduino.
We built it in the wearables workshop with Eric Overmeir at
[Hackers and Designers](https://hackersanddesigners.nl) 2019.

By Dasha Ilina, Chuck Kuan and Jonas Bo.

## Sketches

| Folder | What it does | State |
| --- | --- | --- |
| `hmmbrella/` | **Hmmmbrella (L.U.R.E.)**: a 60-LED strip. While you touch the sensor, the whole strip fades in to a random warm hue (orange to yellow). When you let go, it slowly fades out. Each new touch picks a new hue. This is the version we used for the video. | Works |
| `touchStrip_1/` | A 30-LED strip. Each touch lights one random dark LED in a random colour, which brightens while you hold it. After 3.3 s it jumps to full brightness and stays, then fades out slowly, so touches add up to a field of fading stars. Let go earlier and the LED fades out from where it was. | Compiles for the Uno, fixed in 2026 (see below), not yet tested on the strip |

Each folder also has `erics.ino`, the helper functions from the workshop
template: `colorLerp()` (mixes two colours), `smooth()` (an exponential filter
for the sensor value) and `setLed(x, y, color)` (sets an LED in a grid of
strips). The Arduino IDE opens it as a second tab of the same sketch.

## Hardware

- An Arduino Uno
- A WS2812B / NeoPixel LED strip: 60 LEDs for `hmmbrella`, 30 for `touchStrip_1`
- A 10 MΩ resistor
- A wire, a piece of aluminium foil or conductive fabric to use as the touch pad
- A 5 V power supply strong enough for the strip (a full 60-LED strip can draw up to about 3.6 A at full white)

### Wiring

| From | To |
| --- | --- |
| LED strip DIN | Arduino pin **10** |
| LED strip 5V / GND | 5 V supply (and Uno GND) |
| 10 MΩ resistor | between pin **4** (send) and pin **2** (sensor) |
| Touch pad (wire / foil) | pin **2** |

## Software

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Install these libraries with the Library Manager:
   - [FastLED](https://github.com/FastLED/FastLED)
   - [CapacitiveSensor](https://github.com/PaulStoffregen/CapacitiveSensor)
3. Open `hmmbrella/hmmbrella.ino` (or `touchStrip_1/touchStrip_1.ino`), choose
   **Tools > Board > Arduino Uno** and upload it.
4. Open the Serial Monitor at 9600 baud. It prints `FIRST CONTACT` when you
   touch the pad and `LETTING GO` when you let go.

## Settings

These are at the top of each sketch:

| Name | Meaning | `hmmbrella` | `touchStrip_1` |
| --- | --- | --- | --- |
| `NUM_LEDS_STRIP` | LEDs on the strip | 60 | 30 |
| touch threshold | sensor value that counts as a touch | 300 (`val > 300` in `loop()`) | `TOUCH_ON` 300, released below `TOUCH_OFF` 100 |
| fade in | how fast a touch brightens | `fadeInAmt` 5 per loop | full brightness after `minDuration` |
| `fadeOutAmt` | brightness taken away per loop after letting go | 0.5 | 1 |
| `minDuration` | ms a touch must last to be added | not used | 3333 |
| hue of a new touch | FastLED hue 0–255 | `random(25, 64)`, warm | `random(256)`, any |

The sensor values depend on the size of the touch pad, the cable length and
the resistor. If the strip reacts without a touch, or not at all, uncomment
the `Serial.print` lines in `loop()`, watch the raw values and change the
threshold of 300.

## Fixes in `touchStrip_1` (2026)

The 2019 version of `touchStrip_1` never worked. These bugs were fixed:

- `findSpot()` never ended: `found == true;` compared instead of assigning, so
  the first touch hung the board. It now picks a random dark LED, or the
  dimmest one when all are lit.
- The touched LED never brightened: `fadeLeds(-1)` took away the same amount
  that the fade-in added each loop. The LED that is fading in is no longer
  faded out, and the fade-in is now timed, so it reaches full brightness after
  `minDuration`.
- The touched LED showed the colour of an old touch, then jumped to a new random
  colour when the touch was added. It now keeps the colour picked at first
  contact.
- After the first added touch the strip stopped reacting: letting go was only
  noticed during a touch that had not been added yet.
- `activeTouches` went down once per frame for every dark LED. It is now a
  function that counts the lit LEDs.
- `updateLeds()` passed hue and saturation to `setRGB()`, and `fillLeds()` read
  outside the `brights` array. Both are fixed.

## Thanks

Thanks to Eric Overmeir for the workshop and the code template, and to
Hackers and Designers for hosting it.
