/*
 Name:		DanfosMotorControl.ino
 Created:	7/8/2016 10:52:26 PM
 Author:	Lietotajs
*/

// MC connected pins
#define PIN_OPEN 4
#define PIN_IS_OPENED 5
#define PIN_CLOSE 6
#define PIN_IS_CLOSED 7

// Status constants
#define STATUS_READY	0	//	Gatavs darbibai, stav ne gala poziicijas
#define STATUS_CLOSE	1	//	Aizverts, motors izslegts
#define STATUS_OPEN		2	//	Atverts, motors izslegts
#define STATUS_CLOSING	3	//	Aizveras, motors ieslegts
#define STATUS_OPENING	4	//	Atveras, motors ieslegts

// Action constants
#define ACTION_NO		10
#define ACTION_CLOSE	11	//	Atvert
#define ACTION_OPEN		12	//	Aizvert
#define ACTION_STOP		13	//	Apturet

byte action = 0;
byte status = 0;

static word value;  // Value for handle input


// the setup function runs once when you press reset or power the board
void setup()
{
	Serial.begin(115000);
	pinMode(PIN_OPEN, OUTPUT);
	digitalWrite(PIN_OPEN, LOW);
	pinMode(PIN_CLOSE, OUTPUT);
	digitalWrite(PIN_CLOSE, LOW);
	pinMode(PIN_IS_OPENED, INPUT);
	pinMode(PIN_IS_CLOSED, INPUT);
}

// the loop function runs over and over again until power down or reset
void loop()
{
	status = getStatus();
	if (action == ACTION_CLOSE && (status == STATUS_READY || status == STATUS_OPEN)) digitalWrite(PIN_CLOSE, HIGH);
	if (action == ACTION_OPEN && (status == STATUS_READY || status == STATUS_CLOSE)) digitalWrite(PIN_OPEN, HIGH);
	if (action == ACTION_STOP && status == STATUS_OPENING)	digitalWrite(PIN_OPEN, LOW);
	if (action == ACTION_STOP && status == STATUS_CLOSING)	digitalWrite(PIN_CLOSE, LOW);
	action = ACTION_NO;
	if (Serial.available())
	{
		noInterrupts();
		handleInput(Serial.read());
		interrupts();
	}
}

int getStatus()
{
	int result = 0;

	if (digitalRead(PIN_IS_CLOSED) == 0)
	{
		digitalWrite(PIN_CLOSE, LOW);
		result = STATUS_CLOSE;
	}
	if (digitalRead(PIN_IS_OPENED) == 0)
	{
		digitalWrite(PIN_OPEN, LOW);
		result = STATUS_OPEN;
	}
	if (digitalRead(PIN_CLOSE) == 1) result = STATUS_CLOSING;
	if (digitalRead(PIN_OPEN) == 1) result = STATUS_OPENING;
	//if (digitalRead(IS_CLOSED_PIN) == 0 && digitalRead(IS_OPENED_PIN) == 0 && digitalRead(OPEN_PIN) == 0 && digitalRead(CLOSE_PIN) == 0)	  result = Ready;
	return result;

}

static void handleInput(char c)
{
	if (c >= '0' && c <= '9')
	{
		value = 10 * value + c - '0';
		//Serial.println(value);
		return;
	}
	if (c >= 'a' && c <= 'z')
	{
		//        Serial.print(value);
		//        Serial.println(c);
	}

	if (c > ' ')
	{
		switch (c)
		{
		case 'c':
			Serial.println("Closing valve...");
			action = ACTION_CLOSE;
			break;
		case 'o':
			Serial.println("Opening valve...");
			action = ACTION_OPEN;
			break;
		case 's':
			Serial.println("STOPING...");
			action = ACTION_STOP;
			break;
		case 'm':
			//opendelay = value;
			break;
		default:
			Serial.println("Command don't understud!");
			Serial.println();
		}
	}
	value = 0;
}

