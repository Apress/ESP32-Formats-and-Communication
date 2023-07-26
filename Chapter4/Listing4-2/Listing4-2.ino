/*******************************************************************************
 * Sketch name: Configuration sketch
 * Description: Required libraries
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

#define LILYGO_WATCH_2020_V2        // define T-Watch model as V2
#include <LilyGoWatch.h>        // include LilyGoWatch library
#include <HTTPClient.h>         // libraries for accessing
#include <WiFi.h>           //      Wi-Fi and URLs
#include <ssid_password.h>        // file with ssid and passwords
#include <ArduinoJson.h>        // library for JSON documents
#include <soc/rtc.h>          // library for real-time clock
#include <BluetoothSerial.h>        // library for Bluetooth
#include <IRremoteESP8266.h>        // libraries for infrared signal
#include <IRsend.h>         //      transmission
#include "images.h"         // icon image files
#include "image565.h"         // colour image file
