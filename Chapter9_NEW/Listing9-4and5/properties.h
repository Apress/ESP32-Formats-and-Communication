#include <ArduinoIoTCloud.h>        // Arduino IoT Cloud libraries
#include <Arduino_ConnectionHandler.h>   // ESP32 device name and key
const char DEVICE_LOGIN_NAME[] = "xxxx";    // change xxxx to Device login
const char DEVICE_KEY[] = "xxxx";     // change xxxx to Device Key
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
