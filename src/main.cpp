#include <Arduino.h>
#include <PicoOTA.h>
#include "OpenKNX.h"
#include "IrCodeModule.h"
//#include "UpdaterModule.h"
#include "FtpServer.h"
#include "LittleFS.h"

/*
Used propertyIndexes:
IrCodeModule
 - 242 Learn/Delete Code
UpdateModule
 - 243 Update start
 - 244 Update data
 - 245 Update end
*/

void listAllFilesInDir(String dir_path)
{
	Dir dir = LittleFS.openDir(dir_path);
	while(dir.next()) {
		if (dir.isFile()) {
			// print file names
			logInfo("main", "File: %s", (dir_path + dir.fileName()).c_str());
		}
		if (dir.isDirectory()) {
			// print directory names
			logInfo("main", "Dir: %s", (dir_path + dir.fileName() + "/").c_str());
			// recursive file listing inside new directory
			listAllFilesInDir(dir_path + dir.fileName() + "/");
		}
	}
}

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

    logError("main", "%u", '/');


    LittleFS.begin();
    //logHexInfo("Test", (uint8_t*)XIP_BASE + 0x00034664 - 10, 20);

    /*LittleFS.format();
    
    if(!LittleFS.exists("test.txt"))
    {
        File f = LittleFS.open("test.txt", "w");
        if (!f) {
            logError("main", "file open failed");
        }
        f.write("Hallo, hier bin ich und schreibe in diese Datei");
        f.close();
    } else {
        File f = LittleFS.open("test.txt", "r");
        logInfo("main", "%s", f.readString().c_str());
    }
    if(!LittleFS.exists("/neu.txt"))
    {
        File f = LittleFS.open("/neu.txt", "w+");
        if(!f)
        {
            logError("main", "neu.txt konnte nicht erstellt werden");
        } else {
            f.write("Und hier auch noch\r\neinen Umbruch");
            f.flush();
            f.close();
        }
    }
    if(!LittleFS.exists("/data/"))
    {
        if(LittleFS.mkdir("daten"))
        {
            if(!LittleFS.exists("/data/stand.txt"))
            {
                File f = LittleFS.open("/data/stand.txt", "w");
                f.write("Und hier auch noch\r\neinen Umbruch");
                f.flush();
                f.close();
            }
        } else {
            logError("main", "Verzeichnis konnte ncht erstellt werden");
        }
    }*/
    

    listAllFilesInDir("/");

    LittleFS.end();

}

void loop()
{
    openknx.loop();
}