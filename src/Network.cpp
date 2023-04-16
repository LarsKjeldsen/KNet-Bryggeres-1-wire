#include "Network.h"
#include "Reading.h"
#include "WIFI-Secret.h"

char ssid[] = SSID_NAME;
char password[] = PASSWORD;

IPAddress ip(192, 168, 1, 215);
IPAddress gw(192, 168, 1, 1);
IPAddress mask(255, 255, 255, 0);

WiFiClient ethClient;

IPAddress MQTTServer(192, 168, 1, 22);
PubSubClient MQTTclient(ethClient);



void WiFi_Setup()
{
	WiFi.mode(WIFI_STA);
	WiFi.config(ip, gw, mask);
	WiFi.begin(ssid, password);

	while (int ret = WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.print("Connection Failed! Retrying... ErrorCode = "); Serial.println(ret);
		delay(1000);
//		ESP.restart();
	}
	while (WiFi.status() != WL_CONNECTED) {
		delay(50);
		Serial.print(".");
	}

	ArduinoOTA.setHostname("Bryggers-1wire");

	ArduinoOTA.onStart([]() {
		Serial.println("OTA Start ");
		});
	ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
		});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		unsigned int procent = (progress * 100) / total;
			Serial.print(procent); Serial.print("% - ");
		});
	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
		});
	ArduinoOTA.begin();


	Serial.println("");
	Serial.print("WiFi connected IP address: ");
	Serial.println(WiFi.localIP());
}
     
void Send_reading(Reading* r)
{
	for (int i = 0; i < r->DevNum; i++)
	{
		float value = r->DevValue[i] / 100;

		if (value == -999)
		{
			String s = "KNet/Bryggers/" + r->DevName[i] + "-ERROR";
			SendMQTT(s.c_str(), value, false);
		}
		else
		{
//			String s = "KNet/Huset/Bryggers/" + r->DevName[i];
//			SendMQTT(s.c_str(), value);
			if (i == 0) SendMQTT("KNet/Huset/Bryggers/Varmt_Vand",			value);   // 28ffa82d65040003
			else if (i == 1) SendMQTT("KNet/Huset/Bryggers/Gulv_lille_retur", value); // 28ff6436650400ac 
			else if (i == 2) SendMQTT("KNet/Huset/Bryggers/Gulv_spa_ud",	value);   // 28ff5630650400a2
			else if (i == 3) SendMQTT("KNet/Huset/Bryggers/Fjernvarme_ind",	value);   // 28ff56316504002d 
			else if (i == 4) SendMQTT("KNet/Huset/Bryggers/Fjernvarme_ud",	value);   // 28fffd3565040060
			else if (i == 5) SendMQTT("KNet/Huset/Bryggers/Gulv_lille_ud",	value);   // 28fff337650400c5
			else if (i == 6) SendMQTT("KNet/Huset/Bryggers/Gulv_spa_retur",	value);   // 28ffab2e650400c5
		}
	}
}

void MQTT_Setup()
{
	int c = 0;
	String IP = WiFi.localIP().toString();
	MQTTclient.setServer(MQTTServer, 1883);

	String clientId = "Bryggers-" + IP + "-";
	clientId += String(random(0xffff), HEX);
	while (!MQTTclient.connected() && c++ < 20)
	{
		Serial.println("Attempting MQTT connection...");
		// Attempt to connect
		if (MQTTclient.connect(clientId.c_str()))
		{
			Serial.println("connected");
			// Once connected, publish an announcement...
			return;
		}
		delay(1000);
	}
	Serial.println("Error in MQTT Connection. NOT CONNECTED");
}


void SendMQTT(const char* Topic, int32_t payload, bool retained)
{
	if (!MQTTclient.connected())
		MQTT_Setup();

	char s[20];
	itoa(payload, s, 10);

	if (! MQTTclient.publish(Topic, s, retained))
		ESP.restart();
}

void SendMQTT(const char* Topic, float payload, bool retained)
{
	if (!MQTTclient.connected())
		MQTT_Setup();

	char s[20];
	dtostrf(payload, 4, 1, s);

	if (!MQTTclient.publish(Topic, s, retained))
		ESP.restart();
}

void SendMQTT(const char* Topic, char *payload, bool retained)
{
	if (!MQTTclient.connected())
		MQTT_Setup();

	if (! MQTTclient.publish(Topic, payload, retained))
		ESP.restart();
}

