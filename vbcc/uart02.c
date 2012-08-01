
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void ASMDELAY ( unsigned int );
extern void dummy ( unsigned int );

#define GPFSEL1         0x7E200004
#define GPSET0          0x7E20001C
#define GPCLR0          0x7E200028
#define GPPUD           0x7E200094
#define GPPUDCLK0       0x7E200098

#define AUX_ENABLES     0x7E215004
#define AUX_MU_IO_REG   0x7E215040
//#define AUX_MU_IO_REG    (*((volatile unsigned int *)0x7E215040 ))
#define AUX_MU_IER_REG  0x7E215044
#define AUX_MU_IIR_REG  0x7E215048
#define AUX_MU_LCR_REG  0x7E21504C
#define AUX_MU_MCR_REG  0x7E215050
#define AUX_MU_LSR_REG  0x7E215054
//#define AUX_MU_LSR_REG    (*((volatile unsigned int *)0x7E215054 ))
#define AUX_MU_MSR_REG  0x7E215058
#define AUX_MU_SCRATCH  0x7E21505C
#define AUX_MU_CNTL_REG 0x7E215060
#define AUX_MU_STAT_REG 0x7E215064
#define AUX_MU_BAUD_REG 0x7E215068

//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//alt function 5 for uart1
//alt function 0 for uart0
//((19,000,000/115200)/8)-1 = 19.6
//-------------------------------------------------------------------------
static void uart_init ( void )
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
    PUT32(GPFSEL1,ra);

    PUT32(GPPUD,0);

    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,(1<<14));
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,0);

    PUT32(AUX_MU_CNTL_REG,2);
}
//-------------------------------------------------------------------------
void notmain ( void )
{
    unsigned int ra;

    uart_init();


    //PUT32(AUX_ENABLES,1);
    //PUT32(AUX_MU_IER_REG,0);
    //PUT32(AUX_MU_CNTL_REG,0);
    //PUT32(AUX_MU_LCR_REG,3);
    //PUT32(AUX_MU_MCR_REG,0);
    //PUT32(AUX_MU_IER_REG,0);
    //PUT32(AUX_MU_IIR_REG,0xC6);
    //PUT32(AUX_MU_BAUD_REG,20);

    //ra=GET32(GPFSEL1);
    //ra&=~(7<<12); //gpio14
    //ra|=2<<12;    //alt5
    //PUT32(GPFSEL1,ra);

    //PUT32(GPPUD,0);

    ////ASMDELAY(150);
    //for(ra=0;ra<150;ra++) dummy(ra);
    //PUT32(GPPUDCLK0,(1<<14));
    ////ASMDELAY(150);
    //for(ra=0;ra<150;ra++) dummy(ra);
    //PUT32(GPPUDCLK0,0);

    //PUT32(AUX_MU_CNTL_REG,2);





    ra=0;
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x20)
        {
            PUT32(AUX_MU_IO_REG,0x30+(ra++&7));
        }
    }
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
