#include "timer.h"

// Default periods for Timer 1 (1s) and Timer 2 (2s)
#define TIMER1_PERIOD_MSEC 1000U
#define TIMER2_PERIOD_MSEC 2000U

void hardware_init(void)
{
    // 1. Configure the HF CLOCK
    *((volatile uint32_t *)CLK_SELECT) = (0 << 3); 

    // 2. Configure 16-bit Clock Divider 1 and 2 to generate 1 kHz clocks
    // Disable Divider 1 and 2
    *((volatile uint32_t *)PERI_DIV_CMD) = (1UL << 30) | (3 << 14) | (63 << 8) | (1 << 6) | (1 << 0);
    *((volatile uint32_t *)PERI_DIV_CMD) = (1UL << 30) | (3 << 14) | (63 << 8) | (1 << 6) | (2 << 0);
    // Set division values: 24000 - 1
    *((volatile uint32_t *)PERI_DIV_16_CTL1) = (24000 - 1) << 8;
    *((volatile uint32_t *)PERI_DIV_16_CTL2) = (24000 - 1) << 8;
    // Enable Divider 1 and 2
    *((volatile uint32_t *)PERI_DIV_CMD) = (1UL << 31) | (3 << 14) | (63 << 8) | (1 << 6) | (1 << 0);
    *((volatile uint32_t *)PERI_DIV_CMD) = (1UL << 31) | (3 << 14) | (63 << 8) | (1 << 6) | (2 << 0);

    // 3. Route Clock Dividers to TCPWM Peripherals
    *((volatile uint32_t *)PERI_PCLK_CTL7) = (1 << 6) | (1 << 0);  // Divider 1 to TCPWM1
    *((volatile uint32_t *)PERI_PCLK_CTL8) = (1 << 6) | (2 << 0);  // Divider 2 to TCPWM2

    // 4. Initialize GPIO for LEDs and Switch
    // Set Data Registers logic High (LED Off, Switch Pull-up initialized High)
    *((volatile uint32_t *)GPIO_PRT0_DR) |= (1 << 0) | (1 << 1); 
    *((volatile uint32_t *)GPIO_PRT1_DR) |= (1 << 0);            

    // Set Drive mode to Strong Drive (6) for P0.0 (Red) and P0.1 (Green)
    *((volatile uint32_t *)GPIO_PRT0_PC) = (*((volatile uint32_t *)GPIO_PRT0_PC) & ~((7 << 0) | (7 << 3))) | (6 << 0) | (6 << 3);
    
    // Set Drive mode to Resistive Pull-Up (3) for Switch on P1.0
    *((volatile uint32_t *)GPIO_PRT1_PC) = (*((volatile uint32_t *)GPIO_PRT1_PC) & ~(7 << 0)) | (3 << 0);

    // 5. Initialize TCPWM1 and TCPWM2
    // Disable TCPWM1 and TCPWM2
    *((volatile uint32_t *)TCPWM_CTRL) &= ~((1 << 1) | (1 << 2));
    
    // Timer 1 setup
    *((volatile uint32_t *)TCPWM_CNT1_COUNTER) = 0;
    *((volatile uint32_t *)TCPWM_CNT1_CTRL) = 0;
    *((volatile uint32_t *)TCPWM_CNT1_PERIOD) = (TIMER1_PERIOD_MSEC - 1);

    // Timer 2 setup
    *((volatile uint32_t *)TCPWM_CNT2_COUNTER) = 0;
    *((volatile uint32_t *)TCPWM_CNT2_CTRL) = 0;
    *((volatile uint32_t *)TCPWM_CNT2_PERIOD) = (TIMER2_PERIOD_MSEC - 1);

    // Enable TCPWM1 and TCPWM2
    *((volatile uint32_t *)TCPWM_CTRL) |= (1 << 1) | (1 << 2);

    // 6. Start TCPWM1 and TCPWM2 using RELOAD commands
    // Bit 9 is Timer 1, Bit 10 is Timer 2
    *((volatile uint32_t *)TCPWM_CMD) = (1 << 9) | (1 << 10);
}
