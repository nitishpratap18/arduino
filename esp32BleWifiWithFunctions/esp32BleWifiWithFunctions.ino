/*
   This is code for write to arduino through ble as well as wifi.

*/
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <WiFi.h>
#include <BLE2902.h>

WiFiServer server(80);
int status = WL_IDLE_STATUS;
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"



String wifiCredentials = "";
int indexOfPartition;
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {

      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {

        Serial.print("New value: ");
        wifiCredentials = "";
        for (int i = 0; i < value.length(); i++) {
          wifiCredentials  = wifiCredentials+value[i];
        }
        Serial.print("hello : ");
        Serial.println(wifiCredentials);
        
      }
    }
};

void setup() {
  Serial.begin(115200);
  startBle();
  while(wifiCredentials.length()<11){
    Serial.println(wifiCredentials.length());
    //just wait do nothing
    if(wifiCredentials.length()>=11){
      Serial.println(wifiCredentials.length());
      break;
    }
  }

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
//    Serial.println("Client Disconnected.");

}
void startBle() {

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
  Serial.println("Bluetooth is started connect now");
}
void startWifi() {
  Serial.print("I am in startwifi :");
  Serial.println(wifiCredentials);
   String wifiName = "";
  String password = "";
  int indexOfPartition = wifiCredentials.indexOf(" ");
  wifiName = wifiCredentials.substring(0, indexOfPartition);
  password = wifiCredentials.substring(indexOfPartition + 1);
  Serial.println(password);
  char wifi[wifiName.length() + 1];
  char pass[password.length() + 1];
  for (int i = 0; i < wifiName.length(); i++) {
    wifi[i] = wifiName.charAt(i);
  }
  wifi[wifiName.length()] = '\0';
  pass[password.length()] = '\0';
  for (int i = 0; i < password.length(); i++) {
    pass[i] = password.charAt(i);
  }
  Serial.print("wifiName is :");
  Serial.println(wifi);
  Serial.print("Password is :");
  Serial.println(pass);
  
  
  // attempt to connect to WiFi network:
  

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(wifi);
    Serial.print(pass);
    
    Serial.println("Trying to connect to wifi");
    WiFi.begin(wifi, pass);
    delay(500);
    Serial.print(".");
  }
  Serial.print("wifi is connected to : ");
  Serial.println(WiFi.localIP());
}
