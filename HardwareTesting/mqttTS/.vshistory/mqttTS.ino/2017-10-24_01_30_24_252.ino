/*
Name:		mqttTS.ino
*/

#include <Ethernet.h>
#include <PubSubClient.h>
#include <Bounce2.h>


long now = 0;

EthernetClient ethClient;
byte MAC_ADDRESS[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x31, 0xB8 };

#define IOT_SERVER "mqtt.thingspeak.com"
#define CHANELL_ID "344138"
#define WRITE_API_KEY "MVZK45X89XJ2E8EZ"
PubSubClient iotClient(ethClient);
unsigned long iotlastReconnectAttempt = 0;
unsigned long iotlastSendTime = 0;
bool addAmp = false;

#define COUNTER_INPUT_PIN 2
Bounce debouncerInputPin = Bounce();
bool counterInputUpdated = false;
unsigned int counterInputCount = 0;
unsigned long counterInputTime = 0;
unsigned long counterInputFlow = 0;

#define COUNTER_COLLECTOR_PIN 3
Bounce debouncerCollectorPin = Bounce();
bool counterCollectorUpdated = false;
unsigned int counterCollectorCount = 0;
unsigned long counterCollectorTime = 0;
unsigned long counterCollectorFlow = 0;

#define COUNTER_WARMFLOOR_PIN 4
Bounce debouncerWarmfloorPin = Bounce();
bool counterWarmfloorUpdated = false;
unsigned int counterWarmfloorCount = 0;
unsigned long counterWarmfloorTime = 0;
unsigned long counterWarmfloorFlow = 0;

#define COUNTER_FIRSTFLOOR_PIN 5
Bounce debouncerFirstfloorPin = Bounce();
bool counterFirstfloorUpdated = false;
unsigned int counterFirstfloorCount = 0;
unsigned long counterFirstfloorTime = 0;
unsigned long counterFirstfloorFlow = 0;

#define COUNTER_SECONDFLOOR_PIN 6
Bounce debouncerSecondfloorPin = Bounce();
bool counterSecondfloorUpdated = false;
unsigned int counterSecondfloorCount = 0;
unsigned long counterSecondfloorTime = 0;
unsigned long counterSecondfloorFlow = 0;

boolean iotReconnect() {
	if (iotClient.connect("arduinoClient")) {
		Serial.println("connected!");
		// Once connected, publish an announcement...
		iotClient.publish("outTopic", "hello world");
	}
	return iotClient.connected();
}

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
	iotClient.setServer(IOT_SERVER, 1883);
}

void loop()
{
	if (!iotClient.connected()) {
		now = millis();
		if (now - iotlastReconnectAttempt > 15000) {
			iotlastReconnectAttempt = now;
			// Attempt to reconnect
			Serial.print("[IOT] Connecting...");
			iotReconnect();
			//if (iotReconnect()) { iotlastReconnectAttempt = 0; }
		}
	}
	else {
		// Client connected
		now = millis();
		addAmp = false;
		String iotString = String("");
		if (now - iotlastSendTime > 15000) {
			if (counterInputUpdated) {
				iotString = String("field1=") + String(counterInputFlow / 1000);
				addAmp = true;
				counterInputUpdated = false;
			}
			if (counterCollectorUpdated) {
				if (addAmp) { iotString = iotString + String("&");}
				iotString = iotString + String("field2=") + String(counterCollectorFlow / 1000);
				addAmp = true;
				counterCollectorUpdated = false;
			}
			if (counterWarmfloorUpdated) {
				if (addAmp) { iotString = iotString + String("&"); }
				iotString = iotString + String("field3=") + String(counterWarmfloorFlow / 1000);
				addAmp = true;
				counterWarmfloorUpdated = false;
			}
			//String data = String("field1=" + String(t, DEC) + "&field2=" + String(h, DEC) + "&field3=" + String(lightLevel, DEC));
			Serial.println(iotString);
			// Publish data to ThingSpeak. Replace <YOUR-CHANNEL-ID> with your channel ID and <YOUR-CHANNEL-WRITEAPIKEY> with your write API key
			iotClient.publish("channels/344138/publish/MVZK45X89XJ2E8EZ", iotString.c_str());
			iotlastSendTime = now;
		}
		iotClient.loop();
	}

	if (debouncerInputPin.update()) {
		if (debouncerInputPin.rose()) {
			now = millis();
			counterInputCount++;
			counterInputUpdated = true;
			float b = (3600000.0 / (now - counterInputTime));
			counterInputFlow = b * 1000.0;
			Serial.print("Count/Input: ");
			Serial.println(counterInputCount);
			Serial.print("Flow/Input: ");
			Serial.print(counterInputFlow / 1000);
			Serial.print(".");
			Serial.println(counterInputFlow % 1000);
			counterInputTime = now;
		}
	}
	//if (debouncerCollectorPin.update()) {
	//	if (debouncerCollectorPin.rose()) {
	//		now = millis();
	//		counterCollectorCount++;
	//		float b = (3600000.0 / (now - counterCollectorTime));
	//		unsigned long c = b * 1000.0;
	//		Serial.print("Count/Collector: ");
	//		Serial.println(counterCollectorCount);
	//		Serial.print("Flow/Collector: ");
	//		Serial.print(c / 1000);
	//		Serial.print(".");
	//		Serial.println(c % 1000);
	//		counterCollectorTime = now;
	//		String pubString = String(counterCollectorCount);
	//		mqttClient.publish("Count/Collector", pubString.c_str());
	//		pubString = String(c / 1000);
	//		mqttClient.publish("Flow/Collector", pubString.c_str());
	//	}
	//}
	//if (debouncerWarmfloorPin.update()) {
	//	if (debouncerWarmfloorPin.rose()) {
	//		now = millis();
	//		counterWarmfloorCount++;
	//		float b = (3600000.0 / (now - counterWarmfloorTime));
	//		unsigned long c = b * 1000.0;
	//		Serial.print("Count/Warmfloor: ");
	//		Serial.println(counterWarmfloorCount);
	//		Serial.print("Flow/Warmfloor: ");
	//		Serial.print(c / 1000);
	//		Serial.print(".");
	//		Serial.println(c % 1000);
	//		counterWarmfloorTime = now;
	//		String pubString = String(counterWarmfloorCount);
	//		mqttClient.publish("Count/Warmfloor", pubString.c_str());
	//		pubString = String(c / 1000);
	//		mqttClient.publish("Flow/Warmfloor", pubString.c_str());
	//	}
	//}
	//if (debouncerFirstfloorPin.update()) {
	//	if (debouncerFirstfloorPin.rose()) {
	//		now = millis();
	//		counterFirstfloorCount++;
	//		float b = (3600000.0 / (now - counterFirstfloorTime));
	//		unsigned long c = b * 1000.0;
	//		Serial.print("Count/Firstfloor: ");
	//		Serial.println(counterFirstfloorCount);
	//		Serial.print("Flow/Firstfloor: ");
	//		Serial.print(c / 1000);
	//		Serial.print(".");
	//		Serial.println(c % 1000);
	//		counterFirstfloorTime = now;
	//		String pubString = String(counterFirstfloorCount);
	//		mqttClient.publish("Count/Firstfloor", pubString.c_str());
	//		pubString = String(c / 1000);
	//		mqttClient.publish("Flow/Firstfloor", pubString.c_str());
	//	}
	//}
	//if (debouncerSecondfloorPin.update()) {
	//	if (debouncerSecondfloorPin.rose()) {
	//		now = millis();
	//		counterSecondfloorCount++;
	//		float b = (3600000.0 / (now - counterSecondfloorTime));
	//		unsigned long c = b * 1000.0;
	//		Serial.print("Count/Secondfloor: ");
	//		Serial.println(counterSecondfloorCount);
	//		Serial.print("Flow/Secondfloor: ");
	//		Serial.print(c / 1000);
	//		Serial.print(".");
	//		Serial.println(c % 1000);
	//		counterSecondfloorTime = now;
	//		String pubString = String(counterSecondfloorCount);
	//		mqttClient.publish("Count/Secondfloor", pubString.c_str());
	//		pubString = String(c / 1000);
	//		mqttClient.publish("Flow/Secondfloor", pubString.c_str());
	//	}
	//}
}
