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
			Serial.print("Count/Input: ");
			Serial.println(counterInputCount);
			Serial.print("Flow/Input: ");
			Serial.print(c / 1000);
			Serial.print(".");
			Serial.println(c % 1000);
			counterInputTime = now;
			String pubString = String(counterInputCount);
			mqttClient.publish("Count/Input", pubString.c_str());
			pubString = String(c / 1000);
			mqttClient.publish("Flow/Input", pubString.c_str());
		}
	}
	if (debouncerCollectorPin.update()) {
		if (debouncerCollectorPin.rose()) {
			now = millis();
			counterCollectorCount++;
			float b = (3600000.0 / (now - counterCollectorTime));
			unsigned long c = b * 1000.0;
			Serial.print("Count/Collector: ");
			Serial.println(counterCollectorCount);
			Serial.print("Flow/Collector: ");
			Serial.print(c / 1000);
			Serial.print(".");
			Serial.println(c % 1000);
			counterCollectorTime = now;
			String pubString = String(counterCollectorCount);
			mqttClient.publish("Count/Collector", pubString.c_str());
			pubString = String(c / 1000);
			mqttClient.publish("Flow/Collector", pubString.c_str());
		}
	}
	if (debouncerWarmfloorPin.update()) {
		if (debouncerWarmfloorPin.rose()) {
			now = millis();
			counterWarmfloorCount++;
			float b = (3600000.0 / (now - counterWarmfloorTime));
			unsigned long c = b * 1000.0;
			Serial.print("Count/Warmfloor: ");
			Serial.println(counterWarmfloorCount);
			Serial.print("Flow/Warmfloor: ");
			Serial.print(c / 1000);
			Serial.print(".");
			Serial.println(c % 1000);
			counterWarmfloorTime = now;
			String pubString = String(counterWarmfloorCount);
			mqttClient.publish("Count/Warmfloor", pubString.c_str());
			pubString = String(c / 1000);
			mqttClient.publish("Flow/Warmfloor", pubString.c_str());
		}
	}
	if (debouncerFirstfloorPin.update()) {
		if (debouncerFirstfloorPin.rose()) {
			now = millis();
			counterFirstfloorCount++;
			float b = (3600000.0 / (now - counterFirstfloorTime));
			unsigned long c = b * 1000.0;
			Serial.print("Count/Firstfloor: ");
			Serial.println(counterFirstfloorCount);
			Serial.print("Flow/Firstfloor: ");
			Serial.print(c / 1000);
			Serial.print(".");
			Serial.println(c % 1000);
			counterFirstfloorTime = now;
			String pubString = String(counterFirstfloorCount);
			mqttClient.publish("Count/Firstfloor", pubString.c_str());
			pubString = String(c / 1000);
			mqttClient.publish("Flow/Firstfloor", pubString.c_str());
		}
	}
	if (debouncerSecondfloorPin.update()) {
		if (debouncerSecondfloorPin.rose()) {
			now = millis();
			counterSecondfloorCount++;
			float b = (3600000.0 / (now - counterSecondfloorTime));
			unsigned long c = b * 1000.0;
			Serial.print("Count/Secondfloor: ");
			Serial.println(counterSecondfloorCount);
			Serial.print("Flow/Secondfloor: ");
			Serial.print(c / 1000);
			Serial.print(".");
			Serial.println(c % 1000);
			counterSecondfloorTime = now;
			String pubString = String(counterSecondfloorCount);
			mqttClient.publish("Count/Secondfloor", pubString.c_str());
			pubString = String(c / 1000);
			mqttClient.publish("Flow/Secondfloor", pubString.c_str());
		}
	}
}
