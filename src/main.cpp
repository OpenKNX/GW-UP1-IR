#include <Arduino.h>
#include "IrInput.h"

//IrInput ir(D0, -1);
IrInput ir(D0,-1);
//test t;

void setup()
{
    Serial.begin(115200);

    delay(10000);
    Serial.println("Starte");

    ir.setup();
}

int counter = 0;
int lastMillis = 0;
bool pressed = false;

void loop()
{
    ir.loop();
}