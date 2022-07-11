/*
 * This code connect esp32 with bluetooth and phone and then ask for wifi credentials 
 * after that it connects with wifi and if we send wrong credentials then at wrong command this
 * code again run bluetooth and again ask for its wifi password
 */


#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
String s = "";
bool isWifiConnected = false;
#include <WiFi.h>
WiFiServer server(80);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  startBt();
  scanForBt();
  Serial.println("Connected to bluetooth");
  getWifiCredentials();
  connectToWifi();
  startServer();

}

void loop() {
  // put your main code here, to run repeatedly:
  wificlient();

}
void startBt() {
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}
void scanForBt() {
  while (!SerialBT.connected()) {
    if (!SerialBT.connected()) {
      Serial.println("Please connect to bluetooth");
    }
  }
}
void closeBt() {
  SerialBT.end();
}
void getWifiCredentials() {
  s = "";

  while (SerialBT.connected()) {
    while (s.length() <=10) {
      s = SerialBT.readStringUntil('\r');
    }
    Serial.print(s);
    break;
  }

}
void connectToWifi() {

  int indexOfPartition = s.indexOf(" ");
  String uid = s.substring(0, indexOfPartition);
  String pass = s.substring(indexOfPartition + 1);
  char ssid[uid.length()];
  char password[pass.length()];
  for (int i = 0; i < uid.length(); i++) {
    ssid[i] = uid[i];
  }
  for (int i = 0; i < pass.length(); i++) {
    password[i] = pass[i];
  }
  password[pass.length()] = '\0';
  ssid[uid.length()] = '\0';
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print(" ");
  Serial.println(pass);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("wifi is connected to : ");
  Serial.println(WiFi.localIP());
}
void startServer() {
  server.begin();
}

void wificlient() {
  WiFiClient client = server.available();

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        String s = client.readStringUntil('\r');             // read a byte, then
        Serial.println(s);                    // print it out the serial monitor
        if (s == "wrong") {
          s = "";
          getWifiCredentials();
          connectToWifi();
          startServer();
        }
        else if (s == "close") {
          closeBt();
          WiFi.disconnect();
        }
        else {
          Serial.println("Nothing to do now");
        }
      }
    }
  }
  // close the connection:
  client.stop();
  //  Serial.println("Client Disconnected.");
}
