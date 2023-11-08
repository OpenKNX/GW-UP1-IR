#include <Arduino.h>
#include "OpenKNX.h"
#include "hardware.h"
#include "IRremote.h"
#include "pins.h"
#include "knxprod.h"
#include "LittleFS.h"

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
		uint _state = 0;
		IRrecv *rec;
		IRsend *send;
		IRData *_data;
		uint8_t _index;
		long _lastPress = -1;
		uint _pressInterval = 0;
		uint _pressState = 0;
		IRData* read(uint8_t index);
		void write(uint8_t index, IRData *data);
		void print(IRData *data, uint8_t index);
		bool compare(uint8_t index, uint8_t *data);
		void handleCode();
		void checkPress();
		void executeCode();
		void sendCode(uint8_t index);
		bool isEnabled = false;
		long lastCode = 0;
		bool processFunctionProperty(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength) override;
		bool processFunctionPropertyState(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength) override;
		bool processCommand(const std::string cmd, bool diagnoseKo) override;
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
    return "";
}

//will be called once
//only if knx.configured == true
void IrCodeModule::setup()
{
    rec = new IRrecv(13);
	rec->enableIRIn();

	//send = new IRsend(P21);
	//send->begin(P21);
	send = new IRsend();
	send->begin();
	send->enableIROut(38);
}

void IrCodeModule::loop()
{
	switch(_state)
	{
		case 0:
		{
			if(rec->decode())
			{
				rec->resume();
				handleCode();
			}
			checkPress();
			break;
		}

		case 1:
		{
			if(rec->decode())
			{
				rec->resume();
				if(rec->decodedIRData.protocol == 0 || rec->decodedIRData.address == 0 || rec->decodedIRData.numberOfBits == 0) return;

				_data = new IRData();
				_data->protocol = rec->decodedIRData.protocol;
				_data->address = rec->decodedIRData.address;
				_data->command = rec->decodedIRData.command;
				_data->numberOfBits = rec->decodedIRData.numberOfBits;
				_data-> flags = rec->decodedIRData.flags;
				_data->extra = rec->decodedIRData.extra;

				_state = 2;
				logDebugP("IR Code empfangen. Zum verifizieren erneut senden");
				rec->resume();
			}
			break;
		}

		case 2:
		{
			if(rec->decode())
			{
				rec->resume();
				if(rec->decodedIRData.protocol == 0 || rec->decodedIRData.address == 0 || rec->decodedIRData.numberOfBits == 0) return;

				if(rec->decodedIRData.protocol != _data->protocol)
				{
					logErrorP("Protokoll unterschiedlich");
					_state = 3;
					return;
				}
				if(rec->decodedIRData.address != _data->address)
				{
					logErrorP("Adresse unterschiedlich");
					_state = 3;
					return;
				}
				if(rec->decodedIRData.numberOfBits != _data->numberOfBits)
				{
					logErrorP("NumberOfBits unterschiedlich");
					_state = 3;
					return;
				}
				if(rec->decodedIRData.command != _data->command)
				{
					logErrorP("Command unterschiedlich");
					_state = 3;
					return;
				}

				this->write(_index, _data);
				this->print(_data, _index);
				logDebugP("IR Code erhalten und gespeichert");

				delete _data;

				_state = 0;
			}
			break;
		}
	}
}

void IrCodeModule::handleCode()
{
	if(_pressState > 1 && lastCode + 100 > millis()) return;
	if(_pressState < 2 && lastCode + 500 > millis()) return;
	this->print(&rec->decodedIRData, -1);
	if(rec->decodedIRData.protocol == 0 && rec->decodedIRData.address == 0) return;
	lastCode = millis();

	bool flag = false;

	File f;	
	if(!LittleFS.exists("/ircodes.txt"))
	{
		logErrorP("ircodes file doesn't exist");
		return;
	} else {
		f = LittleFS.open("/ircodes.txt", "r+");
		if(f.size() < CODE_COUNT*CODE_SIZE)
		{
			f.seek(f.size());
			for(int i = 0; i < (CODE_COUNT*CODE_SIZE - f.size()); i++)
				f.write((uint8_t)0x00);
		}
	}

	for(int i = 0; i < CODE_COUNT; i++)
	{
		f.seek(i * CODE_SIZE);
		uint8_t *data = new uint8_t[5];
		f.readBytes((char *)data, 5);
		bool compared = compare(i, data);
		delete[] data;

		if(compared)
		{
			_index = i;
			logDebugP("Code wurde gefunden: %i", i);
			int type = ParamIR_inOutTypeIndex(i);
			if(type == 1)
			{
				f.close();
				executeCode();
				return;
			}
			logErrorP("IrCode is not configured for receiving");
			flag = true;
		}
	}

	f.close();
	if(flag) return;

	this->print(&rec->decodedIRData, -1);
	logErrorP("Code wurde nicht gefunden");
}

void IrCodeModule::checkPress()
{
	switch(_pressState)
	{
		case 0:
		{
			_pressInterval = 0;
			break;
		}

		case 1:
		{
			if(_lastPress + 600 < millis())
			{
				logDebugP("Nur Schalten");
				_pressState = 0;
				
				if(ParamIR_inDimmDirectionIndex(_index))
					KoIR_co_n2Index(_index).value(false, DPT_Switch);
				else
					KoIR_co_n2Index(_index).value(true, DPT_Switch);
				}
			break;
		}

		case 2:
		{
			bool value = ParamIR_inDimmDirectionIndex(_index);
			KoIR_co_n1Index(_index).valueNoSend(!value, Dpt(3,7,0));
			KoIR_co_n1Index(_index).value((uint8_t)0b001, Dpt(3,7,1));
			logDebugP("Dimmen gestartet");

			_pressState = 3;
			break;
		}

		case 3:
		{
			if((_pressInterval > 1 && _lastPress + _pressInterval < millis())
				|| (_pressInterval == 0 && _lastPress + 300 < millis()))
			{
				KoIR_co_n1Index(_index).value((uint8_t)0x00, DPT_DecimalFactor); //Dimm Stop
				logDebugP("Dimmen beenden");
				_pressState = 0;
			}
			break;
		}
	}
}

void IrCodeModule::executeCode()
{
	int type = ParamIR_inTypeIndex(_index);
	switch(type)
	{
		case 0:
		{
			logDebugP("Execute Switch");
			int sw = ParamIR_inSwitchIndex(_index);

			switch(sw)
			{
				case 1:
					logDebugP("on");
					KoIR_co_n1Index(_index).value(true, DPT_Switch);
					KoIR_co_n2Index(_index).valueNoSend(true, DPT_Switch);
					break;

				case 2:
					logDebugP("off");
					KoIR_co_n1Index(_index).value(false, DPT_Switch);
					KoIR_co_n2Index(_index).valueNoSend(false, DPT_Switch);
					break;

				case 0:
					logDebugP("toggle");
					KNXValue val = KoIR_co_n2Index(_index).value(DPT_Switch);
					logDebugP("State is %i", (bool)val);
					bool value = !val;
					logDebugP("Set state %i", value);
					KoIR_co_n1Index(_index).value(value, DPT_Switch);
					KoIR_co_n2Index(_index).valueNoSend(value, DPT_Switch);
					break;
			}
			break;
		}
		
		case 1:
		{
			logDebugP("Execute Value");
			uint8_t sw = ParamIR_inValueIndex(_index);

			KoIR_co_n1Index(_index).value(sw, Dpt(5,5));
			break;
		}
		
		case 2:
		{
			logDebugP("Execute Scene");
			uint8_t sw = ParamIR_inSceneIndex(_index);
			sw -= 1;
			KoIR_co_n1Index(_index).value(sw, DPT_SceneNumber);
			break;
		}
		
		case 3:
		{
			logDebugP("Execute Dimm");
			
			if(_pressState == 1)
			{
				_pressState = 2;
			}
			
			if(_pressState == 0)
			{
				if(ParamIR_inDimmSwitchIndex(_index))
					_pressState = 1;
				else
					_pressState = 2;
			}
			
			if(_pressInterval == 0)
				_pressInterval = 1;
			else if(_pressInterval == 1)
			{
				_pressInterval = millis() - _lastPress + 30;
				logErrorP("Interval: %i", millis() - _lastPress);
			}

			_lastPress = millis();
			break;
		}
		
		case 4:
		{
			logDebugP("Execute Color");
			uint8_t *color;
			if(ParamIR_inColorTypeIndex(_index))
				color = ParamIR_inColor_HSVIndex(_index);
			else
				color = ParamIR_inColor_RGBIndex(_index);
			logDebugP("#%.2X%.2X%.2X", color[0], color[1], color[2]);
			uint32_t xcolor = color[2];
			xcolor |= color[1] << 8;
			xcolor |= color[0] << 16;
			KoIR_co_n1Index(_index).value(xcolor, DPT_Colour_RGB);
			break;
		}
		
	}
}

void IrCodeModule::sendCode(uint8_t index)
{
	logDebugP("Sending %i", index);
	IRData *data = this->read(index);
	this->print(data, index);
	rec->disableIRIn();
	send->write(data);
	rec->enableIRIn();
}

void IrCodeModule::print(IRData *data, uint8_t index)
{
	/*if(data->protocol == 0 && data->address == 0)
	{
		logErrorP("Ungültiger Code %i", index);
		return;
	}*/
	logDebugP("IR Code %i", index);
	logIndentUp();
	logDebugP("Protokoll %.2X", data->protocol);
	logDebugP("Address %.4X", data->address);
	logDebugP("Command %.4X", data->command);
	logDebugP("Number %.4X", data->numberOfBits);
	logDebugP("Extra %.4X", data->extra);
	logDebugP("Flags %.2X", data->flags);
	logIndentDown();
}

bool IrCodeModule::compare(uint8_t index, uint8_t *data)
{
	if(rec->decodedIRData.protocol != (decode_type_t)data[0]) return false;

	int temp = data[1] << 8;
	temp = temp | data[2];
	if(rec->decodedIRData.address != temp) return false;

	temp = data[3] << 8;
	temp = temp | data[4];
	if(rec->decodedIRData.command != temp) return false;

	return true;
}

IRData* IrCodeModule::read(uint8_t index)
{
	File f;
	IRData *data = new IRData();
	data->protocol = decode_type_t::UNKNOWN;
	
	if(!LittleFS.exists("/ircodes.txt"))
	{
		logErrorP("ircodes file doesn't exist");
		return data;
	} else {
		f = LittleFS.open("/ircodes.txt", "r+");
		if(f.size() < CODE_COUNT*CODE_SIZE)
		{
			f.seek(f.size());
			for(int i = 0; i < (CODE_COUNT*CODE_SIZE - f.size()); i++)
				f.write((uint8_t)0x00);
		}
	}

	f.seek(index * CODE_SIZE);
	data->protocol = (decode_type_t)f.read();
	int temp = f.read() << 8;
	data->address = temp | f.read();

	temp = f.read() << 8;
	data->command = temp | f.read();
	temp = f.read() << 8;
	data->numberOfBits = temp | f.read();
	temp = f.read() << 8;
	data->extra = temp | f.read();
	return data;
}

void IrCodeModule::write(uint8_t index, IRData *data)
{
	File f;
	if(!LittleFS.exists("/ircodes.txt"))
	{
		f = LittleFS.open("/ircodes.txt", "w+");
		for(int i = 0; i < CODE_COUNT*CODE_SIZE; i++)
			f.write((uint8_t)0x00);
		f.flush();
	} else {
		f = LittleFS.open("/ircodes.txt", "r+");
		if(f.size() < CODE_COUNT*CODE_SIZE)
		{
			f.seek(f.size());
			for(int i = 0; i < (CODE_COUNT*CODE_SIZE - f.size()); i++)
				f.write((uint8_t)0x00);
		}
	}

	f.seek(index * CODE_SIZE);
	f.write((uint8_t)data->protocol);
	f.write((uint8_t)(data->address >> 8));
	f.write((uint8_t)(data->address & 0xFF));
	f.write((uint8_t)(data->command >> 8));
	f.write((uint8_t)(data->command & 0xFF));
	f.write((uint8_t)(data->numberOfBits >> 8));
	f.write((uint8_t)(data->numberOfBits & 0xFF));
	f.write((uint8_t)(data->extra >> 8));
	f.write((uint8_t)(data->extra & 0xFF));

	/*uint8_t *buffer = new uint8_t[9];
	buffer[0] = data->protocol;
	pushWord(data->address, buffer+1);
	pushWord(data->command, buffer+3);
	pushWord(data->numberOfBits, buffer+5);
	pushWord(data->extra, buffer+7);

	delete[] buffer;*/

	f.flush();
	f.close();
}

//will be called once a KO received a telegram
void IrCodeModule::processInputKo(GroupObject& iKo)
{
    int index = floor((iKo.asap() - 1) / 2);
	logDebugP("got KO %i is index %i", iKo.asap(), index);

	int type = ParamIR_inOutTypeIndex(index);
	if(type != 2)
	{
		logDebugP("KO ist nicht zum Empfangen gedacht");
		return;
	}

	type = ParamIR_outTypeIndex(index);
	switch(type)
	{
		case 0: //Switch
		{
			logDebugP("Switch");
			int stype = ParamIR_outSwitchIndex(index);
			switch(stype)
			{
				case 0: //Always
				{
					logDebugP("Send Always");
					sendCode(index);
					break;
				}
				
				case 1: //On
				{
					logDebugP("Send On");
					if(iKo.value(DPT_Switch))
						sendCode(index);
					break;
				}
				
				case 2: //Off
				{
					logDebugP("Send Off");
					if(!iKo.value(DPT_Switch))
						sendCode(index);
					break;
				}
			}
			break;
		}
		
		case 1: //Value
		{
			logDebugP("Value");
			int svalue = ParamIR_outValueIndex(index);
			int ivalue = (uint8_t)iKo.value(DPT_Scaling);
			if(svalue == ivalue)
				sendCode(index);
			break;
		}
		
		case 2: //Scene
		{
			logDebugP("Scene");
			uint8_t ivalue = (uint8_t)iKo.value(DPT_SceneNumber);
			if(ParamIR_outSceneActive1Index(index))
			{
				int svalue = ParamIR_outScene1Index(index) - 1;
				if(svalue == ivalue)
				{
					sendCode(index);
					break;
				}
			}
			if(ParamIR_outSceneActive2Index(index))
			{
				int svalue = ParamIR_outScene2Index(index) - 1;
				if(svalue == ivalue)
				{
					sendCode(index);
					break;
				}
			}
			if(ParamIR_outSceneActive3Index(index))
			{
				int svalue = ParamIR_outScene3Index(index) - 1;
				if(svalue == ivalue)
				{
					sendCode(index);
					break;
				}
			}
			if(ParamIR_outSceneActive4Index(index))
			{
				int svalue = ParamIR_outScene4Index(index) - 1;
				if(svalue == ivalue)
				{
					sendCode(index);
					break;
				}
			}
			break;
		}
	}
}

bool IrCodeModule::processFunctionProperty(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
	if(objectIndex != 0 || propertyId != 242) return false;

	switch(data[1])
	{
		case 0x01:
		{
			logDebugP("Lerne index %i", data[0]);
			logDebugP("Bitte IR Code senden");
			_index = data[0];
			_state = 1;
			resultData[0] = 0x00;
			resultLength = 1;
			break;
		}

		case 0x02:
		{
			logDebugP("Entferne index %i", data[0]);
			resultData[0] = 0x00;
			resultLength = 1;
			_data = new IRData();
			_data->protocol = decode_type_t::UNKNOWN;
			_data->address = 0;
			_data->command = 0;
			this->write(data[0], _data);
			delete _data;
			break;
		}

		default:
		{
			logErrorP("Unbekanntes Kommando");
			resultData[0] = 0xFF;
			resultLength = 1;
			break;
		}
	}
	
	return true;
}

bool IrCodeModule::processFunctionPropertyState(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
	if(objectIndex == 0 && propertyId == 242)
	{
		resultData[0] = _state;
		resultLength = 1;
		if(_state == 3)
			_state = 0;
		return true;
	}

	return false;
}

bool IrCodeModule::processCommand(const std::string cmd, bool diagnoseKo)
{
	if(diagnoseKo) return false;

	std::size_t pos = cmd.find(' ');
	std::string command;
	std::string arg;
	if(pos != -1)
	{
		command = cmd.substr(0, pos);
		arg = cmd.substr(pos+1, cmd.length() - pos - 1);
	} else {
		command = cmd;
	}

	if(command == "learnIR")
	{
		_index = std::stoi(arg);
		logDebugP("Lerne index %i", _index);
		logDebugP("Bitte IR Code senden");
		_state = 1;
		return true;
	}
	if(command == "deleteIR")
	{
		logDebugP("Entferne index %i", _index);
		_data = new IRData();
		_data->protocol = decode_type_t::UNKNOWN;
		_data->address = 0;
		_data->command = 0;
		this->write(_index, _data);
		delete _data;
		return true;
	}


	return false;
}

IrCodeModule openknxIrCodeModule;
extern IrCodeModule openknxIrCodeModule;