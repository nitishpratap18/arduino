/*
 * 
 * This code connects arduino blueTooyth with nrf bluetooth and on arduino wifi
  
*/

#include <ArduinoBLE.h>
#include <SPI.h>
#include <WiFiNINA.h>
char wifiName[] = "iTinker";
char wifiPass[] = "Pass1234a";
int status = WL_IDLE_STATUS;
//----------------------------------------------------------------------------------------------------------------------
// BLE UUIDs
//----------------------------------------------------------------------------------------------------------------------

#define BLE_UUID_TEST_SERVICE               "9A48ECBA-2E92-082F-C079-9E75AAE428B1"
#define BLE_UUID_FILE_NAME                  "2D2F88C4-F244-5A80-21F1-EE0224E80658"

//----------------------------------------------------------------------------------------------------------------------
// BLE
//----------------------------------------------------------------------------------------------------------------------

BLEService testService( BLE_UUID_TEST_SERVICE );
BLEStringCharacteristic fileNameCharacteristic( BLE_UUID_FILE_NAME, BLERead | BLEWrite, 20 );

String fileName = "";


void setup()
{
  Serial.begin( 9600 );
  while ( !Serial );

  BLE.begin();

  // set advertised local name and service UUID:
  BLE.setDeviceName( "Arduino Nano 33 BLE" );
  BLE.setLocalName( "Arduino Nano 33 BLE" );
  BLE.setAdvertisedService( testService );

  // BLE add characteristics
  testService.addCharacteristic( fileNameCharacteristic );

  // add service
  BLE.addService( testService );

  // set the initial value for the characeristic:
  fileNameCharacteristic.writeValue( fileName );

  // start advertising
  BLE.advertise();

}

void loop()
{
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  if ( central )
  {
    Serial.print( "Connected to central: " );
    Serial.println( central.address() );

    while ( central.connected() )
    {
      if ( fileNameCharacteristic.written() )
      {
        fileName = fileNameCharacteristic.value();
        Serial.print( "New file name: " );
        Serial.println( fileName );
        if (WiFi.status() == WL_NO_MODULE) {
          Serial.println("Communication with WiFi module failed!");
          // don't continue
          while (true);
        }
        String fv = WiFi.firmwareVersion();
        if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
          Serial.println("Please upgrade the firmware");
        }
        while (status != WL_CONNECTED) {

          Serial.print("Attempting to connect to Network named: ");
                       // print the network name (wifiName);
          // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
          status = WiFi.begin(wifiName, wifiPass);
          // wait 10 seconds for connection:
          delay(10000);

        }
                               // start the web server on port 80
        printWifiStatus();

      }

    }

    Serial.print( F( "Disconnected from central: " ) );
    Serial.println( central.address() );
  }
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
