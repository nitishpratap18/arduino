#include <ArduinoBLE.h>
#include <SPI.h>
#include <WiFiNINA.h>
#define BLE_UUID_TEST_SERVICE               "9A48ECBA-2E92-082F-C079-9E75AAE428B1"
#define BLE_UUID_FILE_NAME                  "2D2F88C4-F244-5A80-21F1-EE0224E80658"
BLEService testService( BLE_UUID_TEST_SERVICE );
BLEStringCharacteristic wifiPasswordCharacterstic( BLE_UUID_FILE_NAME, BLERead | BLEWrite, 20 );
int status = WL_IDLE_STATUS;
WiFiServer server(80);
String wifiNamePassword = "";
void setup()
{
  Serial.begin( 9600 );
  while ( !Serial );
  startBle();
  
}
void loop() {
  wifiConnection();
  

}
void printWifiStatus() {
  // print the wifiName of the network you're attached to:
  Serial.print("wifiName: ");
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
void startBle(){
  BLE.begin();
  
  // set advertised local name and service UUID:
  BLE.setDeviceName( "Arduino Nano rp 2040" );
  BLE.setLocalName( "Arduino Nano rp 2040" );
  BLE.setAdvertisedService( testService );
  // BLE add characteristics
  testService.addCharacteristic( wifiPasswordCharacterstic );
  // add service
  BLE.addService( testService );
  connectToBle();
  connectToWifi(wifiNamePassword);
}

void connectToBle() {
  
  // set the initial value for the characeristic:
  wifiPasswordCharacterstic.writeValue( wifiNamePassword );
  // start advertising
  BLE.advertise();
  int isBlueToothConnected = 0;
  while (isBlueToothConnected != 1) {
//    Serial.println("Please connect to bluetooth");
    BLEDevice central = BLE.central();
    if ( central )
    {
      Serial.print( "Connected to central: " );
      Serial.println( central.address() );
      while ( central.connected() )
      {
        if ( wifiPasswordCharacterstic.written() )
        {
          wifiNamePassword = wifiPasswordCharacterstic.value();
          Serial.println( wifiNamePassword );
          isBlueToothConnected = 1; 
          connectToWifi(wifiNamePassword);
        }
      }
    }
    else{
      Serial.println("Disconnected");
    }
  }
}

void connectToWifi(String wifiNamePassword ) {

  String wifiName = "";
  String password = "";
  int indexOfPartition = wifiNamePassword.indexOf(" ");
  wifiName = wifiNamePassword.substring(0, indexOfPartition);
  password = wifiNamePassword.substring(indexOfPartition + 1);
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
    Serial.println(wifi);                   // print the network name (wifiName);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(wifi, pass);
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

      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
