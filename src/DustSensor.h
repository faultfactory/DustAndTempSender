#include <pigpio.h>
#include <iostream>

class DustSensor
{
    const int inputPin = 5;
    static unsigned long startTick;
    static unsigned long lastTick;
    static bool firstrun;
    static unsigned long highTicks;
    static unsigned long lowTicks;
    
public:
    float ratio = 0.0f;
    float concentration = 0.0f;

    void readAndReset()
    {
        unsigned long measurementInterval= highTicks + lowTicks;
        ratio = 0;
        if(measurementInterval > 0)
        {
            ratio = (float)lowTicks  / ((float)measurementInterval * 100.0f);
            concentration = 1.1*ratio*ratio*ratio - 3.8*ratio*ratio + 520*ratio + 0.62;
        }
        else
        {
            ratio = 0; 
            concentration = 0.0f;
        }
        std::cout<<"ratio: "<<ratio<<std::endl;
        firstrun = true;
        lowTicks = 0;
        highTicks = 0;
    }
    
    static void callBackFunction(int gpio, int level, uint32_t tick)
    {
        std::cout<<"callBackTriggered"<<std::endl;
        unsigned long ticks;
        if(firstrun == false)
        {
            if(tick < lastTick)
            {
                unsigned long offset = 4294967295/2;
                ticks = (tick + offset) - (lastTick - offset);
                std::cout<<"roll over"<<std::endl;
            }
            else
            {
                ticks = lastTick - tick;
            }
            lastTick = tick;

            if(level == 0)
            {
                highTicks += ticks;
            }
            else if(level ==1)
            {
                lowTicks += ticks;
            }
            else
            {
                //nothing;
            }
        }
        else
        {
            firstrun = false;
            startTick = tick;
            lastTick = tick;
        }
    }
    
    DustSensor()    
    {
        gpioSetMode(inputPin,PI_INPUT);
        gpioSetPullUpDown(inputPin,PI_PUD_OFF);
        std::cout<< "i made a dust sensor" <<std::endl;
        gpioSetISRFunc(inputPin,EITHER_EDGE, 4294967295, callBackFunction);
    }
  
};

bool DustSensor::firstrun=true;
unsigned long DustSensor::startTick=0;
unsigned long DustSensor::lastTick=0;
unsigned long DustSensor::highTicks=0;
unsigned long DustSensor::lowTicks=0;
