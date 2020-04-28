#include <wiringPi.h>

unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000; 
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

int main(void)
{
    wiringPiSetup ();
    pinMode(8,INPUT);

}

