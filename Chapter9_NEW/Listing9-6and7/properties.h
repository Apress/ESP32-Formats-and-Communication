#include <ArduinoIoTCloud.h>        // Arduino IoT Cloud libraries
#include <Arduino_ConnectionHandler.h>   // ESP32 device name and key
const char DEVICE_LOGIN_NAME[] = "xxxx";    // change xxxx to Device login
const char DEVICE_KEY[] = "xxxx";     // change xxxx to Device Key
#include <ssid_password.h>        // file with logon details

void onConnect();     // forward declaration of function

float battery;  // Arduino dashboard variables
int power, cnectTime, countN;

void initProperties()         // details of device name & key
{               //   and dashboard variables
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(battery, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(power, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cnectTime, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(countN, READ, ON_CHANGE, NULL);
}
                // connection to your Wi-Fi router
WiFiConnectionHandler connHandle(ssid, password);
