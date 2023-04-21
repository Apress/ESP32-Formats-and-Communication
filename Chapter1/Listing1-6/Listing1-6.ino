/*******************************************************************************
 * Sketch name: Determine the first 10k prime numbers
 * Description: Benchmark time test for microcontroller
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    1 - ESP32 microcontroller
 ******************************************************************************/

int Nprimes = 9999;        // required number of primes - 1
unsigned long number = 3;     // start from number 3
int count = 2;          // prime number counter
unsigned int start;       // store processing time
unsigned long ms;
int chk, limit, mod, divid;

void setup()
{
  Serial.begin(115200);
  Serial.print("\nCPU "); Serial.println(F_CPU/1000000);
  start = millis();       // start of processing time
}

void loop()
{
  number = number + 2;           // exclude even numbers
  chk = is_prime(number);
  if (chk > 0) count++;       // increment counter when prime
  if (count > Nprimes)
  {
    ms = millis() - start;      // display results
    Serial.printf("Found %d primes in %d ms \n", count, ms);
    Serial.printf("Highest prime is %d \n", number);
    delay(5000);
    number = 3;
    count = 2;
    start = millis();
  }
}

int is_prime(unsigned long num)   // function to check if prime number
{
  limit = sqrt(num);        // check divisors less than square root
  for (int divid = 3; divid <= limit; divid = divid + 2)
  {
    mod = num % divid;        // remainder after dividing
    if (mod == 0) return 0;     // not prime if zero remainder
  }
  return 1;           // no divisor with zero remainder
}
