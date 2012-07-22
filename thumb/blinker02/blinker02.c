
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define ARM_TIMER_LOD   0x7E00B400
#define ARM_TIMER_VAL   0x7E00B404
#define ARM_TIMER_CTL   0x7E00B408
#define ARM_TIMER_DIV   0x7E00B41C
#define ARM_TIMER_CNT   0x7E00B420

#define SYSTIMERCLO     0x7E003004
#define GPFSEL1         0x7E200004
#define GPSET0          0x7E20001C
#define GPCLR0          0x7E200028

//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    ra=GET32(GPFSEL1);
    ra&=~(7<<18);
    ra|=1<<18;
    PUT32(GPFSEL1,ra);
    //PUT32(GPCLR0,1<<16);

    PUT32(ARM_TIMER_CTL,0x00120000);
    PUT32(ARM_TIMER_CTL,0x00120200);

    while(1)
    {
        PUT32(GPSET0,1<<16);
        while(1)
        {
            ra=GET32(ARM_TIMER_CNT);
            if((ra&0x40000)!=0) break;
        }
        PUT32(GPCLR0,1<<16);
        while(1)
        {
            ra=GET32(ARM_TIMER_CNT);
            if((ra&0x40000)==0) break;
        }
    }
    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

