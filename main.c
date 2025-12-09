#include "timer.h"
int main()
{
    /* HF CLOCK divider init*/
    hfclksel();

    /*GPIO pin init for RGB LED*/
    LEDs_init();

    /* Peripheral clock initializatio*/
    peri_clk_config();

    /*TIMER 0 - INIT*/
    timer0_init();

    while (1)
    {
    }

    return 0;
}
