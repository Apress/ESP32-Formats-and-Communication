/*******************************************************************************
 * Sketch name: ledc function instructions
 * Description: Control a servo motor
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    13 - Remote control motors
 ******************************************************************************/

freq = 50                // square wave frequency
wavelength = pow(10,6)/freq         // wavelength (s)
resolution = 8              // PWM resolution
pulse = 500+2000*angle/180          // pulse length (s)
duty = round(pulse*(pow(2,resolution)-1)/wavelength)  // scaled duty cycle
ledcWrite(channel, duty)          // generate square wave
