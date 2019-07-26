//the colorLerp function we can use to calculate a color between two colors given a number between 0-1
RGBColor colorLerp(RGBColor start, RGBColor end_, float frac) {
  RGBColor c = start;
  float dr = end_.r - start.r;
  float dg = end_.g - start.g;
  float db = end_.b - start.b;

  if (frac > 1.0) frac = 1.0;
  if (frac < 0.0) frac = 0.0;

  c.r += floor(dr * frac);
  c.g += floor(dg * frac);
  c.b += floor(db * frac);

  return c;
}


// returns the value smoothed with an exponential filter
float smooth(long rawVal)
{
  smoothVal = ((float)rawVal * SMOOTH_ALPHA) + (smoothVal * (1 - SMOOTH_ALPHA));
  return smoothVal;
}

//setLed is a function to set the color of the led by coordinate
void setLed(int x, int y, RGBColor color) {
  //we calculate the index of the led in the array leds
  //depending on the orientation of your strips you can swap these
  //int index = (x * NUM_LEDS_STRIP) + y;
  int index = (y * NUM_LEDS_STRIP) + x;
  leds[index].setRGB(color.r, color.g, color.b);
}
