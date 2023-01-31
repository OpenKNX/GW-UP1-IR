#include "IrInput.h"
#include "hardware.h"

IrInput::IrInput(int up, int down)
{
    btnUp = up;
    btnDown = down;
    seg = new Segment(4, false);
}

uint8_t pinsIn[] = { D3, D4, D5, D6 };
uint8_t pinsDig[] = { D7, D8, D9, D10 };

void IrInput::setup()
{
    seg->setInputPins(pinsIn);
    seg->setDigitPins(pinsDig);

    if(btnUp >= 0) pinMode(btnUp, INPUT);
    if(btnDown >= 0) pinMode(btnDown, INPUT);

    seg->setDigits(-1);
    //seg->setNumber(1);
}

void IrInput::loop()
{
    seg->loop();

    switch(state)
    {
        case 0:
        {
            if(digitalRead(btnUp))
            {
                pressedUp = true;
            }
            if(!digitalRead(btnUp) && pressedUp)
            {
                counter = 1;
                seg->setNumber(counter, false);
                state = 1;
                pressedUp = false;
                Serial.print("Counter: ");
                Serial.println(counter);
                Serial.println("Go to State 1");

            }
            break;
        }

        case 1:
        {
            if(digitalRead(btnUp) && !pressedDown && !pressedUp)
            {
                pressedUp = true;
                pressed = millis();
            }
            if(btnDown >= 0 && digitalRead(btnDown) && !pressedDown && !pressedUp)
            {
                pressedDown = true;
                pressed = millis();
            }
            if(pressedUp && counter != 0)
            {
                if(pressedUp && !digitalRead(btnUp))
                {
                    pressedUp = false;
                    counter++;
                    seg->setNumber(counter, false);
                    Serial.println("Count up");
                }
                if(millis() - pressed > 2000)
                {
                    pressedUp = false;
                    state = 2;
                    Serial.println("Go to State 2");
                }
            }
            if(btnDown >= 0 && !digitalRead(btnDown) && pressedDown)
            {
                pressedDown = false;
                counter--;
                if(counter == 0)
                {
                    seg->setDigits(-1);
                    state = 0;
                    Serial.println("Go to State 0");
                } else {
                    seg->setNumber(counter, false);
                    Serial.println("Count down");
                }
            }
            break;
        }

        case 2:
        {
            if(!pressedUp)
            {
                pressedUp = true;
                seg->setDigit(-2, 3);
            }
            break;
        }
    }
}