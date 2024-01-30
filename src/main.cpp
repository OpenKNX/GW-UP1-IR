#include <Arduino.h>
#include "OpenKNX.h"
#include "IrCodeModule.h"
#include "FileTransferModule.h"

void setup()
{
	const uint8_t firmwareRevision = 0;
    openknx.init(firmwareRevision);
    openknx.addModule(1, openknxIrCodeModule);
    openknx.addModule(2, openknxFileTransferModule);
    openknx.setup();
}

void loop()
{
    openknx.loop();
}