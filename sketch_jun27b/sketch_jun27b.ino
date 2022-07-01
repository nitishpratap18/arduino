#include<WiFiNINA.h>
void setup() {
  // put your setup code here, to run once:
 pinMode(LEDR,OUTPUT);
 pinMode(LEDG,OUTPUT);
 pinMode(LEDB,OUTPUT);
 

}

void loop() {
//  // put your main code here, to run repeatedly:
//  digitalWrite(LEDR,HIGH);
//  delay(500);
//  digitalWrite(LEDR,LOW);
//  delay(500);
analogWrite(LEDR,255);
analogWrite(LEDG,255);
digitalWrite(LEDB,LOW);

}  
