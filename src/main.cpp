#include <Arduino.h>
#include <PicoOTA.h>
#include "OpenKNX.h"
#include "IrCodeModule.h"
#include "UpdateModule.h"

/*
Used propertyIndexes:
 - 242 Learn/Delete Code
 - 243 Update start
 - 244 Update data
 - 245 Update end
*/

void setup()
{
    Serial2.setTX(4);
    Serial2.setRX(5);
	const uint8_t firmwareRevision = 0;
    openknx.init(firmwareRevision);
    openknx.addModule(1, new IrCodeModule());
    openknx.addModule(2, new UpdateModule());
    openknx.setup();

}

void loop()
{
    openknx.loop();
}