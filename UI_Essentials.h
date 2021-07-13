///////////////////////////////////////////////////////////LCD LIBRARIES/////////////////////////////////////////////////
// Include Arduino Wire library for I2C
#include <Wire.h>

// Include LCD display library for I2C
#include <LiquidCrystal_I2C.h>

////////////////////////////////////////////////////////KeyPad Initialization/////////////////////////////////////////////
// Include Keypad library
#include <Keypad.h>

// Constants for row and column sizes
const byte ROWS = 4;
const byte COLS = 4;

// Array to represent keys on keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Connections to Arduino
byte rowPins[ROWS] = {52, 50, 48, 46};
byte colPins[COLS] = {44, 42, 40, 38};

/////////////////////////////////////////////////////Keypad & LCD Objects//////////////////////////////////////////////
                                          
                                         //****Keypad--> userKeypad , LCD-->lcd****//
// keypad object
Keypad userKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 
// LCD object
LiquidCrystal_I2C lcd(0x27, 20, 4); //SCL:A5, SDA:A4

////////////////////////////////////////////////////////VOID SETUP/////////////////////////////////////////////////////
void setup() {
  // Setup LCD with backlight and initialize
  Serial.begin(9600);
  lcd.backlight();
  lcd.begin();

}
