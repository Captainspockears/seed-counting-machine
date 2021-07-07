 #include "HX711.h"
HX711 cell(3, 52);
void setup() {
  Serial.begin(9600);
}
long val=0;
float count=0;

void loop() {
  count = count + 1;
  //val = ((count-1)/count) * val    +  (1/count) * cell.read(); // take long term average
   val = 0.5 * val    +   0.5 * cell.read(); // take recent average
  //val = cell.read(); // most recent reading
   Serial.println((val - 8418464) / 10539.0f * 102

);

   
}
