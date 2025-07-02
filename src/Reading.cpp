#include "Reading.h"



#define ONE_WIRE_BUS D4
#define TEMPERATURE_PRECISION 9
#define ONEWIRESNUM 7
/*
1 VV ud
2 Gulv lille retur
3 Gulv spa ud
4 FV ind
5 FV ud
6 Gulv lille ud
7 Gulv spa retur

*/

OneWire  ds(ONE_WIRE_BUS);
DallasTemperature sensors(&ds);


Reading::Reading()
{
	Serial.println("Starting 1-wire");
	DevAddr = new DeviceAddress[ONEWIRESNUM];
	DevName = new String[ONEWIRESNUM];
	DevValue = new int16_t[ONEWIRESNUM];
	DevNum = 0;
	// int error;
	// unsigned long Timeout = millis() + 2000UL;

	pinMode(ONE_WIRE_BUS, OUTPUT);
	sensors.begin();
	delay(100);

	String Address;

	Serial.println("**************************");

	char s[256];

	for (int i = 0; i < ONEWIRESNUM; i++)
	{
		Address = "";
		if (sensors.getAddress(DevAddr[i], i))
		{
			for (int j = 0; j < 8; j++)
				if (DevAddr[DevNum][j] < 10)
					DevName[i] += "0" + String(DevAddr[DevNum][j], HEX);
				else
					DevName[i] += String(DevAddr[DevNum][j], HEX);

			sprintf(s, "Addresse %d : %s", DevNum, Address.c_str());
			SendMQTT("KNet/Huset/Bryggers/Status", s, false);

//			Serial.print("Address "); Serial.print(DevNum); Serial.print(" = "); Serial.println(Address.c_str());
			DevNum++;
		}
	}
//	Serial.print("Number of devices found : "); Serial.println(DevNum);

	sprintf(s, "Devices : %d", DevNum);
	SendMQTT("KNet/Huset/Bryggers/Status", s, false);
}


void Reading::Get_1wire()
{
	sensors.setWaitForConversion(true);
	sensors.setResolution(12);

	sensors.requestTemperatures();

	for (int k = 0; k < DevNum; k++)
	{
		float temp = -999.0;
		temp = sensors.getTempC(DevAddr[k]); // getTempCByIndex(k);

		if (temp != DEVICE_DISCONNECTED_C && temp != 85.0)  // 85 is a error or default value (very strange setup from Maxim...)
		{
//			Serial.print(DevName[k]); Serial.print(" = "); Serial.println(temp);
			DevValue[k] = (int16_t)(temp * 100);
		}
		else
		{
//			Serial.print(DevName[k]); Serial.println(" = ERROR");
			DevValue[k] = -999;
		}
		delay(350);
	}
	Serial.println();
}
