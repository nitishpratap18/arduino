/*
  This example creates a BLE peripheral with a service that contains String
  characteristics that can be read and written.
  The length of the String characteristic needs to be fixed.

  The circuit:
  - Arduino Nano 33 BLE Sense board.

  You can use a generic BLE central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
#include <SPI.h>
#include <WiFiNINA.h>
//char p[] = "Pass1234a";
//char ss[] = "iTinker"; 
//----------------------------------------------------------------------------------------------------------------------
// BLE UUIDs
//----------------------------------------------------------------------------------------------------------------------

#define BLE_UUID_TEST_SERVICE               "9A48ECBA-2E92-082F-C079-9E75AAE428B1"
#define BLE_UUID_FILE_NAME                  "2D2F88C4-F244-5A80-21F1-EE0224E80658"

//----------------------------------------------------------------------------------------------------------------------
// BLE
//----------------------------------------------------------------------------------------------------------------------

BLEService testService( BLE_UUID_TEST_SERVICE );
BLEStringCharacteristic wifiPassworiCharacterstic( BLE_UUID_FILE_NAME, BLERead | BLEWrite, 20 );
String ssidPassword = "";
String ssid = "";
String password = "";
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup()
{
  Serial.begin( 9600 );
  while ( !Serial );

  BLE.begin();

  // set advertised local name and service UUID:
  BLE.setDeviceName( "Arduino Nano rp 2040" );
  BLE.setLocalName( "Arduino Nano rp 2040" );
  BLE.setAdvertisedService( testService );

  // BLE add characteristics
  testService.addCharacteristic( wifiPassworiCharacterstic );

  // add service
  BLE.addService( testService );

  // set the initial value for the characeristic:
  wifiPassworiCharacterstic.writeValue( ssidPassword );

  // start advertising
  BLE.advertise();

  int isBlueToothConnected = 0;
  while (isBlueToothConnected != 1) {
    Serial.println("Please connect to bluetooth");
    BLEDevice central = BLE.central();
    if ( central )
    {
      Serial.print( "Connected to central: " );
      Serial.println( central.address() );

      while ( central.connected() )
      {
        if ( wifiPassworiCharacterstic.written() )
        {
          ssidPassword = wifiPassworiCharacterstic.value();
          Serial.println( ssidPassword );
          isBlueToothConnected = 1;
          break;
        }
      }
    }

  }
  int indexOfPartition = ssidPassword.indexOf(" ");
  ssid = ssidPassword.substring(0, indexOfPartition);
  password = ssidPassword.substring(indexOfPartition + 1);
  Serial.println(password);
  char sid[ssid.length()+1];
  char pass[password.length()+1];
  for (int i = 0; i < ssid.length(); i++) {
    sid[i] = ssid.charAt(i);
  }
  sid[ssid.length()] = '\0';
  for (int i = 0; i < password.length(); i++) {
    pass[i] = password.charAt(i);
  }
  Serial.println("Ssid is :");
  Serial.println(sizeof(sid));
  Serial.println("Password is :");
  Serial.println(pass);
  Serial.println();
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
    Serial.println(sid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(sid,pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status





}

void loop()
{
  // listen for BLE peripherals to connect:
  WiFiClient client = server.available();   // listen for incoming clients
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            //            client.print("<style>");
            //            client.print(".container {margin: 0 auto; text-align: center; margin-top: 100px;}");
            //            client.print("button {color: white; width: 100px; height: 100px;");
            //            client.print("border-radius: 50%; margin: 20px; border: none; font-size: 20px; outline: none; transition: all 0.2s;}");
            //            client.print(".red{background-color: rgb(196, 39, 39);}");
            //            client.print(".green{background-color: rgb(39, 121, 39);}");
            //            client.print(".blue {background-color: rgb(5, 87, 180);}");
            //            client.print(".off{background-color: grey;}");
            //            client.print("button:hover{cursor: pointer; opacity: 0.7;}");
            //            client.print("</style>");
            client.print("<div class='container'>");
            //            client.print("<button class='red' type='submit' onmousedown='location.href=\"/RH\"'>ON</button>");
            //            client.print("<button class='off' type='submit' onmousedown='location.href=\"/RL\"'>OFF</button><br>");
            //            client.print("<button class='green' type='submit' onmousedown='location.href=\"/GH\"'>ON</button>");
            //            client.print("<button class='off' type='submit' onmousedown='location.href=\"/GL\"'>OFF</button><br>");
            //            client.print("<button class='blue' type='submit' onmousedown='location.href=\"/BH\"'>ON</button>");
            //            client.print("<button class='off' type='submit' onmousedown='location.href=\"/BL\"'>OFF</button>");
            client.print("<input type='color' id='myColor' name='myColor' value='#e66465' onchange='myFunction()'> ");

            client.print("</div>");
            client.print("<script> function myFunction() {var x = document.getElementById('myColor').value; location.href=x; hash = document.location.hash;hash = hash.substr(1);red = parseInt(hash.substr(0,2),16);green = parseInt(hash.substr(2,2),16);blue = parseInt(hash.substr(4,2),16); console.log(hash);} </script>");
            // The HTTP response ends with another blank line:x
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        Serial.println(currentLine);
        // Check to see if the client request was /X
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
        //        }
       
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
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
