#include <iostream>
#include <stdio.h>
#include <pigpio.h>
#include <thread>
#include <atomic>
#include <cstring>
std::atomic<bool> quit(false);   

void got_signal(int)
{
    quit.store(true);
}

// const int tconwriteAddr = B01000000;
// const int tcon_0on_1off = B00001111;

int main()
{
    gpioInitialise();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int h = spiOpen(0,32000,0x00);
    if(h<0 || h==PI_BAD_HANDLE)
    {
        std::cout<<"SPI open failure"<<std::endl;
        return h;
    }
    std::cout<<"open complete"<<std::endl;
    
    char *txBuf= new char;
    *txBuf =  0x0;
    int outBytes = spiWrite(h,txBuf,1);
    

    bool rising = true;
    uint8_t value = 0;
    while(true)
    {
        std::cout<<'v '<< +value<<std::endl;
        *txBuf = (char)value;
        std::cout<<'t '<<*txBuf<<std::endl;
        
        int outBytes = spiWrite(h,txBuf,1);
        
        if(outBytes==PI_BAD_HANDLE || outBytes==PI_BAD_SPI_COUNT || outBytes==PI_SPI_XFER_FAILED)
        {
            std::cout<<"write failure"<<std::endl;
        }
        else
        {
            // std::cout<<outBytes<<std::endl;
        }

        // int v = ((txBuf[1]&3)<<8) | txBuf[2];
        // printf("%d\n", v);



        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if(quit.load()) 
        {
            spiClose(h);
            std::cout<<"QUITTING"<<std::endl;
            break;
        }
        
        if(rising)
        {
            value++;
            if(value == 0xFF)
            {

                rising = false;
            }
        }
        else
        {   value--;
            if(value == 0x00)
            {

                rising = true;
            }

        }

        
    }
    delete txBuf;
    gpioTerminate();
}
