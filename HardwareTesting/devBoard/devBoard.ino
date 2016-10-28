/*
 Name:		devBoard.ino
*/

#include <UIPEthernet.h>
#include <LiquidCrystal_SR_LCD3.h>

byte MAC_ADDRESS[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x31, 0xB8 };

EthernetClient ethClient;
LiquidCrystal_SR_LCD3 lcd(A2, A0, A1);

void setup()
{
	lcd.begin(16, 2);        // set up the LCD's number of columns and rows
	lcd.print("Getting IP...");
	lcd.setCursor(0, 1);
	Ethernet.begin(MAC_ADDRESS);
	lcd.print(Ethernet.localIP());
}

void loop()
{

}
