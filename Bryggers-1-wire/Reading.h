// Reading.h

#ifndef _READING_h
#define _READING_h

#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Network.h"

class Reading
{
public:
	DeviceAddress *DevAddr;
	String *DevName;
	int16_t *DevValue;
	int DevNum;

	void Get_1wire();
	Reading();
private:
};
#endif

