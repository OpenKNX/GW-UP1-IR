#include <Arduino.h>
#include "OpenKNX.h"
#include "hardware.h"
#include "IRremote.h"
#include "pins.h"

class IrCodeModule : public OpenKNX::Module
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
		IRData read(uint8_t index);
		void write(uint8_t index, IRData data);
		void print(IRData data, int index);
};

//Give your Module a name
//it will be desplayed when you use the method log("Hello")
// -> Log     Hello
const std::string IrCodeModule::name()
{
    return "IrCode";
}

//You can also give it a version
//will be displayed in Command Infos 
const std::string IrCodeModule::version()
{
    return "0.0dev";
}

//will be called once
//only if knx.configured == true
void IrCodeModule::setup()
{
    rec = new IRrecv(P29);
	rec->enableIRIn();

	logInfoP("setup");
	IRData data = this->read(0);
	this->print(data, 0);
}

void IrCodeModule::loop()
{
    if(rec->decode())
	{
		rec->resume();
		if(rec->decodedIRData.protocol == 0 && rec->decodedIRData.address == 0) return;
		this->print(rec->decodedIRData, -1);
		this->write(0, rec->decodedIRData);
	}
	if(Serial2.available())
	{
		byte b1 = Serial2.read();
		if(b1 == 0xAB)
		{
			b1 = Serial.read();
			if(b1 == 0xFF)
			{
				b1 = Serial.read();
				IRData data;
				data.protocol = (decode_type_t)Serial.read();
				int temp = Serial.read() << 8;
				data.address = temp | Serial.read();
				temp = Serial.read() << 8;
				data.command = temp | Serial.read();
				temp = Serial.read() << 8;
				data.numberOfBits = temp | Serial.read();
				temp = Serial.read() << 8;
				data.extra = temp | Serial.read();
				data.flags = Serial.read();

				temp = Serial.read();
				this->print(data, b1);

				if(temp != 0xAC)
				{
					logErrorP("Falscher Endcode!");
					return;
				}

				this->print(data, b1);
				this->write(b1, data);
			}
		}
	}
}

void IrCodeModule::print(IRData data, int index)
{
	if(data.protocol == 0 && data.address == 0)
	{
		logErrorP("Ungültiger Code %i", index);
		return;
	}
	logInfoP("IR Code %i", index);
	logIndentUp();
	logInfoP("Protokoll %u", data.protocol);
	logInfoP("Address %u", data.address);
	logInfoP("Command %u", data.command);
	logIndentDown();
}

IRData IrCodeModule::read(uint8_t index)
{
	long address = CODE_FLASH_OFFSET + (index * CODE_SIZE);
	uint8_t *pointer = knx.platform().getNonVolatileMemoryStart();
	IRData data;
	data.protocol = (decode_type_t)pointer[address];
	int temp = pointer[address + 1] << 8;
	data.address = temp | pointer[address + 2];
	temp = pointer[address + 3] << 8;
	data.command = temp | pointer[address + 4];
	temp = pointer[address + 5] << 8;
	data.numberOfBits = temp | pointer[address + 6];
	temp = pointer[address + 7] << 8;
	data.extra = temp | pointer[address + 8];
	data.flags = pointer[address + 9];
	return data;
}

void IrCodeModule::write(uint8_t index, IRData data)
{
	uint8_t *buffer = new uint8_t[10] {
		rec->decodedIRData.protocol,
		rec->decodedIRData.address >> 8,
		rec->decodedIRData.address & 0xFF,
		rec->decodedIRData.command >> 8,
		rec->decodedIRData.command && 0xFF,
		rec->decodedIRData.numberOfBits >> 8,
		rec->decodedIRData.numberOfBits && 0xFF,
		rec->decodedIRData.extra >> 8,
		rec->decodedIRData.extra && 0xFF,
		rec->decodedIRData.flags
	};

	long address = CODE_FLASH_OFFSET + (index * CODE_SIZE);
	knx.platform().writeNonVolatileMemory(0, buffer, 10);

	delete[] buffer;
}

//will be called once a KO received a telegram
void IrCodeModule::processInputKo(GroupObject& iKo)
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