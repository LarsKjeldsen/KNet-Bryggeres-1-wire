/*
 Name:		Bryggers_1_wire.ino
 Created:	6/22/2019 6:56:00 PM
 Author:	lars S. Kjeldsen
*/

// the setup function runs once when you press reset or power the board

#include <DallasTemperature.h>
#include <OneWire.h>
#include "Network.h"
#include "Reading.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "Network.h"
#include "Reading.h"

Reading* reading;
#define SLEEPTIME 60000

void setup() {
	Serial.begin(115200);
	Serial.println("Booting");

	WiFi_Setup();

	SendMQTT("KNet/Huset/Bryggers/Status", "Reboot", false);

	reading = new Reading();
}



unsigned long start = 0;  // Get a reading right away.

void loop() {
	ArduinoOTA.handle();
	
	if (millis() - start > SLEEPTIME)
	{
		start = millis();
		reading->Get_1wire();
		Send_reading(reading);
	}
	
	delay(250); // 60 sek minus loop runtime
}
