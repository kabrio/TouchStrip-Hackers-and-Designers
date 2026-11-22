# TouchStrip – Hackers and Designers 2019

An LED strip that reacts to touch, made with a capacitive sensor and an Arduino.
We built it in the wearables workshop with Eric Overmeir at
[Hackers and Designers](https://hackersanddesigners.nl) 2019.

By Dasha Ilina, Chuck Kuan and Jonas Bo.

## Sketches

| Folder | What it does | State |
| --- | --- | --- |
| `hmmbrella/` | **Hmmmbrella (L.U.R.E.)**: a 60-LED strip. While you touch the sensor, the whole strip fades in to a random warm hue (orange to yellow). When you let go, it slowly fades out. Each new touch picks a new hue. This is the version we used for the video. | Works |
| `touchStrip_1/` | A 30-LED strip. Each touch should light one random LED. Hold the touch for about 3.3 s and the LED stays lit, then fades out slowly, so touches add up to a field of fading stars. | Unfinished, see below |

Each folder also has `erics.ino`, the helper functions from the workshop
template: `colorLerp()` (mixes two colours), `smooth()` (an exponential filter
for the sensor value) and `setLed(x, y, color)` (sets an LED in a grid of
strips). The Arduino IDE opens it as a second tab of the same sketch.

## Hardware

- An Arduino board
- A WS2812B / NeoPixel LED strip: 60 LEDs for `hmmbrella`, 30 for `touchStrip_1`
- A 10 MΩ resistor
- A wire, a piece of aluminium foil or conductive fabric to use as the touch pad
- A 5 V power supply strong enough for the strip (a full 60-LED strip can draw up to about 3.6 A at full white)

### Wiring

| From | To |
| --- | --- |
| LED strip DIN | Arduino pin **10** |
| LED strip 5V / GND | 5 V supply (and Arduino GND) |
| 10 MΩ resistor | between pin **4** (send) and pin **2** (sensor) |
| Touch pad (wire / foil) | pin **2** |

## Software

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Install these libraries with the Library Manager:
   - [FastLED](https://github.com/FastLED/FastLED)
   - [CapacitiveSensor](https://github.com/PaulStoffregen/CapacitiveSensor)
3. Open `hmmbrella/hmmbrella.ino` (or `touchStrip_1/touchStrip_1.ino`) and upload it.
4. Open the Serial Monitor at 9600 baud. It prints `FIRST CONTACT` when you
   touch the pad and `LETTING GO` when you let go.

## Settings

These are at the top of each sketch:

| Name | Meaning | `hmmbrella` | `touchStrip_1` |
| --- | --- | --- | --- |
| `NUM_LEDS_STRIP` | LEDs on the strip | 60 | 30 |
| `val > 300` (in `loop()`) | sensor value that counts as a touch | 300 | 300 (released below 100) |
| `fadeInAmt` | brightness added per loop while touching | 5 | 1 |
| `fadeOutAmt` | brightness taken away per loop after letting go | 0.5 | 1 (`fadeLeds(-1)`) |
| `minDuration` | ms a touch must last | 200 | 3333 |
| `rHue = random(25, 64)` | hue range of a new touch (FastLED hue 0–255) | 25–64, warm | 0–255 |

The sensor values depend on the size of the touch pad, the cable length and
the resistor. If the strip reacts without a touch, or not at all, uncomment
the `Serial.print` lines in `loop()`, watch the raw values and change the
threshold of 300.

## Known issues in `touchStrip_1`

- `findSpot()` never ends: `found == true;` compares instead of assigning, so
  the first touch hangs the board. It should be `found = true;`, and the check
  should test `brights[randomL]` rather than loop over all LEDs.
- `updateLeds()` passes hue and saturation to `setRGB()`. `fadeLeds()` sets the
  right colour right after, so this has no visible effect, but it can be removed.
- `activeTouches` goes down once per frame for every dark LED, so it soon
  becomes negative.

## Thanks

Thanks to Eric Overmeir for the workshop and the code template, and to
Hackers and Designers for hosting it.
