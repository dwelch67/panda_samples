
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

void  notmain ( void )
{
    unsigned int ra;
    unsigned int then;
    unsigned int now;

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
            if((now-then)>=5000000) break;
        }
        then=now;
        PUT32(IO0SET,1<<LEDBIT);
        while(1)
        {
            now=GET32(T0TC);
            if((now-then)>=5000000) break;
        }
        then=now;
    }
}

//8*5,000,000 = 40,000,000 CCLK counts between led state changes
//For me this takes 10 seconds so 4,000,000 MHz is the CCLK rate
//when powered up like this.



