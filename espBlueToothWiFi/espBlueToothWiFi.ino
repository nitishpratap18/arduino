/*
  WiFi Web Server LED Blink

  A simple web server that lets you blink an LED via the web.
  This sketch will print the IP address of your WiFi Shield (once connected)
  to the Serial monitor. From there, you can open that address in a web browser
  to turn on and off the LED on pin 5.

  If the IP address of your shield is yourAddress:
  http://yourAddress/H turns the LED on
  http://yourAddress/L turns it off

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the Wifi.begin() call accordingly.

  Circuit:
   WiFi shield attached
   LED attached to pin 5

  created for arduino 25 Nov 2012
  by Tom Igoe

  ported for sparkfun esp32
  31.01.2017 by Jan Hendrik Berlin

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
void setup()
{
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode(5, OUTPUT);      // set the LED pin mode
  while (!SerialBT.connected()) {
    if (!SerialBT.connected()) {
      Serial.println("Please connect to bluetooth");
    }
  }
  Serial.println("Connected to bluetooth");
  // We start by connecting to a WiFi network
  Serial.print("Connecting to wifi ");
  while (SerialBT.connected()) {
    while (s.length() == 0) {
      s = SerialBT.readStringUntil('\r');
    }
    int indexOfPartition = s.indexOf(" ");
    String uid = s.substring(0, indexOfPartition);
    String pass = s.substring(indexOfPartition + 1);
    Serial.println("I am here");

    char ssid[uid.length()];
    char password[pass.length()];
    for (int i = 0; i < uid.length(); i++) {
      ssid[i] = uid[i];
    }
    for (int i = 0; i < pass.length(); i++) {
      password[i] = pass[i];
    }
    Serial.println(pass);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.print("wifi is connected to : ");
    Serial.println(WiFi.localIP());
    break;
  }
  server.begin();
}

void loop() {
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
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }

}
