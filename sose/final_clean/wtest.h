/*
Released under Creative Commons Attribution 4.0
by bitluni 2016
https://creativecommons.org/licenses/by/4.0/
Attribution means you can use it however you like as long you
mention that it's base on my stuff.
I'll be pleased if you'd do it by sharing http://youtube.com/bitlunislab
*/

#include "LedFunction.h"

class wtest: public LedFunction
{
  public:
  wtest()
  {
  }

  virtual void render()
  {
    int j = (millis() >> 7) & 255;
    for(int i = 0; i < state->count; i++)
    {
      int ij = 255 - ((i + j) & 255);
      if(ij < 63.75)
        state->setRgb(i, 255 - ij * 4, ij * 4, 0);
      else
      if(ij < 127.5)
        state->setRgb(i, (ij - 63.75) * 4, 255 - (ij - 63.75) * 4, 0);
      else
      if(ij < 191.25)
        state->setRgb(i, 255 - (ij -127.5) * 4, (ij -127.5) * 4, 0);
      else
        state->setRgb(i, (ij - 191.25) * 4, 255 - (ij - 191.25) * 4, 0);
    }
  }
};
