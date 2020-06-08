#include <Wire.h>
//pin 3 and 5(RPi) go to a4 and a5(arduino) , GND to GND (20 Rpi)
const int led1 = 13;
void setup() 
{
  // put your setup code here, to run once:
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  pinMode(led1, OUTPUT);
  digitalWrite(led1, LOW);
}
void receiveEvent(int number){
  while (Wire.available()){
    char d = Wire.read();
    if (d == 1)
    {
        digitalWrite(led1, HIGH); 
    }
    else if (d == 0)
    {
        digitalWrite(led1, LOW); 
    }
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
}
