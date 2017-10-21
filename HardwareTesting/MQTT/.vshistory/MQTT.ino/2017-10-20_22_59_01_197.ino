/*
 Name:		MQTT.ino
*/

#include <Ethernet.h>
#include <PubSubClient.h>
//#include <LiquidCrystal_SR_LCD3.h>

byte MAC_ADDRESS[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x31, 0xB8 };
#define MQTT_SERVER "10.20.30.60"
//IPAddress mqtt_server(10, 20, 30, 60);

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

unsigned long Time1 = 0, Time2 = 0;
volatile unsigned int c = 0;
//volatile char message_buff[50];

void mqtt_reconnect() {
	while (!mqttClient.connected()) // Loop until we're reconnected
	{
		Serial.print("[MQTT] Connecting...");
	//	lcd.setCursor(0, 1);
		if (mqttClient.connect("arduinoClient")) // Attempt to connect
		{
			Serial.println("connected!");
			mqttClient.publish("PingPong", "hello world");
		}
		else
		{
			Serial.print("failed, rc=");
			Serial.println(mqttClient.state());
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

void countImp(void)
{
	c++;
	String pubString = String(c);
	//pubString = "{\"idx\":7,\"nvalue\":0,\"svalue\":\"" + pubString + "\"}";
//pubString =   '{**idx*:7,**nvalue**:0,**svalue**:**' + pubString + '**}';
	pubString =  pubString;
	Serial.println(pubString);
	//pubString.toCharArray(message_buff, pubString.length() + 1);
	//mqttClient.publish("H2O/Count", message_buff);
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
		mqtt_reconnect();
	}
	Time1 = millis();
	if (Time1 - Time2 > 15000)
	{
		countImp();
		Time2 = Time1;
	}
	mqttClient.loop();

}
