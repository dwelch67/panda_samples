
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
    //use the main (external) oscillator
    PUT32(SCS,1<<5);
    while((GET32(SCS)&(1<<6))==0) continue;
    PUT32(CLKSRCSEL,1);
}
//-------------------------------------------------------------------
void uart_init ( void )
{
    PUT32(PCONP,GET32(PCONP)|(1<<3)); //PCUART0, should be on already.
    PUT32(PCLKSEL0,(GET32(PCLKSEL0)&(~(3<<6)))|(1<<6)); //CCLK/1
    PUT32(PINSEL0,(GET32(PINSEL0)&(~(0xF<<4)))|(0x5<<4)); //TXD0/RXD0
//12000000 Hz PCLK 38400 baud
//dl 0x10 mul 0x09 div 0x02 baud 38352 diff 48
    PUT32(U0ACR,0x00); //no autobaud
    PUT32(U0LCR,0x83); //dlab=1; N81
    PUT32(U0DLL,0x10); //dl = 0x0010
    PUT32(U0DLM,0x00); //dl = 0x0010
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0LCR,0x03); //dlab=0; N81
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0FDR,(0x9<<4)|(0x2<<0)); //mul 0x09, div 0x02
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
