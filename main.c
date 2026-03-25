#include "timer.h"

int main()
{
    // Initialize the clocks, GPIOs, and the Timer
    hardware_init();

    uint16_t last_t1 = 0;
    uint16_t curr_t1 = 0;

    uint16_t last_t2 = 0;
    uint16_t curr_t2 = 0;

    int mode = 0;        // 0: Red 1s / Green 2s. 1: Red 2s / Green 1s
    int btn_state = 0;   // 0: Released, 1: Pressed

    // Continuous polling loop
    while (1)
    {
        // 1. Poll Switch on P1.0 (Active Low)
        uint8_t is_pressed = (*((volatile uint32_t *)GPIO_PRT1_PS) & (1 << 0)) == 0;
        if (is_pressed)
        {
            if (btn_state == 0) // Rising edge of press
            {
                btn_state = 1;
                mode = !mode;

                // Disable timers to cleanly update Period and safely reset Counters
                *((volatile uint32_t *)TCPWM_CTRL) &= ~((1 << 1) | (1 << 2));

                if (mode == 0)
                {
                    *((volatile uint32_t *)TCPWM_CNT1_PERIOD) = 1000 - 1; // 1s
                    *((volatile uint32_t *)TCPWM_CNT2_PERIOD) = 2000 - 1; // 2s
                }
                else
                {
                    *((volatile uint32_t *)TCPWM_CNT1_PERIOD) = 2000 - 1; // 2s
                    *((volatile uint32_t *)TCPWM_CNT2_PERIOD) = 1000 - 1; // 1s
                }

                // Reset counters to prevent hang if current > new period
                *((volatile uint32_t *)TCPWM_CNT1_COUNTER) = 0;
                *((volatile uint32_t *)TCPWM_CNT2_COUNTER) = 0;
                last_t1 = 0;
                last_t2 = 0;

                // Re-enable and restart timers
                *((volatile uint32_t *)TCPWM_CTRL) |= (1 << 1) | (1 << 2);
                *((volatile uint32_t *)TCPWM_CMD) = (1 << 9) | (1 << 10);
            }
        }
        else
        {
            btn_state = 0; // Released
        }

        // 2. Read the 16-bit hardware counter for Timer 1 (Red LED)
        curr_t1 = (uint16_t)(*((volatile uint32_t *)TCPWM_CNT1_COUNTER));
        if (curr_t1 < last_t1)
        {
            // Toggle Red LED (Pin 0.0)
            *((volatile uint32_t *)GPIO_PRT0_DR) ^= (1 << 0);
        }
        last_t1 = curr_t1;

        // 3. Read the 16-bit hardware counter for Timer 2 (Green LED)
        curr_t2 = (uint16_t)(*((volatile uint32_t *)TCPWM_CNT2_COUNTER));
        if (curr_t2 < last_t2)
        {
            // Toggle Green LED (Pin 0.1)
            *((volatile uint32_t *)GPIO_PRT0_DR) ^= (1 << 1);
        }
        last_t2 = curr_t2;
    }

    return 0;
}
