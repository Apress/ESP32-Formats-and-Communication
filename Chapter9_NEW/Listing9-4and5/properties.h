#include <ArduinoIoTCloud.h>        // Arduino IoT Cloud libraries
#include <Arduino_ConnectionHandler.h>   // ESP32 device name and key
const char DEVICE_LOGIN_NAME[] = "b53ebd01-b119-49ea-895b-0fe61fa0cb2c";
const char DEVICE_KEY[] = "FCFZCUJDXFEX6BZ93UPR";
#include <ssid_password.h>        // file with logon details

void LEDchange();           // forward declaration of function

bool LED = 0;              // Arduino dashboard variables
int varCO2, varTVOC;

void initProperties()         // details of device name & key
{                            //   and dashboard variables
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(LED, READWRITE, ON_CHANGE, LEDchange);
  ArduinoCloud.addProperty(varCO2, READ, 10 * SECONDS, NULL);
  ArduinoCloud.addProperty(varTVOC, READ, 10 * SECONDS, NULL);
}
                // connection to your Wi-Fi router
WiFiConnectionHandler connHandl(ssid, password);
