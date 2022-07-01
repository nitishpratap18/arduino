#include<Arduino_LSM6DSOX.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  if(!IMU.begin()){
    Serial.println("Failed to initialize sensor");
    while(1);
  }
  Serial.println("Temperature is :");
  

}

void loop() {
  // put your main code here, to run repeatedly:
  if (IMU.temperatureAvailable())
  {
    int temperature_deg = 0;
    IMU.readTemperature(temperature_deg);

    Serial.print("LSM6DSOX Temperature = ");
    Serial.print(temperature_deg);
    Serial.println(" °C");
  }
  delay(100000);

}
