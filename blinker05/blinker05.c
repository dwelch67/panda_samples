
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

//from blinker04 we figured out that the main oscillator is 12MHz.
//the pll has a pre-divider (n) and a multilpier (m) and we have to
//get the pll between 275 and 550 MHz.
// (275/2)/12 = 11.45
// (550/2)/12 = 22.92
// so with an n of divide by 1 then we multiply between 12 and 22
// 12 * 12 * 2 = 288MHz
// 72MHz is the max CCLK rate for this cpu.  The cclk divisor has a
// rule of being either divide by 1 or an even divisor.
// 288MHz/72MHz = 4 a whole number and even numbered divisor.
// so that will work nsel=1, msel=12, cclk divisor = 4

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
//5. Write to the PLLCFG and make it effective with one feed sequence. The PLLCFG can
//only be updated when the PLL is disabled.
    PUT32(PLLCFG,((1-1)<<16)|(12-1)); //nsel=1, msel=12
    PUT32(PLLFEED,0xAA);
    PUT32(PLLFEED,0x55);
//6. Enable the PLL with one feed sequence.
    PUT32(PLLCON,1);
    PUT32(PLLFEED,0xAA);
    PUT32(PLLFEED,0x55);
    //6.1 not listed in instructions
    while((GET32(PLLSTAT) & (1<<24)) == 0) continue;
//7. Change the CPU Clock Divider setting for the operation with the PLL. It's critical to do
//this before connecting the PLL.
    PUT32(CCLKCFG,(4-1)); //cclksel = 4
//8. Wait for the PLL to achieve lock by monitoring the PLOCK bit in the PLLSTAT register,
    while((GET32(PLLSTAT) & (1<<26)) == 0) continue;
//9. Connect the PLL with one feed sequence.
    PUT32(PLLCON,3);
    PUT32(PLLFEED,0xAA);
    PUT32(PLLFEED,0x55);
    //9.1 not listed in instructions
    while((GET32(PLLSTAT) & (1<<25)) == 0) continue;
//It's very important not to merge any steps above. For example, don't update the PLLCFG
//and enable the PLL simultaneously with the same feed sequence

    PUT32(SCS,GET32(SCS)&(~1));
    PUT32(IO0DIR,1<<LEDBIT);
    PUT32(IO0SET,1<<LEDBIT);

    PUT32(PCONP,GET32(PCONP)|(1<<1)); //PCTIM0, should be on already.
    PUT32(PCLKSEL0,GET32(PCLKSEL0)|(3<<2)); //CCLK/8
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
            if((now-then)>=90000000) break;
        }
        then=now;
        PUT32(IO0SET,1<<LEDBIT);
        while(1)
        {
            now=GET32(T0TC);
            if((now-then)>=90000000) break;
        }
        then=now;
    }
}

//using a PCLKSEL of CCLK/8  72/8 = 9MHz counting the timer to 90Million
//should be 10 seconds...and it is.


