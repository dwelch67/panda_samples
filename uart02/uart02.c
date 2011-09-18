
//-------------------------------------------------------------------
//-------------------------------------------------------------------

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

#define PINSEL0     0xE002C000

#define U0ACR       0xE000C020
#define U0LCR       0xE000C00C
#define U0DLL       0xE000C000
#define U0DLM       0xE000C004
#define U0IER       0xE000C004
#define U0FDR       0xE000C028
#define U0FCR       0xE000C008
#define U0TER       0xE000C030
#define U0LSR       0xE000C014
#define U0THR       0xE000C000
#define U0RBR       0xE000C000

//-------------------------------------------------------------------
void pll_init ( void )
{
    PUT32(SCS,1<<5);
    while((GET32(SCS)&(1<<6))==0) continue;
    PUT32(CLKSRCSEL,1);
    PUT32(PLLCFG,((1-1)<<16)|(12-1)); //nsel=1, msel=12
    PUT32(PLLFEED,0xAA);
    PUT32(PLLFEED,0x55);
    PUT32(PLLCON,1);
    PUT32(PLLFEED,0xAA);
    PUT32(PLLFEED,0x55);
    while((GET32(PLLSTAT) & (1<<24)) == 0) continue;
    PUT32(CCLKCFG,(4-1)); //cclksel = 4
    while((GET32(PLLSTAT) & (1<<26)) == 0) continue;
    PUT32(PLLCON,3);
    PUT32(PLLFEED,0xAA);
    PUT32(PLLFEED,0x55);
    while((GET32(PLLSTAT) & (1<<25)) == 0) continue;
}
//-------------------------------------------------------------------
void uart_init ( void )
{
    PUT32(PCONP,GET32(PCONP)|(1<<3)); //PCUART0, should be on already.
    PUT32(PCLKSEL0,(GET32(PCLKSEL0)&(~(3<<6)))|(1<<6)); //CCLK/1
    PUT32(PINSEL0,(GET32(PINSEL0)&(~(0xF<<4)))|(0x5<<4)); //TXD0/RXD0
//72000000 Hz PCLK 38400 baud
//dl 0x4A mul 0x0C div 0x07 baud 38407 diff 7
    PUT32(U0ACR,0x00); //no autobaud
    PUT32(U0LCR,0x83); //dlab=1; N81
    PUT32(U0DLL,0x4A); //dl = 0x004A
    PUT32(U0DLM,0x00); //dl = 0x004A
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0LCR,0x03); //dlab=0; N81
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0FDR,(0xC<<4)|(0x7<<0)); //mul 0x0C, div 0x07
    PUT32(U0FCR,(1<<2)|(1<<1)|(1<<0)); //enable and reset fifos
    PUT32(U0TER,(1<<7)); //transmit enable
}
//-------------------------------------------------------------------
void uart_putc ( unsigned int x )
{
    while (( GET32(U0LSR) & (1<<5)) == 0) continue;
    PUT32(U0THR,x);
}
//-------------------------------------------------------------------
unsigned int uart_getc ( void )
{
    while (( GET32(U0LSR) & (1<<0)) == 0) continue;
    return(GET32(U0RBR));
}
//-------------------------------------------------------------------
void hexstring ( unsigned int d, unsigned int cr )
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
        uart_putc(rc);
        if(rb==0) break;
    }
    if(cr)
    {
        uart_putc(0x0D);
        uart_putc(0x0A);
    }
    else
    {
        uart_putc(0x20);
    }
}
//-------------------------------------------------------------------
void  notmain ( void )
{
    pll_init();
    uart_init();
    hexstring(0x12345678,1);
    hexstring(0x12345678,1);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
