#include <Arduino.h>
#include <PicoOTA.h>
#include "OpenKNX.h"
#include "IrCodeModule.h"
//#include "UpdaterModule.h"
#include "FtpServer.h"
#include "LittleFS.h"

void setup()
{
    Serial2.setTX(4);
    Serial2.setRX(5);
	const uint8_t firmwareRevision = 0;
    openknx.init(firmwareRevision);
    openknx.addModule(1, new IrCodeModule());
    //openknx.addModule(2, new UpdaterModule());
    openknx.addModule(3, new FtpServer());
    openknx.setup();
}

void loop()
{
    openknx.loop();
}