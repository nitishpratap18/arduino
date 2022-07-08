/*
  This code can on arduino bluetooth can connect with nrf bluetooth and 
  allow us to send data from nrf to Serial monitoor....
*/

#include <ArduinoBLE.h>


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
      }
    }

    Serial.print( F( "Disconnected from central: " ) );
    Serial.println( central.address() );
  }
}
