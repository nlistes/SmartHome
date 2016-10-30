/*
 Name:		StrOperations.ino
*/

volatile unsigned int c = 0;
unsigned long Time1 = 0, Time2 = 0;
char message_buff[50];

void setup()
{
	Serial.println("Aiziet...");

}

void loop()
{
	Time1 = millis();
	if (Time1 - Time2 > 5000)
	{
		c++;
		String pubString = String(c);
		Serial.println(pubString);
		pubString = "{\"idx\":7,\"nvalue\":0,\"svalue\":\"" + pubString + "\"}";
		Serial.println(pubString);
		//pubString.toCharArray(message_buff, pubString.length() + 1);
		//Serial.println(message_buff);
		//Serial.println(pubString.c_str());
		Time2 = Time1;
	}

}
