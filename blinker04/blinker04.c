
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define LEDBIT      16
#define SCS         0xE01FC1A0
#define IO0SET      0xE0028004
#define IO0DIR      0xE0028008
#define IO0CLR      0xE002800C

#define PCONP       0xE01FC0C4
#define PCLKSEL0    0xE01FC1A8
#define T0TCR       0xE0004004
#define T0TC        0xE0004008
#define T0PR        0xE000400C
#define T0MCR       0xE0004014

#define CLKSRCSEL   0xE01FC10C
#define PLLCON      0xE01FC080
#define PLLCFG      0xE01FC084
#define PLLSTAT     0xE01FC088
#define PLLFEED     0xE01FC08C
#define CCLKCFG     0xE01FC104

void  notmain ( void )
{
    unsigned int ra;
    unsigned int then;
    unsigned int now;

    //enable the main oscillator
    PUT32(SCS,1<<5);
    while((GET32(SCS)&(1<<6))==0) continue;
////1. Disconnect the PLL with one feed sequence if PLL is already connected.
    //PUT32(PLLCON,GET32(PLLCON)&(~(1<<1)));
    //PUT32(PLLFEED,0xAA);
    //PUT32(PLLFEED,0x55);
    //while((GET32(PLLSTAT)&(1<25))!=0) continue;
////2. Disable the PLL with one feed sequence.
    //PUT32(PLLCON,0);
    //PUT32(PLLFEED,0xAA);
    //PUT32(PLLFEED,0x55);
    //while((GET32(PLLSTAT)&(3<24))!=0) continue;
////3. Change the CPU Clock Divider setting to speed up operation without the PLL, if
////desired.
//4. Write to the Clock Source Selection Control register to change the clock source.
    PUT32(CLKSRCSEL,1);
    PUT32(CCLKCFG,(1-1));

    PUT32(SCS,GET32(SCS)&(~1));
    PUT32(IO0DIR,1<<LEDBIT);
    PUT32(IO0SET,1<<LEDBIT);

    PUT32(PCONP,GET32(PCONP)|(1<<1)); //PCTIM0, should be on already.
    PUT32(PCLKSEL0,(GET32(PCLKSEL0)&(~(3<<2)))|(1<<2)); //CCLK/1
    PUT32(T0TCR,0);
    PUT32(T0TCR,2);
    for(ra=0;ra<10;ra++) dummy(ra); //insure a pclk comes along
    PUT32(T0PR,(1-1)); //divide by 1 = PCLK
    PUT32(T0MCR,0); //no features, no interrupts
    PUT32(T0TCR,1); //start timer

    then=GET32(T0TC);
    while(1)
    {
        PUT32(IO0CLR,1<<LEDBIT);
        while(1)
        {
            now=GET32(T0TC);
            if((now-then)>=120000000) break;
        }
        then=now;
        PUT32(IO0SET,1<<LEDBIT);
        while(1)
        {
            now=GET32(T0TC);
            if((now-then)>=120000000) break;
        }
        then=now;
    }
}

//the schematic I have does not show the frequency of the crystal
//for the main clock.  so we have to figure it out.

//if pllcon has the lower two bits zero then the main clock will
//pass through as is, we have to enable the main oscillator in SCS
//wait for it to settle, then we can use it for CCLK.

//at 120,000,000 CCLK ticks I see 10 seconds between led state changes
//that puts the oscillator probably at 12MHz.
