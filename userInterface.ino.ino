#include "UI_Essentials.h"

// Length of input + 1 for null character
#define lenInput 3

// Character to hold user input
char Data[lenInput];

// MasterInput for Clearing   <----- ////////////////////////NEEDS TO BE ALTERED///////////////////////
char Master[lenInput] = "CC";

// Counter for character entries
byte data_count = 0; 

// Character to hold key input // ONE CHARACTER
char pressedKey;

//Boolean to break out of loop
bool stopLoop = false; 

char * title = "SEED QUANTIFING & PACKAGING UNIT FOR SMALL SCALE ENTERPRISES  ";
int lineNum=0; 
int cursorpos=0;
#define scrollSpeed 300

void loop() {
  if (stopLoop == false){
      InitScreen();
  }
}

//////////////////////////////////////////////////////INITIAL SCREEN/////////////////////////////////////////////////
//void InitScreen(){
//  lcd.setCursor(0,0);  
//  for (int i = 0; i <= 30; i++) {
//  lcd.setCursor(0,0);
//  lcd.print(Scroll_LCD_Left("Seed Counting & Packing Machine"));
//  delay(200);}
//  lcd.setCursor(5,1);
//  lcd.print(" Group 35 ");
//  lcd.setCursor(0,3);
//  lcd.print("PRESS # KEY TO BEGIN ");
//}
void InitScreen()
{ 
  //SCROLL SPEED=300
  delay(scrollSpeed);
  
  int lenoftitle=strlen(title);
  if (cursorpos==(lenoftitle - 1)) // for rollover effect
  {
    cursorpos=0;
  }
  lcd.setCursor(0,lineNum);
  if (cursorpos < lenoftitle-20)
  {
    for ( int ichar = cursorpos; ichar<cursorpos+20; ichar++)
    {
      lcd.print(title[ichar]);
      }
   }
  else{
     for (int ichar = cursorpos; ichar<lenoftitle-1; ichar++){
       lcd.print(title[ichar]);
      }
     for (int ichar = 0; ichar<= 20-(lenoftitle-cursorpos);ichar++){
      lcd.print(title[ichar]);
      };
    }
  cursorpos++;
  
  //Rest of the Lines
  lcd.setCursor(5,1);
  lcd.print(" Group 35 ");
  lcd.setCursor(1,3);
  lcd.print("PRESS # TO PROCEED ");
}
///////////////////////////////////////////////////////////SEED MENU///////////////////////////////////////////////////////
void seedMenu(char choice){
    if (choice=='#'){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Select Seed Variety");
    lcd.setCursor(0,2);
    lcd.print("1.CORN");
    lcd.setCursor(14,2);
    lcd.print("2.OKRA");
    lcd.setCursor(0,3);
    lcd.print("3.BEANS");
    lcd.setCursor(14,3);
    lcd.print("4.DILL");
    exit;
      }
    else{
      lcd.clear();
      lcd.setCursor(0,3);
      lcd.print("Invalid Input");
      InitScreen();
      }
}
//////////////////////////////////////////////////////////SEED CHOICE////////////////////////////////////////////////////////
void seedChoice(char choice){

  lcd.clear();
  lcd.setCursor(0,0);
  switch(choice){
    case '1':
    lcd.print("Choice : Maize");
    break;
    case '2':
    lcd.print("Choice : Okra");
    case '3':
    lcd.print("Choice : Beans");
    break;
    case '4':
    lcd.print("Choice : Dill");
    break;
  }
  lcd.setCursor(0,1);
  lcd.print("00 - Clear Choice" );
  lcd.setCursor(0,2);
  lcd.print(" * - By Weight");
  lcd.setCursor(0,3);
  lcd.print(" # - By Count");
}
///////////////////////////////////////////////////////////LIVE FEED/////////////////////////////////////////////////////////////
void livefeed(char weigh_count,float seedCount,float weight){
  lcd.setCursor(2,0);
  lcd.print("   LIVE FEED ");
  lcd.setCursor(0,1);
  lcd.print("Count:");
  lcd.setCursor(0,3);
  lcd.print("Weight:");
  }
// ///////////////////////////////////////////////////////BY COUNT /////////////////////////////////////////////////////////////
int bycount(){
  int seedcount;
  int  noseed;
  lcd.setCursor(1,0);
  lcd.print("Counting By Number");
  lcd.setCursor(0,1);
  lcd.print("Enter No of Seeds");
  lcd.setCursor(0,3);
  lcd.print("0-Clear    D-Proceed");
  return seedcount
  }
/////////////////////////////////////////////////////////BY WEIGHT /////////////////////////////////////////////////////////////
float byweight(){
  float weight;
  int  noseed;
  lcd.setCursor(1,0);
  lcd.print("Counting By Number");
  lcd.setCursor(0,1);
  lcd.print("Req. Seed Weight:");
  lcd.setCursor(0,3);
  lcd.print("0-Clear    D-Proceed");
  return weight
  }
////////////////////////////////////////////////////////FINAL SCREEN/////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////USER INPUT///////////////////////////////////////////////////////////////
