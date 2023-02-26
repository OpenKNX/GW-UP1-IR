#include <Arduino.h>
#include "OpenKNX.h"
#include "LedModule.h"

void setup()
{
	const uint8_t firmwareRevision = 0;
    openknx.init(firmwareRevision);
    openknx.addModule(1, new LedModule());
    openknx.setup();
}

void loop()
{
    openknx.loop();
}