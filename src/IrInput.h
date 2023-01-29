#pragma once

#include "Segment.h"

class IrInput
{
  private:
    int btnUp;
    int btnDown;
    int counter;
    int state;
    int pressedUp;
    int pressedDown;
    int pressed;
    Segment *seg;

  public:
    IrInput(int up, int down);

    void setup();
    void loop();
};