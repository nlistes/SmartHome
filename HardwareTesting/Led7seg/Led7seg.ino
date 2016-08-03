/*
 Chip: ATmega8
 Working principle: Only one segment turned ON.
*/

// avr-libc library includes
#include <avr/io.h>
#include <avr/interrupt.h>

/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

/* x=target variable, y=mask */
#define BITMASK_SET(x,y) ((x) |= (y))
#define BITMASK_CLEAR(x,y) ((x) &= (~(y)))
#define BITMASK_FLIP(x,y) ((x) ^= (y))
#define BITMASK_CHECK(x,y) ((x) & (y))

#define digitsCount 4

volatile	uint8_t ledDigits[digitsCount];	// Output buffer for 7seg led display. Timer1 interrupt handler takes info from there to turn ON/OFF segments .
volatile	uint8_t workingSegment = 0;		// This segment will turn ON in next Timer1 interrupt handler iteration.

// Timer1 interrupt handler.
// This interrupt handler working with prepared output buffer (uint8_t ledDigits[digitsCount]) and only swich on/off segments
ISR(TIMER1_COMPA_vect)
{
	BITMASK_SET(PORTD, B11111111); // 7seg led display is with common anode. To turn OFF all segments of each digit, set pins 0-7 to HIGH
	// Prepare
	BITMASK_CLEAR(PORTD, BITMASK_CHECK(ledDigits[workingSegment / 8], 1 << workingSegment % 8)); // ? What is going here?!?

	BITMASK_CLEAR(PORTB, B1111); // set pin 8-11 to LOW
	BIT_SET(PORTB, workingSegment / 8);
	workingSegment++;
	if (workingSegment > 31) workingSegment = 0;
}

// The setup function runs once when you press reset or power the board
void setup()
{
	ledPrint("C.23.9");
	//Serial.begin(9600);
	BITMASK_SET(DDRD, B11111111);	// set PORT D (segment pins for led display) pins (0-7) to output
	BITMASK_SET(DDRB, B1111);		// set PORT B (common pins for led display) pins 8-11 to output

	// initialize Timer1
	noInterrupts();	// disable global interrupts
	TCCR1A = 0;		// set entire TCCR1A register to 0
	TCCR1B = 0;		// same for TCCR1B
	OCR1A = 5000;	// set compare match register to desired timer count
	TCCR1B |= (1 << WGM12);	// turn on CTC mode
	TCCR1B |= (1 << CS10);	// Set CS10 bits for 1 prescaler - start timer
	TIMSK |= (1 << OCIE1A);	// enable timer compare interrupt
	interrupts();	// enable global interrupts
}

// the loop function runs over and over again until power down or reset
void loop()
{
}

// Procedure to prepare output buffer for Timer1 interrupt handler.
void ledPrint(String ledBuffer)
{
	byte ledSigns[] =
	{
		B11111100, //00 - 0
		B01100000, //01 - 1
		B11011010, //02 - 2
		B11110010, //03 - 3
		B01100110, //04 - 4
		B10110110, //05 - 5
		B10111110, //06 - 6
		B11100000, //07 - 7
		B11111110, //08 - 8
		B11110110, //09 - 9
		B10011100, //10 - C
		B10011110, //11 - E
		B01101110, //12 - H
		B00011100, //13 - L
		B11001110, //14 - P
		B00000000  //15 - space
	};
	char c0, c1;
	uint8_t ledSymbolCounter = 0, j = 0;
	//Serial.println(ledBuffer.length());
	while (ledSymbolCounter < 4)
	{
		c0 = ' ';
		if (ledBuffer.length() - 1 >= j) c0 = ledBuffer.charAt(j);
		c1 = ' ';
		if (ledBuffer.length() - 1 >= j + 1) c1 = ledBuffer.charAt(j + 1);
		//Serial.println(c0);
		switch (c0)
		{
			case '0': ledDigits[ledSymbolCounter] = ledSigns[0]; break;
			case '1': ledDigits[ledSymbolCounter] = ledSigns[1]; break;
			case '2': ledDigits[ledSymbolCounter] = ledSigns[2]; break;
			case '3': ledDigits[ledSymbolCounter] = ledSigns[3]; break;
			case '4': ledDigits[ledSymbolCounter] = ledSigns[4]; break;
			case '5': ledDigits[ledSymbolCounter] = ledSigns[5]; break;
			case '6': ledDigits[ledSymbolCounter] = ledSigns[6]; break;
			case '7': ledDigits[ledSymbolCounter] = ledSigns[7]; break;
			case '8': ledDigits[ledSymbolCounter] = ledSigns[8]; break;
			case '9': ledDigits[ledSymbolCounter] = ledSigns[9]; break;
			case 'C': ledDigits[ledSymbolCounter] = ledSigns[10]; break;
			case 'E': ledDigits[ledSymbolCounter] = ledSigns[11]; break;
			case 'H': ledDigits[ledSymbolCounter] = ledSigns[12]; break;
			case 'L': ledDigits[ledSymbolCounter] = ledSigns[13]; break;
			case 'P': ledDigits[ledSymbolCounter] = ledSigns[14]; break;
			case '.': ledDigits[ledSymbolCounter] |= 1 << 0;      break;
			default:  ledDigits[ledSymbolCounter] = ledSigns[15];
		}
		j++;
		if (c1 != '.') ledSymbolCounter++;
	};
}
