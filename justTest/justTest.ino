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
#define BLE_UUID_Password                  "2D2F88C4-F244-5A80-21F1-EE0224E80658"

//----------------------------------------------------------------------------------------------------------------------
// BLE
//----------------------------------------------------------------------------------------------------------------------

BLEService testService( BLE_UUID_TEST_SERVICE );
BLEStringCharacteristic wifiIdPassword( BLE_UUID_Password, BLERead | BLEWrite, 20 );
String ssidPassword = "";
void setup()
{
  Serial.begin( 9600 );
  while ( !Serial );

  BLE.begin();

  // set advertised local name and service UUID:
  BLE.setDeviceName( "Arduino nano rp 2040" );
  BLE.setLocalName( "Arduino nano rp 2040" );
  BLE.setAdvertisedService( testService );

  // BLE add characteristics
  testService.addCharacteristic( wifiIdPassword );

  // add service
  BLE.addService( testService );

  // set the initial value for the characeristic:
  wifiIdPassword.writeValue( ssidPassword );
  // start advertising
  BLE.advertise();
  BLEDevice central = BLE.central();
  while (!central) {
    if (!central) {
      Serial.println("Please connect to bluetooth");
    }
  }
  while ( central )
  {
    Serial.print( "Connected to central: " );
    Serial.println( central.address() );

    while ( central.connected() )
    {
      if ( wifiIdPassword.written() )
      {
        ssidPassword = wifiIdPassword.value();
        int indexOfPartition = ssidPassword.indexOf(" ");
        String ssid = ssidPassword.substring(0, indexOfPartition);
        String password = ssidPassword.substring(indexOfPartition + 1);

        Serial.println( ssidPassword.length() );
      }
    }

    Serial.print( F( "Disconnected from central: " ) );
    Serial.println( central.address() );
  }


}

void loop()
{




  //  Serial.print( F( "Disconnected from central: " ) );
  //    Serial.println( central.address() );
}
