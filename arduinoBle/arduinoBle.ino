
#include <ArduinoBLE.h>
#include <SPI.h>
#include <WiFiNINA.h>
#define BLE_UUID_TEST_SERVICE               "9A48ECBA-2E92-082F-C079-9E75AAE428B1"
#define BLE_UUID_FILE_NAME                  "2D2F88C4-F244-5A80-21F1-EE0224E80658"
BLEService testService( BLE_UUID_TEST_SERVICE );
BLEStringCharacteristic wifiPassworiCharacterstic( BLE_UUID_FILE_NAME, BLERead | BLEWrite, 20 );

String ssid = "";
String password = "";
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup()
{
  Serial.begin( 9600 );
  while ( !Serial );
  String ssidPassword = connectToBle();
  connectToWifi(ssidPassword);
}

void loop() {
  wifiConnection();

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

String connectToBle() {
  BLE.begin();
  String ssidPassword = "";
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
          return ssidPassword;
        }
      }
    }

  }
}

void connectToWifi(String ssidPassword ) {

  int indexOfPartition = ssidPassword.indexOf(" ");
  ssid = ssidPassword.substring(0, indexOfPartition);
  password = ssidPassword.substring(indexOfPartition + 1);
  Serial.println(password);
  char sid[ssid.length() + 1];
  char pass[password.length() + 1];
  for (int i = 0; i < ssid.length(); i++) {
    sid[i] = ssid.charAt(i);
  }
  sid[ssid.length()] = '\0';
  pass[password.length()] = '\0';
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
    status = WiFi.begin(sid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}
void wifiConnection() {

  WiFiClient client = server.available();
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        String s = client.readStringUntil('\r');             // read a byte, then
        Serial.println(s);                    // print it out the serial monitor
        if (s == "RED") {
          digitalWrite(LEDR, HIGH);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
