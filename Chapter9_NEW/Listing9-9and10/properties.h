#include <ArduinoIoTCloud.h>        // Arduino IoT Cloud libraries
#include <Arduino_ConnectionHandler.h>   // ESP32 device name and key
const char DEVICE_LOGIN_NAME[] = "xxxx";    // change xxxx to Device login
const char DEVICE_KEY[] = "xxxx";     // change xxxx to Device Key
#include <ssid_password.h>        // file with logon details

float kWh = 0, varBattery;  // Arduino dashboard variables
int power, countN, channel;

void initProperties()         // details of device name & key
{               //   and dashboard variables
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(varBattery, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(power, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(countN, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(kWh, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(channel, READ, ON_CHANGE, NULL);
}
                // connection to your Wi-Fi router
WiFiConnectionHandler connHandle(ssid, password);
