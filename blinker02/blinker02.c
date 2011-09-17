
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define LEDBIT  16
#define SCS     0xE01FC1A0
#define IO0SET  0xE0028004
#define IO0DIR  0xE0028008
#define IO0CLR  0xE002800C

void  notmain ( void )
{
    unsigned int ra;

    PUT32(SCS,GET32(SCS)&(~1));
    PUT32(IO0DIR,1<<LEDBIT);

    while(1)
    {
        PUT32(IO0CLR,1<<LEDBIT);
        for(ra=0x40000;ra;ra--) dummy(ra);
        PUT32(IO0SET,1<<LEDBIT);
        for(ra=0x40000;ra;ra--) dummy(ra);
    }
}
