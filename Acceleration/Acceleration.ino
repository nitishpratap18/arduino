 #include<Arduino_LSM6DSOX.h>
#include<math.h>
float Ax, Ay, Az;
float velocity;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  if (!IMU.begin()) {
    Serial.println("Failed to initialize sensor");
    while (1);
  }
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
  Serial.println();

}

void loop() {
  // put your main code here, to run repeatedly:

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(Ax, Ay, Az);
    velocity = sqrt(pow(Ax, 2) + pow(Ay, 2) + pow(Az, 2)) * 9.8;
    Serial.print("velocity is ");
    Serial.println(velocity);
    Serial.println("Accelerometer data: ");
    Serial.print(Ax);
    Serial.print('\t');
    Serial.print(Ay);
    Serial.print('\t');
    Serial.println(Az);
    Serial.println();

  }
  delay(500);

}
