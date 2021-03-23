#include <msp430.h>

#define WRITE_SIZE  128
#define READ_SIZE   512 //128*4

//FRAM access
#define baseFRAM ((unsigned long*)0x1E04)

//FRAM operations
void FRAMWrite(void);
void FRAMRead(void);

unsigned char count = 0;
unsigned long data;
unsigned long error_check_fram[WRITE_SIZE];     //here you can access 255 locations only
                                                // and it wrap around to read from oth location


#if defined(__TI_COMPILER_VERSION__)
//#pragma PERSISTENT(FRAM_write)                //for single write
//unsigned long FRAM_write[WRITE_SIZE] = {0};

#pragma NOINIT(FRAM_write)
unsigned long FRAM_write[WRITE_SIZE];           //for permanent write
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent unsigned long FRAM_write[WRITE_SIZE] = {0};
#elif defined(__GNUC__)
unsigned long __attribute__((persistent)) FRAM_write[WRITE_SIZE] = {0};
#else
#error Compiler not supported!
#endif

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure GPIO
  P1OUT &= ~BIT0;                           // Clear P1.0 output latch for a defined power-on state
  P1DIR |= BIT0;                            // Set P1.0 to output direction

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Initialize dummy data
  data = 0x00010001;

  while(1)
  {
    //data += 0x00010001;
    FRAMWrite();
    FRAMRead();
    //count++;
    /*if (count > 100)
    {
      P1OUT ^= 0x01;                        // Toggle LED to show 512K bytes
      count = 0;                            // ..have been written
      data = 0x00010001;
    }*/
  }
}

void FRAMWrite(void)
{
  unsigned int i=0;

  for ( i= 0; i< WRITE_SIZE; i++)   //writing 512 blocks of FRAM; 128*4(unsigned long)
  {
    FRAM_write[i] = data;
    data += 0x00010001;
    //added delay for real-time fram write
    //__delay_cycles(100000);
  }
}

void FRAMRead(void)
{
    unsigned int i = 0;
    for(i = 0; i< WRITE_SIZE; i++)
    {
        error_check_fram[i] = *(baseFRAM + i);//getting the address
        //error_check_fram[j] = (baseFRAM + i);//getting the data
    }
}


