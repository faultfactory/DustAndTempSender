#include <iostream>
#include <stdio.h>
#include <pigpio.h>
#include <thread>
#include <atomic>
#include <cstring>
#include "../DS18B20/src/DS18B20.h"
std::atomic<bool> quit(false);   

void got_signal(int)
{
    quit.store(true);
}

// const int tconwriteAddr = B01000000;
// const int tcon_0on_1off = B00001111;

void coutIOIssue(int retval)
{
    switch(retval)
    {
    case PI_BAD_HANDLE:
    {
        std::cout<<"BAD HANDLE"<<std::endl;
        break;
    }
    case PI_I2C_READ_FAILED:
    {
        std::cout<<"I2C READ FAILED"<<std::endl;
        break;
    }
    case PI_I2C_WRITE_FAILED:
    {
        std::cout<<"I2C WRITE FAILED"<<std::endl;
        break;
    }
    case PI_BAD_PARAM:
    {
        std::cout<<"BAD PARAM"<<std::endl;
        break;
    }
    default:
    {
    }
    }
}




int main()
{
    gpioInitialise();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int h = i2cOpen(1,0x18,0x00);
    if(h<0 || h==PI_BAD_HANDLE)
    {
        std::cout<<"i2c open failure"<<std::endl;
        return h;
    }
    std::cout<<"open complete"<<std::endl;

    DS18B20 w1Device("28-0316851d30ff");

    while(true)
    {

        coutIOIssue(i2cWriteByte(h,0xAA));
               
        int stat = i2cReadByte(h);
        coutIOIssue(stat);
        bool firstRun = true; 
        while (stat & 0x20)  // || firstRun) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));;
            firstRun = false;
            stat = i2cReadByte(h);
            coutIOIssue(stat);
            std::cout<<"status loop, stat = "<<(int)stat<<std::endl;
        }

        char* buf = new char[4];
        coutIOIssue(i2cReadDevice(h,buf,4));

        char status = buf[0];

        uint32_t ret;
        ret = buf[1];
        ret <<=8;
        ret |= buf[2];
        ret <<=8;
        ret |= buf[3];

        std::cout<<"Pressure int Is "<<ret<<std::endl;

        float psi = (ret - 0x19999A) * (25.0f - 0.0f);
        psi /= (float)(0xE66666 - 0x19999A);
        psi += 0; 
        float kpa = psi * 6.8947572932;
        std::cout<<"KPA: "<<kpa<<std::endl;

        float tempNow = w1Device.getTemp();

        std::cout<<"Celcius: "<< tempNow<<std::endl;
                


        // std::cout<<'v '<< +value<<std::endl;
        // *txBuf = (char)value;
        // std::cout<<'t '<<*txBuf<<std::endl;
        
        // int outBytes = spiWrite(h,txBuf,1);
        
        // if(outBytes==PI_BAD_HANDLE || outBytes==PI_BAD_SPI_COUNT || outBytes==PI_SPI_XFER_FAILED)
        // {
        //     std::cout<<"write failure"<<std::endl;
        // }
        // else
        // {
        //     // std::cout<<outBytes<<std::endl;
        // }

        // // int v = ((txBuf[1]&3)<<8) | txBuf[2];
        // // printf("%d\n", v);



        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if(quit.load()) 
        {
            spiClose(h);
            std::cout<<"QUITTING"<<std::endl;
            break;
        }
        
     
    }
    gpioTerminate();
}
