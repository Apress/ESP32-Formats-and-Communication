/*******************************************************************************
 * Sketch name: Map of GPS satellite positions
 * Description: Determine distance and azimuth (direction) angle
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

void appSatellite()        // function to display satellite positions
{
  String str;
  char GPS[100];            // NMEA message
  char * pch;
  char * val[20];           // NMEA message values
  int id[20], elev[20], azim[20], sgnl[20]; // identity, elevation, azimuth,
  float dist[20], alpha, lat, lon, spd;   //     signal and distance arrays
  int Nrec, rec, Nsat, maxVal, strtVal, vals;
  int n, m, minSgnl, maxSgnl, Nsgnl, valSgnl;
  int16_t x, y;
  Serial1.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX, GPS_TX); // Serial output
  tft->fillScreen(TFT_BLACK);
  for (int i=1; i<6; i++)
  {
    tft->drawCircle(120, 120, 20*i, TFT_YELLOW);  // map distance contours
    tft->setTextSize(1);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setCursor(115, (6-i)*20-10);   // distance labels 21k to 25k
    tft->printf("%dk", 20+i);
  }
  while (!ttgo->getTouch(x, y))     // wait for touch
  {
  while (Serial1.available())       // NMEA message available
  {
  str = Serial1.readStringUntil('\n');
  if(str.startsWith("$GPGSV"))      // NMEA satellite message
  {
    str.replace("\,\,","\,0\,");      // replace ,, with ,0,
    str.toCharArray(GPS,str.length());    // convert string to char array
    byte index = 0;
    pch = strtok(GPS, ",");       // c++ strtok function
    while (pch != NULL)         // to parse NMEA message
    {
      val[index] = pch;         // array of values
      index++;
      pch = strtok(NULL, ",");
    }
    Nrec = atoi(val[1]);        // number of records
    rec = atoi(val[2]);         // record number
    Nsat = atoi(val[3]);        // total number of satellites
    maxVal = min(rec*4, Nsat);    // cumulative satellite number in records
    strtVal = (rec-1)*4;        // first satellite number in record
    vals = maxVal-strtVal;        // number of satellites in record
    for (int i=0; i<vals; i++)
    {
      n = strtVal+i;
      id[n] =   atoi(val[4+4*i]);     // identity val[4,  8, 12, 16]
      elev[n] = atoi(val[5+4*i]);     // elevation val[5,  9, 13, 17]
      azim[n] = atoi(val[6+4*i]);     // azimuth val[6, 10, 14, 18]
      sgnl[n] = atoi(val[7+4*i]);     // signal val[7, 11, 15, 19]
      alpha = 6.378*sin(elev[n]*DEG_TO_RAD);  // distance in 1000km
      dist[n] = sqrt(alpha*alpha+2*6.378*20.2+20.2*20.2)-alpha;
    }
    if(rec == Nrec)         // finished processing messages
    {
      minSgnl = 100;          // determine minimum and
      maxSgnl = 0;          // maximum signal strength
      Nsgnl = 0;            // satellite signal received
      valSgnl = 0;          // valid GPS location
      for (int i=0; i<Nsat; i++)
      {             // PI/180 = DEG_TO_RAD
        n = round(120.0 + (dist[i]-20.0)*20.0*sin(azim[i]*PI/180.0));
        m = round(120.0 - (dist[i]-20.0)*20.0*cos(azim[i]*DEG_TO_RAD));
        if(sgnl[i] > 0) tft->fillCircle(n, m, 4, TFT_GREEN);
        else tft->fillCircle(n, m, 4, TFT_RED);   // colour code satellites
        tft->drawString(String(id[i]), n+8, m-8 ,2);  // satellite identity
        if(sgnl[i] > 0) Nsgnl++;        // satellite signals received
        if(sgnl[i] > maxSgnl) maxSgnl = sgnl[i];  // maximum signal
        if(sgnl[i] > 0 && sgnl[i] < minSgnl) minSgnl = sgnl[i];
        if(sgnl[i] > 14) valSgnl++;        // signal strength > 14
      }               // display summary values
      tft->drawString("Nsatell "+String(Nsat)+" ", 0, 0, 2);
      tft->drawString("Nsignal "+String(Nsgnl)+" ", 0, 20, 2);
      str = String(minSgnl)+" - "+String(maxSgnl) + "  ";
      if(str.length() > 9) str = "";    // resolve noise on maxSgnl
      tft->drawString("Signal " + str, 0, 220, 2);
      str = "NO  ";
      if(valSgnl > 3) str = "YES";        // valid GPS location
      tft->drawString("Location " + str, 135, 220, 2);
    }
  }             // end of if(str.startsWith("$GPGSV"))
}             // end of while (Serial1.available())
}             // end of while (!ttgo->getTouch(x, y))
while (ttgo->getTouch(x, y)) {}   // wait for touch release
}
