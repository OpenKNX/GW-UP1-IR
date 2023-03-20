#include <Arduino.h>
#include <PicoOTA.h>
#include "OpenKNX.h"

#define INFO_INTERVAL 10000

class UpdateModule : public OpenKNX::Module
{
	public:
		const std::string name() override;
		const std::string version() override;
		void loop() override;

	private:
        File _file;
        uint8_t *_data;
        uint32_t _size;
        uint _position;
        uint _lastPosition;
        long _lastInfo = 0;
        bool _rebootRequested = false;
		bool processFunctionProperty(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength) override;
		bool processFunctionPropertyState(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength) override;
};

//Give your Module a name
//it will be desplayed when you use the method log("Hello")
// -> Log     Hello
const std::string UpdateModule::name()
{
    return "UpdateModule";
}

//You can also give it a version
//will be displayed in Command Infos 
const std::string UpdateModule::version()
{
    return "0.0dev";
}

void UpdateModule::loop()
{
    if(_rebootRequested)
        rp2040.reboot();
}

bool UpdateModule::processFunctionProperty(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    if(objectIndex != 0) return false;

    switch(propertyId)
    {
        case 243:
        {
            logInfoP("Starting Firmware Update");
            _size = data[0] << 24;
            _size |= data[1] << 16;
            _size |= data[2] << 8;
            _size |= data[3];
            _position = 0;
            _lastInfo = millis();
            _lastPosition = 0;
            logInfoP("File Size: %i", _size);
            LittleFS.begin();
            _file = LittleFS.open("firmware.bin", "w");
            resultLength = 0;
            return true;
        }
        
        case 244:
        {
            if(_file.write(data, length) != length)
            {
                resultData[0] = 0x01;
                resultLength = 1;
                return true;
            }
            _position += length;

            if(delayCheck(_lastInfo, INFO_INTERVAL))
            {
                _lastInfo = millis();
                logInfoP("Progress: %i\% - %i B/s", (_position / _size) * 100, (_position - _lastPosition) / (INFO_INTERVAL / 1000));
                _lastPosition = _position;
            }

            resultLength = 0;
            return true;
        }
        
        case 245:
        {
            _file.close();
            picoOTA.begin();
            picoOTA.addFile("firmware.bin");
            picoOTA.commit();
            LittleFS.end();
            resultLength = 0;
            return true;
        }
    }
}