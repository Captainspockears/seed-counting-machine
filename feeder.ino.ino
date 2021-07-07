#include <AFMotor.h>

////////////////////////////////////////////////////////////////////SEED DIMENSIONS////////////////////////////////////////////////////////////////////////////////

float beandimension = 1860                                 // 1.6 cm
float corndimension = 1520                                 // 1.3 cm
float okradimesion  = 2048                                 // By weight - Full Open
float dilldimesion  = 2048                                 // By weight - Ful Open

///////////////////////////////////////////////////////////////////FLAP CONTROL/////////////////////////////////////////////////////////////////////////////////////

#define rpm 10                                           // stepper speed - flap control

const int stepsPerRevolution = 2048;
int ip;
void open();
void close ();

AF_Stepper flapcontroller (stepsPerRevolution,2);        // connect motor to port #2 (M3 and M4)


/////////////////////////////////////////////////////////////////////VIBRATION///////////////////////////////////////////////////////////////////////////////////////

int intensity = 200;                                     // Motor speed / Vibration intensity
int duration;
int motor_state;                                        // Indicates state of motor, 0:OFF ; 1:ON

void vibrate(int intensity ,int duration = 1000);       // Function prototype  (Duration to be removed later)
void stop_vibrate();                                    // Function prototype

AF_DCMotor motor(2);                                    // creating a motor object - Takes in Motor Port Number off shield, M1=1; M2=2; M3=3 & M4=4

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  flapcontroller.setSpeed(rpm);                           // 10 rpm - STEPPER
  Serial.begin(9600);

  // Set initial speed of the motor & stop

  motor.setSpeed(intensity);                              // Takes in Speed, Speed ranges from 0(Off) to 255(Full Throttle)
  motor.run(RELEASE);                                    // Takes in FORWARD/BACKWARD/RELEASE(Cutsoff power = setSpeed(0))
  Serial.begin(9600);
  vibrate(255);
  //stop_vibrate ();

}

void loop() {
    while(Serial.available()){
         ip = Serial.read();
         if (ip == '1'){
             close ();
         }
         else if (ip == '0'){
             open ();
         }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void open (int dimension){                                          // Opens Flaps   // Need to pass the dimesion parameter
  flapcontroller.step(dimension, FORWARD ,DOUBLE);  
  } 

void close (int dimesion){                                          // Closes Flaps
  flapcontroller.step(3000, BACKWARD ,DOUBLE);       
  }

void vibrate(int intensity , int duration){             // Starts Vibration 
  motor_state = 1;
  motor.setSpeed(intensity);
  motor.run(FORWARD);
  delay(duration);
  //Serial.println(motor_state);
}

void stop_vibrate(){                                    // Stops Vibration 
  motor.run(RELEASE);
  motor_state = 0;
  //Serial.println(motor_state);
}
