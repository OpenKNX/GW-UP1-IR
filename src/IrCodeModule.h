#include <Arduino.h>
#include "OpenKNX.h"
#include "hardware.h"
#include "IRremote.h"
#include "pins.h"
#include "knxprod.h"

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
		void print(IRData *data, uint8_t index);
		bool compare(uint8_t index);
		void handleCode();
		void executeCode(int index);
		bool isEnabled = false;
		long lastCode = 0;
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
	isEnabled = true;

	for(uint8_t i = 0; i < 4; i++)
	{
		IRData *data = this->read(i);
		this->print(data, i);
		delete data;
	}

	Serial2.setTX(P11);
	Serial2.setRX(P12);
	Serial2.begin(115200);
	Serial2.write(0xAD);
}

void IrCodeModule::loop()
{
    if(rec->decode())
	{
		rec->resume();
		handleCode();
	}

	if(Serial2.available())
	{
		byte b1 = Serial2.read();
		logInfoP("Got Serial Byte %.2X", b1);
		if(b1 == 0xAB)
		{
			logInfoP("Got AB");
			b1 = Serial2.read();
			logInfoP("Got %.2X", b1);

			switch(b1)
			{
				case 0xFD:
				{
					rec->enableIRIn();
					logInfoP("IRIn enabled");
					isEnabled = true;
					break;
				}

				case 0xFE:
				{
					rec->disableIRIn();
					logInfoP("IRIn disabled");
					isEnabled = false;
					break;
				}

				case 0xFF:
				{
					b1 = Serial2.read();
					IRData *data = new IRData();
					data->protocol = (decode_type_t)Serial2.read();
					int temp = Serial2.read() << 8;
					data->address = temp | Serial2.read();
					temp = Serial2.read() << 8;
					data->command = temp | Serial2.read();
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
					if(!isEnabled)
					{
						logInfoP("IRIn enabled");
						rec->enableIRIn();
					}
					break;
				}
			}
		}
	}
}

void IrCodeModule::handleCode()
{
	if(lastCode + 500 > millis()) return;
	if(rec->decodedIRData.protocol == 0 && rec->decodedIRData.address == 0) return;
	lastCode = millis();
	this->print(&rec->decodedIRData, -1);

	for(int i = 0; i < CODE_COUNT; i++)
	{
		if(compare(i))
		{
			logInfoP("Code wurde gefunden: %i", i);

			int offset = mod_ir_para[i] + PARAM_ir_inOutType;
			logInfoP("Parameter Offset: %i, offset");

			int type = (knx.paramByte(offset) << PARAM_ir_inOutType_Shift) & PARAM_ir_inOutType_Mask;
			logInfoP("Parameter Type: %i", type);
			if(type == 1)
			{
				executeCode(i);
				break;
			}
			logErrorP("IrCode is not configured for receiving");
		}
	}

	logErrorP("Code wurde nicht gefunden");
}

void IrCodeModule::executeCode(int index)
{
	int offset = mod_ir_para[index] + PARAM_ir_inType;
	int type = (knx.paramByte(offset) << PARAM_ir_inType_Shift) & PARAM_ir_inType_Mask;
	switch(type)
	{
		case 0:
		{
			logInfoP("Execute Switch");
			offset = mod_ir_para[index] + PARAM_ir_inSwitch;
			int sw = (knx.paramByte(offset) << PARAM_ir_inSwitch_Shift) & PARAM_ir_inSwitch_Mask;

			int coNr1 = 1 + mod_ir_coms[index];
			int coNr2 = 2 + mod_ir_coms[index];

			switch(sw)
			{
				case 0:
					logInfoP("toggle");
					KNXValue val = knx.getGroupObject(coNr2).value(DPT_Switch);
					logInfoP("State is %b", val);
					bool value = !val;
					logInfoP("Set state %b", value);
					knx.getGroupObject(coNr1).value(value, DPT_Switch);
					knx.getGroupObject(coNr2).valueNoSend(value, DPT_Switch);
					break;

				case 1:
					logInfoP("on");
					knx.getGroupObject(coNr1).value(true, DPT_Switch);
					knx.getGroupObject(coNr2).valueNoSend(true, DPT_Switch);
					break;

				case 2:
					logInfoP("off");
					knx.getGroupObject(coNr1).value(false, DPT_Switch);
					knx.getGroupObject(coNr2).valueNoSend(false, DPT_Switch);
					break;
			}
			break;
		}
		
		case 1:
		{
			logInfoP("Execute Value");
			offset = mod_ir_para[index] + PARAM_ir_inValue;
			uint8_t sw = knx.paramByte(offset);

			int coNr1 = 1 + mod_ir_coms[index];
			knx.getGroupObject(coNr1).value(sw, Dpt(5,5));
			break;
		}
		
		case 2:
		{
			logInfoP("Execute Scene");
			offset = mod_ir_para[index] + PARAM_ir_inScene;
			uint8_t sw = (knx.paramByte(offset) << PARAM_ir_inScene_Shift) & PARAM_ir_inScene_Mask;

			int coNr1 = 1 + mod_ir_coms[index];
			knx.getGroupObject(coNr1).value(sw, DPT_SceneNumber);
			break;
		}
		
		case 3:
		{
			logInfoP("Execute Dimm");
			break;
		}
		
		case 4:
		{
			logInfoP("Execute Color");
			break;
		}
		
	}
}

void IrCodeModule::print(IRData *data, uint8_t index)
{
	if(data->protocol == 0 && data->address == 0)
	{
		logErrorP("Ungültiger Code %i", index);
		return;
	}
	logInfoP("IR Code %i", index);
	logIndentUp();
	logInfoP("Protokoll %.2X", data->protocol);
	logInfoP("Address %.4X", data->address);
	logInfoP("Command %.4X", data->command);
	logInfoP("Number %.4X", data->numberOfBits);
	logInfoP("Extra %.4X", data->extra);
	logInfoP("Flags %.2X", data->flags);
	logIndentDown();
}

bool IrCodeModule::compare(uint8_t index)
{
	long address = CODE_FLASH_OFFSET + (index * CODE_SIZE);
	uint8_t *pointer = knx.platform().getNonVolatileMemoryStart();

	if(rec->decodedIRData.protocol != (decode_type_t)pointer[address]) return false;

	int temp = pointer[address + 1] << 8;
	temp = temp | pointer[address + 2];
	if(rec->decodedIRData.address != temp) return false;

	temp = pointer[address + 3] << 8;
	temp = temp | pointer[address + 4];
	if(rec->decodedIRData.command != temp) return false;

	return true;
}

IRData* IrCodeModule::read(uint8_t index)
{
	long address = CODE_FLASH_OFFSET + (index * CODE_SIZE);
	logInfoP("Address %.4X", address);
	uint8_t *pointer = knx.platform().getNonVolatileMemoryStart();
	
	IRData *data = new IRData();

	data->protocol = (decode_type_t)pointer[address];
	int temp = pointer[address + 1] << 8;
	data->address = temp | pointer[address + 2];

	temp = pointer[address + 3] << 8;
	logInfoP("%.2X", pointer[address + 3]);
	logInfoP("%.2X", pointer[address + 4]);
	data->command = temp | pointer[address + 4];
	logInfoP("%.4X", data->command);

	temp = pointer[address + 5] << 8;
	data->numberOfBits = temp | pointer[address + 6];
	temp = pointer[address + 7] << 8;
	data->extra = temp | pointer[address + 8];
	return data;
}

void IrCodeModule::write(uint8_t index, IRData *data)
{
	uint8_t *buffer = new uint8_t[9] {
		data->protocol,
		data->address >> 8,
		data->address & 0xFF,
		data->command >> 8,
		data->command & 0xFF,
		data->numberOfBits >> 8,
		data->numberOfBits & 0xFF,
		data->extra >> 8,
		data->extra & 0xFF,
	};

	logInfoP("%.2X", buffer[3]);
	logInfoP("%.2X", buffer[4]);

	long address = CODE_FLASH_OFFSET + (index * CODE_SIZE);
	logInfoP("Address %.4X", address);
	knx.platform().writeNonVolatileMemory(address, buffer, 9);
	knx.platform().commitNonVolatileMemory();

	delete[] buffer;
}

//will be called once a KO received a telegram
void IrCodeModule::processInputKo(GroupObject& iKo)
{
    
}