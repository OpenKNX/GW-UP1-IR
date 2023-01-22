#include <Arduino.h>
#include <SoftwareSerial.h>
#include <knx.h>
#include <math.h>
#include "hardware.h"
#include "IRremote.h"

#define deb KNX_DEBUG_SERIAL

//#define goLedSwitch knx.getGroupObject(1)
//#define goLedState knx.getGroupObject(2)
IRrecv rec;
IRsend send;

KnxFacade<RP2040ArduinoPlatform, Bau07B0> knx;

void setup()
{
    KNX_DEBUG_SERIAL.begin(115200);

    /*knx.readMemory();

    if (knx.configured())
    {
        
    } else {
        KNX_DEBUG_SERIAL.println("Nicht konfiguriert!");
    }
        
    rec.begin(D2);
    send.begin();
    //pinMode(D0, OUTPUT);
    //send.begin(D0);
    //digitalWrite(D0, LOW);

    //knx.start();

    //knx.progMode(true);
    KNX_DEBUG_SERIAL.println("Start");
    */

   pinMode(D3, OUTPUT);
   pinMode(D4, OUTPUT);
   pinMode(D5, OUTPUT);
   pinMode(D6, OUTPUT);
   pinMode(D7, OUTPUT);
   pinMode(D8, OUTPUT);
   pinMode(D9, OUTPUT);
   pinMode(D10, OUTPUT);
}

IRData data;
int lastmillis = 0;
int count = 0;

int counter = 0;
int lastMillis = 0;

void loop()
{
    if(lastMillis + 500 < millis())
    {
        counter++;
        lastMillis = millis();
    }

    for(int i = 0; i < 4; i++)
    {
        digitalWrite(D10, i == 3);
        digitalWrite(D9, i == 2);
        digitalWrite(D8, i == 1);
        digitalWrite(D7, i == 0);

        double refe = counter / pow(10,i);
        int digit = (int)refe;
        digit = digit % 10;

        digitalWrite(D3, digit & 0b0001);
        digitalWrite(D4, digit & 0b0010);
        digitalWrite(D5, digit & 0b0100);
        digitalWrite(D6, digit & 0b1000);

        delay(2);
    }

    return;
    knx.loop();

    if (rec.decode()) {
        if (rec.decodedIRData.numberOfBits == 0 ||
            rec.decodedIRData.protocol == UNKNOWN){
            deb.print("No of bits: ");
            deb.println(rec.decodedIRData.numberOfBits, HEX);
            deb.print("Protokol: ");
            deb.println(rec.decodedIRData.protocol, HEX);
            rec.resume();
            return;
        }
        KNX_DEBUG_SERIAL.println("Got IR Code");
        deb.println();
        deb.print("Address: ");
        deb.println(rec.decodedIRData.address, HEX);
        deb.print("Command: ");
        deb.println(rec.decodedIRData.command);
        deb.print("Protokol: ");
        deb.println(rec.decodedIRData.protocol);
        deb.print("Flags: ");
        deb.println(rec.decodedIRData.flags);
        deb.print("NOB: ");
        deb.println(rec.decodedIRData.numberOfBits);

        if(rec.decodedIRData.command == 0x03)
        {
            lastmillis = millis();
            data.command = rec.decodedIRData.command;
            data.address = rec.decodedIRData.address;
            data.numberOfBits = rec.decodedIRData.numberOfBits;
            data.flags = rec.decodedIRData.flags;
            data.protocol = rec.decodedIRData.protocol;
        }

        rec.resume();
    }

    /*
    if(lastmillis > 0 && lastmillis + 5000 < millis())
    {
        deb.println("Sending");
        send.write(&data);
    }*/


    if(lastmillis + 2000 < millis())
    {
        lastmillis = millis();
        count++;

        switch(count)
        {
            case 1:
                deb.println("Sending1");
                send.sendNEC(0xEF00, 3, 1);
                break;

            case 2:
                deb.println("Sending2");
                send.sendNEC2(0xEF00, 2, 1);
                break;

            case 3:
                deb.println("Sending3");
                IRData datax;
                datax.address = 0xEF00;
                datax.command = 3;
                datax.numberOfBits = 32;
                datax.protocol = decode_type_t::NEC;
                send.write(&datax);
                break;

            case 4:
                break;
        }

        if(count == 4)
        {
            count = 0;
        }
    }


    if (!knx.configured()) return;


}