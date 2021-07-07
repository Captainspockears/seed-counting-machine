//#define LED D0            // Led in NodeMCU at pin GPIO16 (D0).
int count=0;
int usernum;
void setup() {
//pinMode(LED, OUTPUT);    // LED pin as output.
pinMode(53,INPUT_PULLUP);
Serial.begin(9600);  

  //Serial.print("Enter the Count:");  
//while (Serial.available() == 0) {} //Wait for user input  
//usernum = Serial.read(); //Reading the Input string from Serial port.  

}
void loop() {
  

  if(digitalRead (53) == LOW )
  {
count=count+1;
delay (400); // TO BE SET WITH RESPECT TO VIBRATION RATE


  }
  Serial.println(count);
}
