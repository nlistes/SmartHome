/*
 Name:		MQTT.ino
*/

#include <UIPEthernet.h>
#include <PubSubClient.h>

byte MAC_ADDRESS[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x31, 0xB8 };
#define MQTT_SERVER "10.20.30.60"

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

unsigned long now = 0, sendTime = 0;


void setup()
{
	Ethernet.begin(MAC_ADDRESS);
	delay(1500);
	mqttClient.setServer(MQTT_SERVER, 1883);
}

void loop()
{
		while (!mqttClient.connected()) // Loop until we're reconnected
			if (!mqttClient.connect("arduinoClient")) // Attempt to connect
				delay(5000);
	now = millis();
	if (now - sendTime > 15000)
	{
		mqttClient.publish("domoticz/in", "{\"idx\":2,\"nvalue\":0,\"svalue\":\"1\"}");
		sendTime = now;
	}
	mqttClient.loop();

}
