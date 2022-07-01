#include <SPI.h>
#include <WiFiNINA.h>
#include<Arduino_LSM6DSOX.h>
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "iTinker";        // your network SSID (name)
char pass[] = "Pass1234a";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);      // initialize serial communication
  while (!Serial);
  if (!IMU.begin()) {
    Serial.println("Failed to initialize sensor");
    while (1);
  }
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port

    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        if (IMU.temperatureAvailable())
        {
          int temperature_deg = 0;
          IMU.readTemperature(temperature_deg);
          Serial.print(temperature_deg);
          client.write(temperature_deg);
        }
        //char c = client.read();             // read a byte, then
        String line = client.readStringUntil('\r');
        //        client.write("hello");
        //Serial.print(line);

        //        Serial.println(line);
        //Serial.println(line.toInt());
        if (line == "On Red") {
          digitalWrite(LEDR, HIGH);
          client.write(56);
        }
        if (line == "Off Red") {
          digitalWrite(LEDR, LOW);
        }
        if (line == "On Green") {
          digitalWrite(LEDG, HIGH);
        }
        if (line == "Off Green") {
          digitalWrite(LEDG, LOW);
        }
        if (line == "On Blue") {
          digitalWrite(LEDB, HIGH);
        }
        if (line == "Off Blue") {
          digitalWrite(LEDB, LOW);
        }
        if(line=="On Blink"){
        Serial.print("Blinking");
          blinkLed();
        }
        if(line == "Off Blink"){
          digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        }


        //         Check to see if the client request was /X
        //        if (currentLine.endsWith("GET /RH")) {
        //          digitalWrite(LEDR, HIGH);
        //        }
        //        if (currentLine.endsWith("GET /RL")) {
        //          digitalWrite(LEDR, LOW);
        //        }
        //        if (currentLine.endsWith("GET /GH")) {
        //          digitalWrite(LEDG, HIGH);
        //        }
        //        if (currentLine.endsWith("GET /GL")) {
        //          digitalWrite(LEDG, LOW);
        //        }
        //        if (currentLine.endsWith("GET /BH")) {
        //          digitalWrite(LEDB, HIGH);
        //        }
        //        if (currentLine.endsWith("GET /BL")) {
        //          digitalWrite(LEDB, LOW);
        //
        //
        //        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
void blinkLed(){
  while(1){
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
  }      
}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
