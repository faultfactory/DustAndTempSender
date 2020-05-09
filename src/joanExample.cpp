#include <stdio.h>

#include <pigpio.h>

int main(int argc, char *argv[])
{
   int i;

   int h_mcp4131_w;

   const int PV=257;

   char txBuf[3];
   unsigned v, w;

   double start, duration;

   int speed, iters, maxsps, diags;

   uint32_t nt;
   unsigned micros;
   int tdiff;

   unsigned readings[4096];

   speed = 1000000;
   iters = 1000;
   maxsps = 100000;
   diags = 0;

   if (argc > 1) speed = atoi(argv[1]);
   if (argc > 2) iters = atoi(argv[2]);
   if (argc > 3) maxsps = atoi(argv[3]);
   if (argc > 4) diags = 1;

   micros = 1000000 / maxsps;

   if (gpioInitialise() < 0) return 1;

   h_mcp4131_w = spiOpen(0, speed, 0x03); /* set mode 3, no reason */

   txBuf[0] = 1;
   txBuf[2] = 0;

   start = time_time();

   txBuf[2] = 0;

   for (i=0; i<1000; i++)
   {
      txBuf[0] = 0; /* write wiper 0 */
      txBuf[1] = i%PV; /* 0 - 128 */
      spiXfer(h_mcp4131_w, txBuf, rxBuf, 2);

      if (diags) printf("set=%3u", i%PV);

      if ((i%PV) != w) printf("mismatch set=%d read=%d\n", i%PV, w);
   }
}