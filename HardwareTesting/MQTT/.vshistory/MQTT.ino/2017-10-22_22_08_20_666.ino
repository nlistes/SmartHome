/*
 Name:		MQTT.ino
*/

#include <Ethernet.h>
#include <PubSubClient.h>
#include <Bounce2.h>

//#define MQTT_ON
#define IOT_ON

long now = 0;

EthernetClient ethClient;
byte MAC_ADDRESS[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x31, 0xB8 };

#define MQTT_SERVER "10.20.30.60"
PubSubClient mqttClient(ethClient);
unsigned long mqttlastReconnectAttempt = 0;

#define IOT_SERVER "mqtt.thingspeak.com"
#define CHANELL_ID "344138"
#define WRITE_API_KEY "MVZK45X89XJ2E8EZ"
PubSubClient iotClient(ethClient);
unsigned long iotlastReconnectAttempt = 0;
unsigned long iotlastSendTime = 0;

#define COUNTER_INPUT_PIN 2
Bounce debouncerInputPin = Bounce();
unsigned int counterInputCount = 0;
unsigned long counterInputTime = 0;
unsigned int counterInputFlow = 0;

#define COUNTER_COLLECTOR_PIN 3
Bounce debouncerCollectorPin = Bounce();
unsigned int counterCollectorCount = 0;
unsigned long counterCollectorTime = 0;

#define COUNTER_WARMFLOOR_PIN 4
Bounce debouncerWarmfloorPin = Bounce();
unsigned int counterWarmfloorCount = 0;
unsigned long counterWarmfloorTime = 0;

#define COUNTER_FIRSTFLOOR_PIN 5
Bounce debouncerFirstfloorPin = Bounce();
unsigned int counterFirstfloorCount = 0;
unsigned long counterFirstfloorTime = 0;

#define COUNTER_SECONDFLOOR_PIN 6
Bounce debouncerSecondfloorPin = Bounce();
unsigned int counterSecondfloorCount = 0;
unsigned long counterSecondfloorTime = 0;

boolean mqttReconnect() {
	if (mqttClient.connect("arduinoClient")) {
		Serial.println("connected!");
		// Once connected, publish an announcement...
		mqttClient.publish("outTopic", "hello world");
		// ... and resubscribe
		//mqttClient.subscribe("inTopic");
	}
	return mqttClient.connected();
}

boolean iotReconnect() {
	if (iotClient.connect("arduinoClient")) {
		Serial.println("connected!");
		// Once connected, publish an announcement...
		iotClient.publish("outTopic", "hello world");
	}
	return iotClient.connected();
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
	iotClient.setServer(IOT_SERVER, 1883);
}

void loop()
{
#ifdef MQTT_ON
	if (!mqttClient.connected()) {
		now = millis();
		if (now - mqttlastReconnectAttempt > 5000) {
			mqttlastReconnectAttempt = now;
			// Attempt to reconnect
			Serial.print("[MQTT] Connecting...");
			if (mqttReconnect()) { mqttlastReconnectAttempt = 0; }
		}
	}
	else {
		// Client connected
		mqttClient.loop();
	}
#endif // MQTT_ON
#ifdef IOT_ON
	if (!iotClient.connected()) {
		now = millis();
		if (now - iotlastReconnectAttempt > 15000) {
			iotlastReconnectAttempt = now;
			// Attempt to reconnect
			Serial.print("[IOT] Connecting...");
			if (iotReconnect()) { iotlastReconnectAttempt = 0; }
		}
	}
	else {
		// Client connected
		now = millis();
		if (now - iotlastSendTime > 15000) {
			//String data = String("field1=" + String(t, DEC) + "&field2=" + String(h, DEC) + "&field3=" + String(lightLevel, DEC));
			String iotString = String("field1=") + String(c / 1000);
			// Publish data to ThingSpeak. Replace <YOUR-CHANNEL-ID> with your channel ID and <YOUR-CHANNEL-WRITEAPIKEY> with your write API key
			Serial.println(iotString);
			iotClient.publish("channels/344138/publish/MVZK45X89XJ2E8EZ", iotString.c_str());

		}
		iotClient.loop();
	}
#endif // IOT_ON

	if (debouncerInputPin.update()) {
		if (debouncerInputPin.rose()) {
			now = millis();
			counterInputCount++;
			float b = (3600000.0 / (now - counterInputTime));
			counterInputFlow = b * 1000.0;
			Serial.print("Count/Input: ");
			Serial.println(counterInputCount);
			Serial.print("Flow/Input: ");
			Serial.print(c / 1000);
			Serial.print(".");
			Serial.println(c % 1000);
#ifdef MQTT_ON
			String pubString = String(counterInputCount);
			mqttClient.publish("Count/Input", pubString.c_str());
			pubString = String(c / 1000);
			mqttClient.publish("Flow/Input", pubString.c_str());
#endif // MQTT_ON
#ifdef IOT_ON
			if (now - counterInputTime > 15000)
			{
			}
#endif // IOT_ON
			counterInputTime = now;
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
