#include <Arduino.h>
#include "OpenKNX.h"
#include "hardware.h"
#include "IRremote.h"

class LedModule : public OpenKNX::Module
{
	public:
		void loop() override;
		void setup() override;
		void processInputKo(GroupObject &ko) override;

		const std::string name() override;
		const std::string version() override;
		// void writeFlash() override;
		// void readFlash(const uint8_t* data, const uint16_t size) override;
		// uint16_t flashSize() override;

	private:
		IRrecv *rec;
};

//Give your Module a name
//it will be desplayed when you use the method log("Hello")
// -> Log     Hello
const std::string LedModule::name()
{
    return "Led";
}

//You can also give it a version
//will be displayed in Command Infos 
const std::string LedModule::version()
{
    return "0.0dev";
}

//will be called once
//only if knx.configured == true
void LedModule::setup()
{
    rec = new IRrecv(D1);
}

//will be called every loop
//only if knx.configured == true
void LedModule::loop()
{
    if(rec->decode())
	{

		rec->resume();
	}
}

//will be called once a KO received a telegram
void LedModule::processInputKo(GroupObject& iKo)
{
    
}

// void LogModule::writeFlash()
// {
//     for (size_t i = 0; i < flashSize(); i++)
//     {
//         //openknx.flash.writeByte(0xd0 + i);
//     }
// }

// void LogModule::readFlash(const uint8_t* data, const uint16_t size)
// {
//     // printHEX("RESTORE:", data,  len);
// }

// uint16_t LogModule::flashSize()
// {
//     return 10;
// }