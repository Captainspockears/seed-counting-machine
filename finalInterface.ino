



///////////////////////////////////////////////////////////LCD LIBRARIES/////////////////////////////////////////////////
// Include Arduino Wire library for I2C
#include <Wire.h>
#include <AFMotor.h>
#include <EEPROM.h>
// Include LCD display library for I2C
#include <LiquidCrystal_I2C.h>
void calibrate();

int buzzer  = 13;
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

///////////////////////////////////////////////////////////////////FLAP CONTROL/////////////////////////////////////////////////////////////////////////////////////

#define rpm 10                                           // stepper speed - flap control

const int stepsPerRevolution = 2048;
void open();
void close ();

/////////////////////////////////////////////////////////////////////VIBRATION///////////////////////////////////////////////////////////////////////////////////////

int intensity = 200;                                     // Motor speed / Vibration intensity
int duration;
int motor_state;                                        // Indicates state of motor, 0:OFF ; 1:ON

void vibrate(int intensity ,int duration = 1000);       // Function prototype  (Duration to be removed later)
void stop_vibrate();                                    // Function prototype  

/////////////////////////////////////////////////////Keypad & LCD Objects//////////////////////////////////////////////
                                          
                                         //****Keypad--> userKeypad , LCD-->lcd****//
// keypad object
Keypad userKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 
// LCD object
LiquidCrystal_I2C lcd(0x27, 20, 4); //SCL:A5, SDA:A4

#include "HX711.h"



#define LB2KG 453.52    // OLD 0.45352
#define CALWEIGHT 104   //OLD 0.104
#define DEFAULT_CALIFACTOR -30000


#define id 1
#define number_of_digits 2

#define DOUT  36
#define CLK 37

HX711 scale;
//LiquidCrystal_I2C lcd(0x27, 20, 4);

const byte addresses[][6] = {"00001", "00002"};
long currentOffset;

float calibration_factor;


///////////////////////////////////////////////////////////////////////////////////Prototypes///////////////////////////////////////////////////////////////////////////////
void quantify();
void weight();
void count();
void live();
void resultscreen();
void proximitysensor();
void putweight();
////////////////////////////////////////////////////////////////////////////////// FLAGS /////////////////////////////////////////////////////////////////////////////////////
bool calibrateFlag = true;
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
bool liveCountFlag = false;
bool liveCountRenderFlag = true;
bool liveCountSetupFlag = true;
bool resultCountFlag = false;
bool resultCountRenderFlag = true;
bool liveWeightFlag = false;
bool liveWeightRenderFlag = true;
bool liveWeightSetupFlag = true;
bool resultWeightFlag = false;
bool resultWeightRenderFlag = true;
bool stopLoop = false;
////////////////////////////////////////////////////////////////////////////INPUT VARIABLES //////////////////////////////////////////////////////////////////////////////////////
String w = "";
String v = "";

int counter = 0;
int packets = 0;
double data = 0;

int lineNum=0;           //LCD
int cursorpos=0;         //LCD

#define scrollSpeed 300  // LCD SCROLL SPEED

////////////////////////////////////////////////////////////////////////////////SEED STRUCTURE ///////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////////////////////VOID LOOP////////////////////////////////////////////////////////////////////////////
void setup() {
  lcd.backlight();
  lcd.begin();
  lcd.setCursor(1,1);
  lcd.print("Cownt Technologies");
  lcd.setCursor(6,2);
  lcd.print("Welcome");
  // Setup LCD with backlight and initialize

  // Setup LCD with backlight and initialize
  //pinMode(53,INPUT); //Proximity Sensor Assignmet
  pinMode(19,INPUT);
  attachInterrupt(digitalPinToInterrupt(19), proximitysensor, FALLING);
  Serial.begin(9600);
  // button pin
  pinMode(A8, INPUT_PULLUP);//Button Pin
  pinMode(buzzer,OUTPUT); //buzzer
                          // 10 rpm - STEPPER
  delay(2000);

  // Set initial speed of the motor & stop

                                      // Takes in FORWARD/BACKWARD/RELEASE(Cutsoff power = setSpeed(0))
}

void loop() {
  if (stopLoop == false){
    if(calibrateFlag){
        lcd.clear();
        EEPROM.write(0x00,0x00);
        calibrate();
        calibrateFlag = false;
    }
    else if (initFlag){
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
      //add();
    }else if(liveCountFlag){
      liveCount();
    }else if (resultCountFlag){
     resultCountScreen() ;
    }else if(liveWeightFlag){
      liveWeight();
      putweight();
    }else if (resultWeightFlag){
     resultWeightScreen() ;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////GETTING USERINPUT/////////////////////////////////////////////////////////////////////////////
char getInput(){
  char customKey = userKeypad.getKey();
  return customKey;
}

//////////////////////////////////////////////////////////////////////////////////////////CALIBRATE/////////////////////////////////////////////////////////////////////////////
void calibrate(){                                      //Load cell calibration - To be called in setup only
    // eeprom
  if (EEPROM.read(0x00) != 0x01) 
  {
    Serial.println("NOT INIT !!!!");
    currentOffset = 0;
    calibration_factor = DEFAULT_CALIFACTOR;     
    // show instructions
    lcd.setCursor(0, 0);
    lcd.print("MUST CALIBIRATE");
    lcd.setCursor(0, 1);
    lcd.print("Press Button #");
    //wait for button press
    while (!(getInput() == '#'));
    lcd.clear();
  }
  else
  {
    EEPROM.get(0x01,currentOffset);
    EEPROM.get(0x01+sizeof(long),calibration_factor);   
    Serial.println("currentOffset = " + String(currentOffset));
    Serial.println("calibration_factor = " + String(calibration_factor));
  }
  

  //scale
  scale.begin(DOUT, CLK);
  delay(100);
  Serial.println("calibration_factor = " + String(calibration_factor));
  scale.set_scale(calibration_factor / LB2KG);

  // if button is pressed (LOW) start calibiration
    // show instructions
    lcd.setCursor(0, 0);
    lcd.print("Clear Scale");
//    // wait till person leaves the button
//    while (!digitalRead(A8));
//    //short delay
//    delay(200);
    
    // set tare and save value
    scale.tare();
    currentOffset = scale.get_offset();
    Serial.println(currentOffset);

    // show on lcd
    lcd.clear();
    lcd.print("Place Test weight");
    lcd.setCursor(0, 1);
    lcd.print("Press Button #");
    
    //wait for button press
    while (!(getInput() == '#'));
    lcd.clear();
    lcd.print("Please wait ");
    Serial.println("calibirte");
    // calibiation
    boolean done = false;
    uint8_t flipDirCount = 0;
    int8_t direction = 1;
    uint8_t dirScale = 100;
    double data = abs(scale.get_units());
    double prevData = data;
    char runningSign[] = {'-','\\','|','/'};
    uint8_t runningSignIdx = 0;
    while (!done)
    {
      // get data
      data = abs(scale.get_units());
      Serial.println("data = " + String(data, 2));
      Serial.println("abs = " + String(abs(data - CALWEIGHT), 4));
      Serial.println("calibration_factor = " + String(calibration_factor));
      // if not match
      if (abs(data - CALWEIGHT) >= 0.01)
      {
        if (abs(data - CALWEIGHT) < abs(prevData - CALWEIGHT) && direction != 1 && data < CALWEIGHT)
        {
          direction = 1;
          flipDirCount++;
        }
        else if (abs(data - CALWEIGHT) >= abs(prevData - CALWEIGHT) && direction != -1 && data > CALWEIGHT)
        {
          direction = -1;
          flipDirCount++;
        }

        if (flipDirCount > 2)
        {
          if (dirScale != 1)
          {
            dirScale = dirScale / 10;
            flipDirCount = 0;
            Serial.println("dirScale = " + String(dirScale));
          }
        }
        // set new factor 
        calibration_factor += direction * dirScale;
        scale.set_scale(calibration_factor / LB2KG);
        // show still running 
        lcd.setCursor(19, 3);
        lcd.print(runningSign[runningSignIdx]);
        runningSignIdx = (runningSignIdx+1)%4;
        //short delay
        delay(5);
        // keep old data 
        prevData = data;
      }
      // if match
      else
      {
        Serial.println("NEW currentOffset = " + String(currentOffset));
        Serial.println("NEW calibration_factor = " + String(calibration_factor));
        EEPROM.put(0x00,0x00); // set init
        EEPROM.put(0x01,currentOffset);
        EEPROM.put(0x01+sizeof(long),calibration_factor);  
        done = true;
        lcd.clear();
      }

    } // end while

  scale.set_offset(currentOffset);

  lcd.setCursor(0, 0);
  lcd.print("Weight is :");
  lcd.setCursor(10, 1);
//  lcd.print(data);
  lcd.setCursor(13, 1);
  lcd.print("KG");
  delay(300);
  
  Serial.println("setup done ...");

}
////////////////////////////////////////////////////////////////////////////////////////////SCROLLING//////////////////////////////////////////////////////////////////////////////////
void scroll(char* msg){                                                                   

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

////////////////////////////////////////////////////////////////////////////INITIAL SCREEN///////////////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////// START & QUIT SCREEN ////////////////////////////////////////////////////////////////////////////////////////////
void menu(){
    if(menuRenderFlag){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("A. Start");
          //Serial.println("A is Pressed");
//        lcd.setCursor(0,1);
//        lcd.print("B. Settings");
        lcd.setCursor(0,3);
        lcd.print("B. Exit");
        menuRenderFlag=false; 
    }
    char* menuInput = getInput();
    if (menuInput == 'A'){
      seedMenuFlag = true;  
      menuFlag = false;
//    }else if (menuInput == 'B'){
//      settingFlag = true;  
//      menuFlag = false;
    }else if (menuInput == 'B'){
      lcd.clear();
      initFlag=true;  
      
    }
  
}

//////////////////////////////////////////////////////////////////////////////SEED MENU/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void seedMenu(){
    if(seedMenu1RenderFlag){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("SELECT SEED VARIETY");
        lcd.setCursor(1,2);
        lcd.print("1.");
        lcd.setCursor(3,2);
        lcd.print(seed1.name);
        lcd.setCursor(1,3);
        lcd.print("2.");
        lcd.setCursor(3,3);
        lcd.print(seed2.name);
        lcd.setCursor(13,2);
        lcd.print("3.");
        lcd.setCursor(15,2);
        lcd.print(seed3.name);
        lcd.setCursor(13,3);
        lcd.print("4.");
        lcd.setCursor(15,3);
        lcd.print(seed4.name);
        
//        lcd.setCursor(10,2);
//        lcd.print("5.");
//        lcd.setCursor(12,2);
//        lcd.print(seed5.name);
//        lcd.setCursor(10,3);
//        lcd.print("B.");
//        lcd.setCursor(12,3);
//        lcd.print("Next");
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
//      case 'B':
//        seedMenu2RenderFlag = true;
//        seedMenu1RenderFlag = false;
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
        initFlag = true;
//        seedMenu1RenderFlag = false;
//        seedMenuFlag = false;
//        addRenderFlag = true;
//        addFlag = true;
        lcd.clear();
    }

}

/////////////////////////////////////////////////////////////////////////////////QUANTIFYING SEED - INITIATES THINGS ////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////////WEIGHT INPUT SCREEN//////////////////////////////////////////////////////////////////////////////
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
      liveWeightFlag = true;
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
/////////////////////////////////////////////////////////////////////////////////////////////COUNT INPUT SCREEN////////////////////////////////////////////////////////////////////////////////////////////
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
      liveCountFlag = true;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////LIVE FEED SCREEN//////////////////////////////////////////////////////////////////////////////////////////
void liveCount(){

  if(liveCountSetupFlag){
    open(int(selectedSeed.dimension*116));
    vibrate();
    liveCountSetupFlag = false;  
  }
  if(counter % w.toInt() == 0 && counter != 0){
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    counter = 0;
    packets = packets + 1;
   lcd.clear();

  }
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
    liveCountFlag = false;  
    liveCountSetupFlag = true;
    resultCountFlag = true;
    close(int(selectedSeed.dimension*116)+100);
    stop_vibrate();
  }else if (menuInput == 'B'){
    seedMenuFlag = true;  
    seedMenu1RenderFlag = true;
    liveCountSetupFlag = true;
    liveCountFlag = false;
    counter = 0;
    packets = 0;
    w = "";
    close(int(selectedSeed.dimension*116)+100);
    stop_vibrate();
  }
}

void liveWeight(){
  Serial.println(data);

  if(liveWeightSetupFlag){
    open(int(selectedSeed.dimension*116));
    vibrate();
    liveWeightSetupFlag = false;  
  }
  if(int(data) % w.toInt() >= 10 && int(data) >= 10){
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    data = 0;
    packets = packets + 1;
   lcd.clear();
  delay(5000);
  
  }
  lcd.setCursor(0,0);
  lcd.print("LIVE FEED ");
  lcd.setCursor(0,2);
  lcd.print("Weight:");
  lcd.setCursor(0,3);
  lcd.print("Packets:");
  lcd.setCursor(13,2);
  lcd.print("A. Done");
  lcd.setCursor(13,3);
  lcd.print("B. Back");

  lcd.setCursor(7,2);
  lcd.print(data);
  lcd.setCursor(9,3);
  lcd.print(packets);

  char* menuInput = getInput();
  if (menuInput == 'A'){
    liveWeightFlag = false;  
    liveWeightSetupFlag = true;
    resultWeightFlag = true;
    close(int(selectedSeed.dimension*116)+100);
    stop_vibrate();
  }else if (menuInput == 'B'){
    seedMenuFlag = true;  
    seedMenu1RenderFlag = true;
    liveWeightSetupFlag = true;
    liveWeightFlag = false;
    counter = 0;
    packets = 0;
    w = "";
    close(int(selectedSeed.dimension*116)+100);
    stop_vibrate();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////PROXIMITY SENSOR//////////////////////////////////////////////////////////////////////////
void proximitysensor(){

    counter = counter + 1;

}
////////////////////////////////////////////////////////////////////////////////////////////loadcell///////////////////////////////////////////////////////////////////////////////////////////

void putweight(){                                  //Puts / Displays load on LCD

  data = abs(scale.get_units());

  if (0.0000 - data > 0.0001){ 
     data = 0.00; //reset to zero
     delay(500);
  }
   
}
////////////////////////////////////////////////////////////////////////////////////////////////////////RESULT SCREEN////////////////////////////////////////////////////////////////////////

void resultCountScreen(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RESULT");
    lcd.setCursor(0,2);
    lcd.print("Total Packets: ");
    lcd.setCursor(0,3);
    lcd.print("Total Count:");
    lcd.setCursor(14,2);
    lcd.print(packets);
    lcd.setCursor(14,3);
    lcd.print(packets*w.toInt() + counter);
    delay(10000);
    menuRenderFlag = true;
    menuFlag = true;
    counter = 0;
    packets = 0;
    w = "";
}

void resultWeightScreen(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RESULT");
    lcd.setCursor(0,2);
    lcd.print("Total Packets: ");
    lcd.setCursor(0,3);
    lcd.print("Total Weight:");
    lcd.setCursor(14,2);
    lcd.print(packets);
    lcd.setCursor(14,3);
    lcd.print(packets*w.toInt() + counter);
    delay(10000);
    menuRenderFlag = true;
    menuFlag = true;
    counter = 0;
    packets = 0;
    w = "";
}
  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void open (int dimension){  // Opens Flaps   // Need to pass the dimesion parameter
  AF_Stepper flapcontroller (stepsPerRevolution,2);
  AF_DCMotor motor(2);
  flapcontroller.setSpeed(rpm);   
  Serial.println(dimension);
  flapcontroller.step(dimension, FORWARD ,DOUBLE);  
  } 

void close (int dimension){    
  AF_Stepper flapcontroller (stepsPerRevolution,2);
  AF_DCMotor motor(2);// Closes Flaps
  flapcontroller.setSpeed(rpm);   
  flapcontroller.step(dimension, BACKWARD ,DOUBLE);       
  }

void vibrate(){             // Starts Vibration 
  AF_Stepper flapcontroller (stepsPerRevolution,1);
  AF_DCMotor motor(1);
  motor.setSpeed(255);                              // Takes in Speed, Speed ranges from 0(Off) to 255(Full Throttle)
  motor.run(RELEASE);
  motor_state = 1;
  motor.setSpeed(255);
  motor.run(FORWARD);
  //Serial.println(motor_state);
}

void stop_vibrate(){                                    // Stops Vibration 
  AF_Stepper flapcontroller (stepsPerRevolution,1);
  AF_DCMotor motor(1);
  motor.setSpeed(intensity);                              // Takes in Speed, Speed ranges from 0(Off) to 255(Full Throttle)
  motor.run(RELEASE);
  motor_state = 0;
  //Serial.println(motor_state);
}
