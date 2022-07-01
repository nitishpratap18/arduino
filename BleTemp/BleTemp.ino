#include <ArduinoBLE.h>
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
while (!Serial);

pinMode(LED_BUILTIN, OUTPUT);
if (!BLE.begin()) 
{
Serial.println("starting BLE failed!");
while (1);
}

BLE.setLocalName("Arduino BlueTooth");
BLE.advertise();
Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEDevice central = BLE.central();

if (central) 
{
Serial.print("Connected to central: ");
Serial.println(central.address());
digitalWrite(LED_BUILTIN, HIGH);

while (central.connected()) {

      Serial.println("I am connected");
}
}
digitalWrite(LED_BUILTIN, LOW);
Serial.print("Disconnected from central: ");
Serial.println(central.address());
}
