/*
 Name:		MQTT.ino
*/

#include <Ethernet.h>
#include <PubSubClient.h>
#include <Bounce2.h>

long now = 0;

byte MAC_ADDRESS[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x31, 0xB8 };
#define MQTT_SERVER "10.20.30.60"

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);
unsigned long lastReconnectAttempt = 0;

#define COUNTER_INPUT_PIN 5
Bounce debouncerInputPin = Bounce();
unsigned int counterInputCount = 0;
unsigned long counterInputTime = 0;

#define COUNTER_COLLECTOR_PIN 6
Bounce debouncerCollectorPin = Bounce();
unsigned int counterCollectorCount = 0;
unsigned long counterCollectorTime = 0;

#define COUNTER_WARMFLOOR_PIN 7
Bounce debouncerWarmfloorPin = Bounce();
unsigned int counterWarmfloorCount = 0;
unsigned long counterWarmfloorTime = 0;

#define COUNTER_FIRSTFLOOR_PIN 8
Bounce debouncerFirstfloorPin = Bounce();
unsigned int counterFirstfloorCount = 0;
unsigned long counterFirstfloorTime = 0;

#define COUNTER_SECONDFLOOR_PIN 9
Bounce debouncerSecondfloorPin = Bounce();
unsigned int counterSecondfloorCount = 0;
unsigned long counterSecondfloorTime = 0;

boolean mqttReconnect() {
	if (mqttClient.connect("arduinoClient")) {
		Serial.println("connected!");
		// Once connected, publish an announcement...
		mqttClient.publish("outTopic", "hello world");
		// ... and resubscribe
		mqttClient.subscribe("inTopic");
	}
	return mqttClient.connected();
}

//void countImp(void) {
//	c++;
//	String pubString = String(c);
//	//pubString = "{\"idx\":7,\"nvalue\":0,\"svalue\":\"" + pubString + "\"}";
//	Serial.println(pubString);
//	mqttClient.publish("H2O/Count", pubString.c_str());
//}

void setup() {
	pinMode(COUNTER_INPUT_PIN, INPUT);
	debouncerInputPin.attach(COUNTER_INPUT_PIN);
	debouncerInputPin.interval(5);
	pinMode(COUNTER_COLLECTOR_PIN, INPUT);
	debouncerCollectorPin.attach(COUNTER_COLLECTOR_PIN);
	debouncerCollectorPin.interval(5);
	pinMode(COUNTER_WARMFLOOR_PIN, INPUT);
	debouncerWarmfloorPin.attach(COUNTER_WARMFLOOR_PIN);
	debouncerWarmfloorPin.interval(5);
	pinMode(COUNTER_FIRSTFLOOR_PIN, INPUT);
	debouncerFirstfloorPin.attach(COUNTER_FIRSTFLOOR_PIN);
	debouncerFirstfloorPin.interval(5);
	pinMode(COUNTER_SECONDFLOOR_PIN, INPUT);
	debouncerSecondfloorPin.attach(COUNTER_SECONDFLOOR_PIN);
	debouncerSecondfloorPin.interval(5);
	Serial.begin(115200);
	Serial.println("Getting IP...");
	Ethernet.begin(MAC_ADDRESS);
	Serial.print("localIP: ");		Serial.println(Ethernet.localIP());
	Serial.print("subnetMask: ");	Serial.println(Ethernet.subnetMask());
	Serial.print("gatewayIP: ");	Serial.println(Ethernet.gatewayIP());
	Serial.print("dnsServerIP: ");	Serial.println(Ethernet.dnsServerIP());
	mqttClient.setServer(MQTT_SERVER, 1883);
}

void loop()
{
	if (!mqttClient.connected()) {
		now = millis();
		if (now - lastReconnectAttempt > 5000) {
			lastReconnectAttempt = now;
			// Attempt to reconnect
			Serial.print("[MQTT] Connecting...");
			if (mqttReconnect()) {lastReconnectAttempt = 0;}
		}
	} else {
		// Client connected
		mqttClient.loop();
	}
	if (debouncerInputPin.update()) {
		if (debouncerInputPin.rose()) {
			now = millis();
			counterInputCount++;
			float b = (3600000.0 / (now - counterInputTime));
			unsigned long c = b * 1000.0;
			Serial.print("Count: ");
			Serial.println(counterInputCount);
			Serial.print("Flow: ");
			Serial.print(c / 1000);
			Serial.print(".");
			Serial.println(c % 1000);
			counterInputTime = now;
			String pubString = String(counterInputCount);
			mqttClient.publish("H2O/Count", pubString.c_str());
			pubString = String(c / 1000);
			mqttClient.publish("H2O/Flow", pubString.c_str());
		}
	}
	//Time1 = millis();
	//if (Time1 - Time2 > 15000)
	//{
	//	countImp();
	//	Time2 = Time1;
	//}
}
