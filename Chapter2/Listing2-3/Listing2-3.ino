/*******************************************************************************
 * Sketch name: Functions to sort data and derive main frequency
 * Description: Nested-sort function
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

void sort()          // sort first row of array into descending order
{           //    and retain column numbers
  int temp[2];
  for (int i=1; i<24; i++)        // nested sort for the ith column
  {
    for (int j=0; j<i; j++)       // then column 0 to column (i-1)
    {
      if(x[0][i] > x[0][j])       // if higher column value greater
      {             //    than lower column value
        for (int k=0; k<2; k++)
        {
          temp[k] = x[k][j];
          x[k][j] = x[k][i];        // then swap values
          x[k][i] = temp[k];
} } } } }

float freqCalc()          // function to calculate main frequency
{
  int sum1 = 0, sum2 = 0;
  for (int i=0; i<3; i++)       // for highest three values
  {
    sum1 = sum1 + (24 - x[1][i]) * x[0][i]; // calculate weighted average
    sum2 = sum2 + x[0][i];        //    and average column height
  }
  float freq = 250.0 * sum1/sum2;     // return main frequency to sketch
  return freq;
}
