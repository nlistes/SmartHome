/*
 Name:		StrOperations.ino
*/

volatile unsigned int c = 0;
unsigned long now = 0, sendTime = 0;
char message_buff[50];

void setup()
{
	Serial.println("Aiziet...");

}

void loop()
{
	now = millis();
	if (now - sendTime > 5000)
	{
		c++;
		String pubString = String(c);
		Serial.println(pubString);
		pubString = "{\"idx\":7,\"nvalue\":0,\"svalue\":\"" + pubString + "\"}";
		Serial.println(pubString);
		//pubString.toCharArray(message_buff, pubString.length() + 1);
		//Serial.println(message_buff);
		//Serial.println(pubString.c_str());
		sendTime = now;
	}

}
