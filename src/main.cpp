#include <Arduino.h>
#include "OpenKNX.h"
#include "IrCodeModule.h"

void setup()
{
	const uint8_t firmwareRevision = 0;
    openknx.init(firmwareRevision);
    openknx.addModule(1, new IrCodeModule());
    openknx.setup();
}

void loop()
{
    openknx.loop();
}