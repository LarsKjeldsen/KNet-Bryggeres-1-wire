// Network.h

#ifndef _NETWORK_h
#define _NETWORK_h

#include "Arduino.h"
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include "Reading.h"

void WiFi_Setup();
void MQTT_Setup();

void SendMQTT(const char* Topic, char* payload, bool retained = true);
void SendMQTT(const char* Topic, int32_t payload, bool retained = true);
void SendMQTT(const char* Topic, float payload, bool retained = true);

void Send_reading(class Reading *);

#endif

