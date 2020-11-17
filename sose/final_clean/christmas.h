/*
Released under Creative Commons Attribution 4.0
by bitluni 2016
https://creativecommons.org/licenses/by/4.0/
Attribution means you can use it however you like as long you
mention that it's base on my stuff.
I'll be pleased if you'd do it by sharing http://youtube.com/bitlunislab
*/

#include "LedFunction.h"

class christmas: public LedFunction
{
  public:
  christmas()
  {    
  }

  virtual void render()
  {
    int j = (millis() >> 4) & 255;
    for(int i = 0; i < state->count; i++)
    {
      int ij = 255 - ((i + j) & 255);
      if(ij < 127.5)
        state->setRgb(i, 255, 0, 0);
      else
        state->setRgb(i, 0, 255, 0);
    }
  }
};
