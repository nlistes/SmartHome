/*
 Name:		MQTT.ino
*/

#include <UIPEthernet.h>
#include <PubSubClient.h>
//#include <LiquidCrystal_SR_LCD3.h>

byte MAC_ADDRESS[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x31, 0xB8 };
//#define MQTT_SERVER "10.20.30.60"
IPAddress mqtt_server(10, 20, 30, 60);

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);
//LiquidCrystal_SR_LCD3 lcd(A2, A0, A1);

unsigned long Time1 = 0, Time2 = 0;
volatile unsigned int c = 0;
//volatile char message_buff[50];

void mqtt_reconnect() {
	while (!mqttClient.connected()) // Loop until we're reconnected
	{
	//	lcd.clear();
	//	lcd.print("Connecting...");
		Serial.print("Connecting...");
	//	lcd.setCursor(0, 1);
		if (mqttClient.connect("arduinoClient")) // Attempt to connect
		{
	//		lcd.print("connected!");
			Serial.println("connected!");
			mqttClient.publish("PingPong", "hello world");
		}
		else
		{
	//		lcd.print("failed, rc=");
			Serial.print("failed, rc=");
	//		lcd.print(mqttClient.state());
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
	//lcd.setCursor(11, 1);
	//lcd.print(pubString);
	//pubString = "{\"idx\":7,\"nvalue\":0,\"svalue\":\"" + pubString + "\"}";
//pubString =   '{**idx*:7,**nvalue**:0,**svalue**:**' + pubString + '**}';
	pubString = "123456789012345 " + pubString;
	Serial.println(pubString);
	//pubString.toCharArray(message_buff, pubString.length() + 1);
	//mqttClient.publish("H2O/Count", message_buff);
	//mqttClient.publish("H2O/Count", pubString.c_str());
}

void setup()
{
	//lcd.begin(16, 2);        // set up the LCD's number of columns and rows
	//lcd.print("Getting IP...");
	Serial.begin(115200);
	Serial.println("Getting IP...");
	//lcd.setCursor(0, 1);
	Ethernet.begin(MAC_ADDRESS);
	delay(1500);
	//lcd.print(Ethernet.localIP());
	Serial.println(Ethernet.localIP());
	mqttClient.setServer(mqtt_server, 1883);
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
