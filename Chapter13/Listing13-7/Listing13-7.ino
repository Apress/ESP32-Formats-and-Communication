/*******************************************************************************
 * Sketch name: Maintain variation in DC motor speed
 * Description: Extension to Listing 13-6
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    13 - Remote control motors
 ******************************************************************************/

void adjust()
{
  float maxAngle, maxFB, maxLR, maxSum;      // heading angle for
  maxAngle = atan(scalar)*180.0/PI;       // maximum sum value
  maxFB = minSpeed + (maxSpeed - minSpeed) * sin(DEG_TO_RAD*maxAngle);
  maxLR = minSpeed + (maxSpeed - minSpeed) * cos(DEG_TO_RAD*maxAngle);
  maxSum = maxFB + maxLR/scalar;        // maximum sum value
  b = (maxSpeed - minSpeed)/(maxSum - minSpeed);  // slope
  a = minSpeed * (1.0 - b);         // intercept
}
