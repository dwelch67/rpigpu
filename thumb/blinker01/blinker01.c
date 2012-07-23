
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define SYSTIMERCLO     0x7E003004
#define GPFSEL1         0x7E200004
#define GPSET0          0x7E20001C
#define GPCLR0          0x7E200028

//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    ra=GET32(GPFSEL1);
    ra&=~(7<<18);
    ra|=1<<18;
    PUT32(GPFSEL1,ra);

    while(1)
    {
        PUT32(GPSET0,1<<16);
        for(ra=0x100000;ra;ra--) dummy(ra);
        PUT32(GPCLR0,1<<16);
        for(ra=0x100000;ra;ra--) dummy(ra);
    }
    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

