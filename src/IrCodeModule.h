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
		IRData* read(uint8_t index);
		void write(uint8_t index, IRData *data);
		void print(IRData *data, int index);
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
	for(int i = 0; i < 5; i++)
	{
		IRData *data = this->read(i);
		this->print(data, i);
		delete data;
	}

	Serial2.setTX(P11);
	Serial2.setRX(P12);
	Serial2.begin(115200);
	delay(1000);
	Serial2.write(0xAD);
}

void IrCodeModule::loop()
{
    if(rec->decode() && false)
	{
		rec->resume();
		if(rec->decodedIRData.protocol == 0 && rec->decodedIRData.address == 0) return;
		this->print(&rec->decodedIRData, -1);
		this->write(0, &rec->decodedIRData);
	}

	if(Serial2.available())
	{
		logInfoP("Got Serial Byte");
		byte b1 = Serial2.read();
		if(b1 == 0xAB)
		{
			logInfoP("Got AB");
			b1 = Serial2.read();
			if(b1 == 0xFF)
			{
				logInfoP("Got FF");
				b1 = Serial2.read();
				logInfoP("Index %u", b1);
				IRData *data = new IRData();
				data->protocol = (decode_type_t)Serial2.read();
				logInfoP("Protokol: %u", data->protocol);
				int temp = Serial2.read() << 8;
				data->address = temp | Serial2.read();
				temp = Serial2.read() << 8;
				data->command = temp | Serial2.read();
				logInfoP("Protokol: %u", data->command);
				temp = Serial2.read() << 8;
				data->numberOfBits = temp | Serial2.read();
				temp = Serial2.read() << 8;
				data->extra = temp | Serial2.read();
				data->flags = Serial2.read();

				temp = Serial2.read();
				this->print(data, b1);

				if(temp != 0xAC)
				{
					logErrorP("Falscher Endcode!");
					return;
				}

				this->write(b1, data);

				delete data;
			}
		}
	}
}

void IrCodeModule::print(IRData *data, int index)
{
	if(data->protocol == 0 && data->address == 0)
	{
		logErrorP("Ungültiger Code %i", index);
		return;
	}
	logInfoP("IR Code %i", index);
	logIndentUp();
	logInfoP("Protokoll %u", data->protocol);
	logInfoP("Address %u", data->address);
	logInfoP("Command %u", data->command);
	logIndentDown();
}

IRData* IrCodeModule::read(uint8_t index)
{
	logInfoP("Read %u", index);
	long address = CODE_FLASH_OFFSET + (index * CODE_SIZE);
	logInfoP("Address: %u", address);
	uint8_t *pointer = knx.platform().getNonVolatileMemoryStart();
	
	IRData *data = new IRData();

	data->protocol = (decode_type_t)pointer[address];
	int temp = pointer[address + 1] << 8;
	data->address = temp | pointer[address + 2];
	temp = pointer[address + 3] << 8;
	data->command = temp | pointer[address + 4];
	temp = pointer[address + 5] << 8;
	data->numberOfBits = temp | pointer[address + 6];
	temp = pointer[address + 7] << 8;
	data->extra = temp | pointer[address + 8];
	data->flags = pointer[address + 9];
	return data;
}

void IrCodeModule::write(uint8_t index, IRData *data)
{
	uint8_t *buffer = new uint8_t[10] {
		data->protocol,
		data->address >> 8,
		data->address & 0xFF,
		data->command >> 8,
		data->command && 0xFF,
		data->numberOfBits >> 8,
		data->numberOfBits && 0xFF,
		data->extra >> 8,
		data->extra && 0xFF,
		data->flags
	};

	long address = CODE_FLASH_OFFSET + (index * CODE_SIZE);
	knx.platform().writeNonVolatileMemory(address, buffer, 10);
	knx.platform().commitNonVolatileMemory();

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