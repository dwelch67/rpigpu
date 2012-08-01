
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT8 ( unsigned int, unsigned int );
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void BRANCHTO ( unsigned int, unsigned int );
extern void dummy ( unsigned int );
extern void ASMDELAY ( unsigned int );

#define GPFSEL1         0x7E200004
#define GPSET0          0x7E20001C
#define GPCLR0          0x7E200028
#define GPPUD           0x7E200094
#define GPPUDCLK0       0x7E200098

#define AUX_ENABLES     0x7E215004
#define AUX_MU_IO_REG   0x7E215040
#define AUX_MU_IER_REG  0x7E215044
#define AUX_MU_IIR_REG  0x7E215048
#define AUX_MU_LCR_REG  0x7E21504C
#define AUX_MU_MCR_REG  0x7E215050
#define AUX_MU_LSR_REG  0x7E215054
#define AUX_MU_MSR_REG  0x7E215058
#define AUX_MU_SCRATCH  0x7E21505C
#define AUX_MU_CNTL_REG 0x7E215060
#define AUX_MU_STAT_REG 0x7E215064
#define AUX_MU_BAUD_REG 0x7E215068


#define ARM_TIMER_CTL   0x7E00B408
#define ARM_TIMER_CNT   0x7E00B420


#define GPU_TARGET_STACK 0xCFFFFFF0
#define GPU_TARGET_PROGRAM 0xCE000000


//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//alt function 5 for uart1
//alt function 0 for uart0
//((19,000,000/115200)/8)-1 = 19.6
//-------------------------------------------------------------------------
void uart_init ( void )
{
    unsigned int ra;

    PUT32(AUX_ENABLES,1);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_CNTL_REG,0);
    PUT32(AUX_MU_LCR_REG,3);
    PUT32(AUX_MU_MCR_REG,0);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_IIR_REG,0xC6);
    PUT32(AUX_MU_BAUD_REG,20);

    ra=GET32(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=2<<12;    //alt5
    ra&=~(7<<15); //gpio15
    ra|=2<<15;    //alt5
    PUT32(GPFSEL1,ra);

    PUT32(GPPUD,0);
    ASMDELAY(150); //for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,(1<<14)|(1<<15));
    ASMDELAY(150); //for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,0);

    PUT32(AUX_MU_CNTL_REG,3);
}
//-------------------------------------------------------------------------
unsigned int uart_recv ( void )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x01) break;
    }
    return(GET32(AUX_MU_IO_REG));
}
//-------------------------------------------------------------------------
void uart_send ( unsigned int c )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x20) break;
    }
    PUT32(AUX_MU_IO_REG,c);

}
//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//-------------------------------------------------------------------------
void notmain ( void )
{

    unsigned int ra;
    unsigned int rx;
    unsigned int addr;
    unsigned int block;
    unsigned int crc;
    unsigned int new_char;

    uart_init();
    hexstring(0xABCD1234);

    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x01)
        {
            hexstring(GET32(AUX_MU_IO_REG));
        }
        else
        {
            break;
        }
    }



    PUT32(ARM_TIMER_CTL,0x00120000);
    PUT32(ARM_TIMER_CTL,0x00120200);

//SOH 0x01
//ACK 0x06
//NAK 0x15
//EOT 0x04

//block numbers start with 1

//132 byte packet
//starts with SOH
//block number byte
//255-block number
//128 bytes of data
//checksum byte (whole packet)
//a single EOT instead of SOH when done, send an ACK on it too


//this is a very crude solution, worked for a small test program though
//if it slips one byte it is all over.

    rx=GET32(ARM_TIMER_CNT);
    while(1)
    {
        ra=GET32(ARM_TIMER_CNT);
        if((ra-rx)>=4000000)
        {
            uart_send(0x15);
            rx+=4000000;
        }
        if(GET32(AUX_MU_LSR_REG)&0x01) break;
    }
    //hexstring(0x11223344);


    block=1;
    addr=GPU_TARGET_PROGRAM;
    while(1)
    {
        new_char=uart_recv();
        new_char&=0xFF;
        if(new_char!=0x01) break;
        crc=0x01;

        new_char=uart_recv();
        new_char&=0xFF;
        crc+=new_char;
        if(new_char!=block) break;

        new_char=uart_recv();
        new_char&=0xFF;
        crc+=new_char;
        //test for inverted
        //if(new_char!=(~last_char)) break;

        for(ra=0;ra<128;ra++)
        {
            new_char=uart_recv();
            new_char&=0xFF;
            crc+=new_char;
            PUT8(addr++,new_char);
        }
        new_char=uart_recv();
        new_char&=0xFF;
        crc&=0xFF;
        if(crc!=new_char) break;
        uart_send(0x06);
        block=(block+1)&0xFF;
    }
    if(new_char==0x04)
    {
        uart_send(0x06);
        for(ra=0;ra<30;ra++) hexstring(ra);
        BRANCHTO(GPU_TARGET_PROGRAM+0x200,GPU_TARGET_STACK);
    }


}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
