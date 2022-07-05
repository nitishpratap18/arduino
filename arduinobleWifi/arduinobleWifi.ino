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
  BLE.addService( testService );

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
      if ( wifiPassworiCharacterstic.written() )
      {
        ssidPassword = wifiPassworiCharacterstic.value();
        int indexOfPartition = ssidPassword.indexOf(" ");
        ssid = ssidPassword.substring(0, indexOfPartition);
        password = ssidPassword.substring(indexOfPartition + 1);


        //        Serial.println( ssid);
        Serial.println(password);
        char sid[ssid.length() + 1];
        char pass[password.length() + 1];
        for (int i = 0; i < ssid.length(); i++) {
          sid[i] = ssid.charAt(i);
        }
        for (int i = 0; i < password.length(); i++) {
          pass[i] = password.charAt(i);
        }
        
        break;
      }
    }

    // start the web server on port 80
  




    //    Serial.print( F( "Disconnected from central: " ) );
    //    Serial.println( central.address() );
  }
}
