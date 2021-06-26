// Modified DC motor control to provide vibration - Using L293D Motor shield

#include<AFMotor.h>                                     // Adafruit Motor shield library

int intensity = 200;                                    // Motor speed / Vibration intensity
int duration;
int motor_state;                                        // Indicates state of motor, 0:OFF ; 1:ON

void vibrate(int intensity ,int duration = 1000);       // Function prototype  (Duration to be removed later)
void stop_vibrate();                                    // Function prototype

AF_DCMotor motor(2);                                    // creating a motor object - Takes in Motor Port Number off shield, M1=1; M2=2; M3=3 & M4=4

void setup() {                                          // Set initial speed of the motor & stop
  
  motor.setSpeed(intensity);                            // Takes in Speed, Speed ranges from 0(Off) to 255(Full Throttle)
  motor.run(RELEASE);                                   // Takes in FORWARD/BACKWARD/RELEASE(Cutsoff power = setSpeed(0))
  Serial.begin(9600);
  vibrate(255);
  stop_vibrate ();
}

void loop() {                                           // Loop
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
