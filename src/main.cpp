#include <Arduino.h>
#include <PicoOTA.h>
#include "OpenKNX.h"
#include "IrCodeModule.h"
#include "UpdateModule.h"
//#include "FTPServer.h"

/*
Used propertyIndexes:
IrCodeModule
 - 242 Learn/Delete Code
UpdateModule
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
    //openknx.addModule(3, new FtpServer());
    openknx.setup();

    //logHexInfo("Test", (uint8_t*)XIP_BASE + 0x00034664 - 10, 20);
}

void loop()
{
    openknx.loop();
}