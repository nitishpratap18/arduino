#include <Arduino_LSM6DSOX.h>


void setup() {
  Serial.begin(9600);

  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println("Hz");
  Serial.println();

}

void loop() {

  float Gx, Gy, Gz;
  float Ax, Ay, Az;


  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(Gx, Gy, Gz);
    IMU.readAcceleration(Ax, Ay, Az);
    Serial.println("Gyroscope data: ");
    Serial.print(Gx);
    Serial.print('\t');
    Serial.print(Gy);
    Serial.print('\t');
    Serial.println(Gz);
    Serial.println();
    Serial.println("Acceleration data: ");
    Serial.print(Ax);
    Serial.print("\t");
    Serial.print(Ay);
    Serial.print("\t");
    Serial.println(Az);
  }

  delay(1000);

}
