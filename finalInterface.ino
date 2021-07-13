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

// Flags
bool initFlag = true;
bool menuFlag = false;
bool menuRenderFlag = true;
bool seedMenuFlag = false;
bool seedMenu1RenderFlag = true;
bool seedMenu2RenderFlag = false;
bool settingFlag = false;
bool quantifyFlag = false;
bool quantifyRenderFlag = true;
bool weightFlag = false;
bool weightRenderFlag = true;
bool countFlag = false;
bool countRenderFlag = true;
bool addFlag = false;
bool addRenderFlag = true;
bool liveFlag = false;
bool liveRenderFlag = true;

String w = "";
String v = "";

int counter = 0;
int packets = 0;

struct seed{
  String name;
  float dimension;
};

seed selectedSeed = {"<EMPTY>", 9};
seed seed1 = {"Corn", 12.72};
seed seed2 = {"Beans", 15.52};
seed seed3 = {"Okra", 6.21};
seed seed4 = {"Dill", 9};
seed seed5 = {"<EMPTY>", 9};
seed seed6 = {"<EMPTY>", 9};
seed seed7 = {"<EMPTY>", 9};
seed seed8 = {"<EMPTY>", 9};
seed seed9 = {"<EMPTY>", 9};
seed seed10 = {"<EMPTY>", 9};

//seed2.name = "Beans";
int lineNum=0; 
int cursorpos=0;
#define scrollSpeed 300

void loop() {
  if (stopLoop == false){
    if (initFlag){
      InitScreen();
    }
    else if (menuFlag){
      menu();
    }
    else if (seedMenuFlag){
      seedMenu();
    }
    else if (quantifyFlag){
      quantify();
    }
    else if (weightFlag){
      weight();  
    }
    else if (countFlag){
      count();  
    }
    else if (addFlag){
      add();
    }else if(liveFlag){
      live();
      proximitysensor();
    }
  }
}

char getInput(){
  char customKey = userKeypad.getKey();
  return customKey;
}

void scroll(char* msg){                                                                   //SCROLLING

    //SCROLL SPEED=300
  delay(scrollSpeed);
  
  int lenoftitle=strlen(msg);
  if (cursorpos==(lenoftitle - 1)) // for rollover effect
  {
    cursorpos=0;
  }
  lcd.setCursor(0,lineNum);
  if (cursorpos < lenoftitle-20)
  {
    for ( int ichar = cursorpos; ichar<cursorpos+20; ichar++)
    {
      lcd.print(msg[ichar]);
      }
   }
  else{
     for (int ichar = cursorpos; ichar<lenoftitle-1; ichar++){
       lcd.print(msg[ichar]);
      }
     for (int ichar = 0; ichar<= 20-(lenoftitle-cursorpos);ichar++){
      lcd.print(msg[ichar]);
      };
    }
  cursorpos++;
  
}

//////////////////////////////////////////////////////INITIAL SCREEN/////////////////////////////////////////////////
void InitScreen()
{  
  char * title = "SEED QUANTIFING & PACKAGING UNIT FOR SMALL SCALE ENTERPRISES  ";
  scroll(title);
  //Rest of the Lines
  lcd.setCursor(2,1);
  lcd.print(" Dept.ECE,PESCE");
  lcd.setCursor(1,3);
  lcd.print("PRESS # TO PROCEED ");
  if (getInput() == '#'){
    menuFlag = true;  
    initFlag = false;
  }
}


void menu(){
    if(menuRenderFlag){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("A. Start");
        lcd.setCursor(0,1);
        lcd.print("B. Settings");
        lcd.setCursor(0,2);
        lcd.print("C. Exit");
        menuRenderFlag=false; 
    }
    char* menuInput = getInput();
    if (menuInput == 'A'){
      seedMenuFlag = true;  
      menuFlag = false;
    }else if (menuInput == 'B'){
      settingFlag = true;  
      menuFlag = false;
    }else if (menuInput == 'C'){
      lcd.noDisplay();  
    }
  
}

///////////////////////////////////////////////////////////SEED MENU///////////////////////////////////////////////////////
void seedMenu(){
    if(seedMenu1RenderFlag){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("* Add New");
        lcd.setCursor(0,1);
        lcd.print("1.");
        lcd.setCursor(2,1);
        lcd.print(seed1.name);
        lcd.setCursor(0,2);
        lcd.print("2.");
        lcd.setCursor(2,2);
        lcd.print(seed2.name);
        lcd.setCursor(0,3);
        lcd.print("3.");
        lcd.setCursor(2,3);
        lcd.print(seed3.name);
        
        lcd.setCursor(10,1);
        lcd.print("4.");
        lcd.setCursor(12,1);
        lcd.print(seed4.name);
        lcd.setCursor(10,2);
        lcd.print("5.");
        lcd.setCursor(12,2);
        lcd.print(seed5.name);
        lcd.setCursor(10,3);
        lcd.print("B.");
        lcd.setCursor(12,3);
        lcd.print("Next");
        seedMenu1RenderFlag=false; 
    }else if(seedMenu2RenderFlag){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("* Add New");
        lcd.setCursor(0,1);
        lcd.print("6.");
        lcd.setCursor(2,1);
        lcd.print(seed6.name);
        lcd.setCursor(0,2);
        lcd.print("7.");
        lcd.setCursor(2,2);
        lcd.print(seed7.name);
        lcd.setCursor(0,3);
        lcd.print("8.");
        lcd.setCursor(2,3);
        lcd.print(seed8.name);
        
        lcd.setCursor(10,1);
        lcd.print("9.");
        lcd.setCursor(12,1);
        lcd.print(seed9.name);
        lcd.setCursor(10,2);
        lcd.print("0.");
        lcd.setCursor(12,2);
        lcd.print(seed10.name);
        lcd.setCursor(10,3);
        lcd.print("A.");
        lcd.setCursor(12,3);
        lcd.print("Back");
        seedMenu2RenderFlag=false; 
    }

    char menuInput = getInput();
    switch(int(menuInput)){
      case 'A':
        seedMenu2RenderFlag = false;
        seedMenu1RenderFlag = true;
        break;
      case 'B':
        seedMenu2RenderFlag = true;
        seedMenu1RenderFlag = false;
        break;
      case '1': 
        seedMenu2RenderFlag = false;
        seedMenu1RenderFlag = false; 
        quantifyFlag = true;
        quantifyRenderFlag = true;
        seedMenuFlag = false;
        selectedSeed = seed1;
        lcd.clear();
        break;
       case '2':  
        seedMenu2RenderFlag = false;
        seedMenu1RenderFlag = false;
        quantifyFlag = true;
        quantifyRenderFlag = true;
        seedMenuFlag = false;
        selectedSeed = seed2;
        lcd.clear();
        break;
       case '3':  
        seedMenu2RenderFlag = false;
        seedMenu1RenderFlag = false;
        quantifyFlag = true;
        quantifyRenderFlag = true;
        seedMenuFlag = false;
        selectedSeed = seed3;
        lcd.clear();
        break;
       case '4':  
        seedMenu2RenderFlag = false;
        seedMenu1RenderFlag = false;
        quantifyFlag = true;
        quantifyRenderFlag = true;
        seedMenuFlag = false;
        selectedSeed = seed4;
        lcd.clear();
        break;
       case '5':  
        seedMenu2RenderFlag = false;
        seedMenu1RenderFlag = false;
        quantifyFlag = true;
        quantifyRenderFlag = true;
        seedMenuFlag = false;
        selectedSeed = seed5;
        lcd.clear();
        break;
       case '6':  
        seedMenu2RenderFlag = false;
        seedMenu1RenderFlag = false;
        quantifyFlag = true;
        quantifyRenderFlag = true;
        seedMenuFlag = false;
        selectedSeed = seed6;
        lcd.clear();
        break;
       case '7':  
        seedMenu2RenderFlag = false;
        seedMenu1RenderFlag = false;
        quantifyFlag = true;
        quantifyRenderFlag = true;
        seedMenuFlag = false;
        selectedSeed = seed7;
        lcd.clear();
        break;
       case '8':  
        seedMenu2RenderFlag = false;
        seedMenu1RenderFlag = false;
        quantifyFlag = true;
        quantifyRenderFlag = true;
        seedMenuFlag = false;
        selectedSeed = seed8;
        lcd.clear();
        break;
       case '9':  
        seedMenu2RenderFlag = false;
        seedMenu1RenderFlag = false;
        quantifyFlag = true;
        quantifyRenderFlag = true;
        seedMenuFlag = false;
        selectedSeed = seed9;
        lcd.clear();
        break;
       case '0':  
        seedMenu2RenderFlag = false;
        seedMenu1RenderFlag = false;
        quantifyFlag = true;
        quantifyRenderFlag = true;
        seedMenuFlag = false;
        selectedSeed = seed10;
        lcd.clear();
        break;
       case '*':
        seedMenu2RenderFlag = false;
        seedMenu1RenderFlag = false;
        seedMenuFlag = false;
        addRenderFlag = true;
        addFlag = true;
        lcd.clear();
    }

}

void quantify(){
  if(quantifyRenderFlag){
    if (selectedSeed.name.equals("<EMPTY>")) {
      lcd.setCursor(0,0);
      lcd.print("Variety not found");
      lcd.setCursor(0,1);
      lcd.print("Add new");
      lcd.setCursor(0,2);
      lcd.print("A. PROCEED");
      lcd.setCursor(0,3);
      lcd.print("B. BACK");
    }else if(selectedSeed.dimension < 10){
      lcd.setCursor(0,0);
      lcd.print("ONLY WEIGHT OPTION");
      lcd.setCursor(0,1);
      lcd.print("AVAILABLE");
      lcd.setCursor(0,2);
      lcd.print("A. PROCEED");
      lcd.setCursor(0,3);
      lcd.print("B. BACK");
    }else{
      lcd.setCursor(0,0);
      lcd.print("Select Mode:");
      lcd.setCursor(0,1);
      lcd.print("A. Count");
      lcd.setCursor(0,2);
      lcd.print("B. Weight");
      lcd.setCursor(0,3);
      lcd.print("C. Back");
    }
      quantifyRenderFlag=false; 
   }

   char menuInput = getInput();
    if (selectedSeed.name.equals("<EMPTY>") && menuInput == 'A'){
      quantifyFlag = false;
      addRenderFlag = true;
      addFlag = true;
      lcd.clear();
    }else if(selectedSeed.name.equals("<EMPTY>") && menuInput == 'B'){
      quantifyFlag = false;
      seedMenu1RenderFlag = true;
      seedMenuFlag = true;
      lcd.clear();
    }else if(selectedSeed.dimension < 10 && menuInput == 'A'){
      quantifyFlag = false;
      weightFlag = true;
      lcd.clear();
    }else if (selectedSeed.dimension < 10 && menuInput == 'B'){
      quantifyFlag = false;
      seedMenu1RenderFlag = true;
      seedMenuFlag = true;
      lcd.clear();
    }else if (selectedSeed.dimension >= 10 && menuInput == 'A'){
      quantifyFlag = false;
      countFlag = true;
      lcd.clear();
    }else if (selectedSeed.dimension >= 10 && menuInput == 'B'){
      quantifyFlag = false;
      weightFlag = true;
      lcd.clear();
    }else if (selectedSeed.dimension >= 10 && menuInput == 'C'){
      quantifyFlag = false;
      seedMenu1RenderFlag = true;
      seedMenuFlag = true;
      lcd.clear();
    }
}

void weight(){  
  lcd.setCursor(0,0);
  lcd.print("INPUT WEIGHT(gms):");
  lcd.setCursor(0,3);
  lcd.print("Max: 250");
  lcd.setCursor(11,1);
  lcd.print("A. Accept");
  lcd.setCursor(11,2);
  lcd.print("B. Delete");
  lcd.setCursor(11,3);
  lcd.print("C. Back");

  char menuInput = getInput();

  if(menuInput){
    lcd.clear();
    if(menuInput >= '0' && menuInput <= '9'){
      w = w + menuInput;  
      lcd.setCursor(0,2);
      lcd.print(w);
    }else if (menuInput == 'B'){
      int lastIndex = w.length() - 1;
      w.remove(lastIndex); 
      lcd.setCursor(0,2);
      lcd.print(w);
    }if (menuInput == 'A'){
      weightFlag = false;
      liveFlag = true;
      lcd.clear();
    }else if (menuInput == 'C'){
      weightFlag = false;
      seedMenu1RenderFlag = true;
      seedMenuFlag = true;
      lcd.clear();
    }
    if(w.toInt() > 250){
      w = "250";  
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("INPUT WEIGHT(gms):");
      lcd.setCursor(0,3);
      lcd.print("Max: 250");
      lcd.setCursor(11,1);
      lcd.print("A. Accept");
      lcd.setCursor(11,2);
      lcd.print("B. Delete");
      lcd.setCursor(11,3);
      lcd.print("C. Back");
      lcd.setCursor(0,2);
      lcd.print(w);
    }
  }
}

void count(){  
  lcd.setCursor(0,0);
  lcd.print("INPUT COUNT:");
  lcd.setCursor(0,3);
  lcd.print("Max: 50");
  lcd.setCursor(11,1);
  lcd.print("A. Accept");
  lcd.setCursor(11,2);
  lcd.print("B. Delete");
  lcd.setCursor(11,3);
  lcd.print("C. Back");

  char menuInput = getInput();

  if(menuInput){
    lcd.clear();
    if(menuInput >= '0' && menuInput <= '9'){
      w = w + menuInput;  
      lcd.setCursor(0,2);
      lcd.print(w);
    }else if (menuInput == 'B'){
      int lastIndex = w.length() - 1;
      w.remove(lastIndex); 
      lcd.setCursor(0,2);
      lcd.print(w);
    }else if (menuInput == 'A'){
      countFlag = false;
      liveFlag = true;
      lcd.clear();
    }else if (menuInput == 'C'){
      countFlag = false;
      seedMenu1RenderFlag = true;
      seedMenuFlag = true;
      lcd.clear();
    }
    if(w.toInt() > 50){
      w = "50";  
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("INPUT COUNT");
      lcd.setCursor(0,3);
      lcd.print("Max: 50");
      lcd.setCursor(11,1);
      lcd.print("A. Accept");
      lcd.setCursor(11,2);
      lcd.print("B. Delete");
      lcd.setCursor(11,3);
      lcd.print("C. Back");
      lcd.setCursor(0,2);
      lcd.print(w);
    }
  }
}

void add(){
  lcd.setCursor(0,0);
  lcd.print("Add new variety:");
  lcd.setCursor(0,2);
  lcd.print("Name:");
  lcd.setCursor(0,3);
  lcd.print("Dimension:");

  char menuInput = getInput();

  if(menuInput){
    lcd.clear();
    if(menuInput >= '0' && menuInput <= '9'){
      w = w + menuInput;  
      lcd.setCursor(0,2);
      lcd.print(w);
    }else if (menuInput == 'B'){
      int lastIndex = w.length() - 1;
      w.remove(lastIndex); 
      lcd.setCursor(0,2);
      lcd.print(w);
    }if (menuInput == 'A'){
      quantifyFlag = false;
      weightFlag = true;
      lcd.clear();
    }else if (menuInput == 'C'){
      weightFlag = false;
      seedMenu1RenderFlag = true;
      seedMenuFlag = true;
      lcd.clear();
    }
    if(w.toInt() > 250){
      w = "250";  
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("INPUT WEIGHT(gms):");
      lcd.setCursor(0,3);
      lcd.print("Max: 250");
      lcd.setCursor(11,1);
      lcd.print("A. Accept");
      lcd.setCursor(11,2);
      lcd.print("B. Delete");
      lcd.setCursor(11,3);
      lcd.print("C. Back");
      lcd.setCursor(0,2);
      lcd.print(w);
    }
  }
}

void live(){
  lcd.setCursor(0,0);
  lcd.print("LIVE FEED ");
  lcd.setCursor(0,2);
  lcd.print("Count:");
  lcd.setCursor(0,3);
  lcd.print("Packets:");
  lcd.setCursor(13,2);
  lcd.print("A. Done");
  lcd.setCursor(13,3);
  lcd.print("B. Back");

  lcd.setCursor(7,2);
  lcd.print(counter);
  lcd.setCursor(9,3);
  lcd.print(packets);

  char* menuInput = getInput();
  if (menuInput == 'A'){
    liveFlag = false;  
    menuRenderFlag = true;
    menuFlag = true;
    counter = 0;
    packets = 0;
    w = "";
  }else if (menuInput == 'B'){
    seedMenuFlag = true;  
    seedMenu1RenderFlag = true;
    liveFlag = false;
    counter = 0;
    packets = 0;
    w = "";
  }
}

void proximitysensor(){
  if(counter % w.toInt() == 0){
    counter = 0;
    packets = packets + 1;
    lcd.clear();
    
  }
  counter = counter + 1;
  delay(500);
}
