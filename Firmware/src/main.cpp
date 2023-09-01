#include <Arduino.h>
#include <PicoOTA.h>
#include "OpenKNX.h"
#include "IrCodeModule.h"
#include "FileTransferModule.h"

void setup()
{
	Serial2.setTX(20); //4);
	Serial2.setRX(21); //5);
	const uint8_t firmwareRevision = 0;
    openknx.init(firmwareRevision);
    openknx.addModule(1, new IrCodeModule());
    openknx.addModule(2, new FileTransferModule());
    openknx.setup();
}

void loop()
{
    openknx.loop();
}