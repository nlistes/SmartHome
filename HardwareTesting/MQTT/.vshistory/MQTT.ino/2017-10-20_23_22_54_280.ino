/*
 Name:		MQTT.ino
*/

#include <Ethernet.h>
#include <PubSubClient.h>

byte MAC_ADDRESS[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x31, 0xB8 };
#define MQTT_SERVER "10.20.30.60"

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);
long lastReconnectAttempt = 0;

unsigned long Time1 = 0, Time2 = 0;
volatile unsigned int c = 0;
//volatile char message_buff[50];

boolean mqttReconnect()
{
	if (mqttClient.connect("arduinoClient"))
	{
		// Once connected, publish an announcement...
		mqttClient.publish("outTopic", "hello world");
		// ... and resubscribe
		mqttClient.subscribe("inTopic");
	}
	return mqttClient.connected();

	//while (!mqttClient.connected()) // Loop until we're reconnected
	//{
	//	Serial.print("[MQTT] Connecting...");
	//	if (mqttClient.connect("arduinoClient")) // Attempt to connect
	//	{
	//		Serial.println("connected!");
	//		mqttClient.publish("PingPong", "hello world");
	//	}
	//	else
	//	{
	//		Serial.print("failed, rc=");
	//		Serial.println(mqttClient.state());
	//		// Wait 5 seconds before retrying
	//		delay(5000);
	//	}
	//}
}

void countImp(void)
{
	c++;
	String pubString = String(c);
	pubString = "{\"idx\":7,\"nvalue\":0,\"svalue\":\"" + pubString + "\"}";
	//pubString =  pubString;
	Serial.println(pubString);
	mqttClient.publish("H2O/Count", pubString.c_str());
}

void setup()
{
	Serial.begin(115200);
	Serial.println("Getting IP...");
	Ethernet.begin(MAC_ADDRESS);
	Serial.print("localIP: ");
	Serial.println(Ethernet.localIP());
	Serial.print("subnetMask: ");
	Serial.println(Ethernet.subnetMask());
	Serial.print("gatewayIP: ");
	Serial.println(Ethernet.gatewayIP());
	Serial.print("dnsServerIP: ");
	Serial.println(Ethernet.dnsServerIP());
	mqttClient.setServer(MQTT_SERVER, 1883);
}

void loop()
{
	if (!mqttClient.connected())
	{
		long now = millis();
		if (now - lastReconnectAttempt > 5000)
		{
			lastReconnectAttempt = now;
			// Attempt to reconnect
			if (mqttReconnect())
			{
				lastReconnectAttempt = 0;
			}
		}
	}
	//if (!mqttClient.connected())
	//{
	//	mqtt_reconnect();
	//}
	Time1 = millis();
	if (Time1 - Time2 > 15000)
	{
		countImp();
		Time2 = Time1;
	}
	mqttClient.loop();

}
