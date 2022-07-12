/*
   This is code for write to arduino through ble as well as wifi.
   
*/
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <WiFi.h>
 WiFiServer server(80);
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
 char ssid[] = "iTinker";
  char password[] = "Pass1234a";
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      
      std::string value = pCharacteristic->getValue();
      
      if (value.length() > 0) {
       
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++){
          Serial.print(value[i]);
         
        }
        Serial.println();
       
      }
    }
};

void setup() {
  Serial.begin(115200);
  startBle();
  startWifi();
  server.begin();
}

void loop() {

  // put your main code here, to run repeatedly:
   WiFiClient client = server.available();

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        String s = client.readStringUntil('\r');             // read a byte, then
        Serial.println(s);                    // print it out the serial monitor
      }
    }
  }
  // close the connection:
  client.stop();
  //  Serial.println("Client Disconnected.");
}
void startBle(){
   
  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}
void startWifi(){
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("wifi is connected to : ");
  Serial.println(WiFi.localIP());
}
