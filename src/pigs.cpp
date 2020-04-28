#include <iostream>
#include <pigpio.h>
#include "DustSensor.h"
#include <thread>
#include <atomic>
#include "../influxdb_cpp/influxdb.hpp"

std::atomic<bool> quit(false);   

void got_signal(int)
{
    quit.store(true);
}

int main()
{
    influxdb_cpp::server_info serverInfo("192.168.1.20", 8086, "ENV_STATUS", "DB_Writer", "AcqDBWritePW");
    gpioInitialise();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    DustSensor ds;
    while(true)
    {
        ds.readAndReset();
        std::cout<<"concentraton: "<< ds.concentration<<std::endl;
        std::string response;
        int returnedValue = influxdb_cpp::builder()
            .meas("RPi3_Sender")
            .tag("SensorModel","PPD42NS")
            .field("Concentration", ds.concentration)
            .post_http(serverInfo,&response);
        std::cout<<returnedValue<<" "<< response<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(30));
        if(quit.load()) 
        {
            break;
        }
    }
    gpioTerminate();
}
