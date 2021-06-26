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
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

/////////////////////////////////////////////////////Keypad & LCD Objects//////////////////////////////////////////////
                                          
                                         //****Keypad--> userKeypad , LCD-->lcd****//
// keypad object
Keypad userKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 
// LCD object
LiquidCrystal_I2C lcd(0x27, 20, 4);

////////////////////////////////////////////////////////VOID SETUP/////////////////////////////////////////////////////
void setup() {
  // Setup LCD with backlight and initialize
  lcd.backlight();
  lcd.begin();

}

////////////////////////////////////////////////////////SCROLL LEFT TO RIGHT///////////////////////////////////////////////////
int Li          = 20;
int Lii         = 1; 
int Ri          = -1;
int Rii         = -1;

String Scroll_LCD_Left(String StrDisplay){
  String result;
  String StrProcess = "                " + StrDisplay + "                ";
  result = StrProcess.substring(Li,Lii);
  Li++;
  Lii++;
  if (Li>StrProcess.length()){
    Li=19;
    Lii=0;
  }
  return result;
}

void Clear_Scroll_LCD_Left(){
  Li=20;
  Lii=0;
}
