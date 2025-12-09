#include "timer.h"

void hfclksel(void)
{
    // Set the clock select register to use the high-frequency clock
    *cs_register = 0 << 3; //[3:2] 0 - no divider ; 1- divide by 2; 2- divide by 4(default); 3- divide by 8; source selection[1:0]0-IMO(default); 1-EXTCLK; 2-ECO
}
void peri_clk_config(void)
{
    // 1. Disable the Specific divider
    div_ED(DISABLE);
    // 2.configure that specific divider value
    config_div(4); // Divide by 4
    // 3.Enable the specific divider
    div_ED(ENABLE);
    // 4.Assign this clock to the required peripheral
    assignclk_peri();
}
void div_ED(int ED)
{
    // Enable or Disable the divider based on the input parameter
    if (ED == ENABLE) // 0th bit is enable bit
    {
        *cmd_register |= (1 << 31) | (3 << 14) | (63 << 8) | (1 << 6) | (3 << 0); // PERI_DIV_CMD
        // Enable the divder 31:bit, Keep 3 at 15:14 and 63 13:8 this selects the HFCLK as reference , Select 16 bit divider 7:6, and Select the divider no 3 using 5:0;
    }
    else
    {
        *cmd_register &= ~(1 << 30); // Disable the divider by clearing the 30th bit PERI_DIV_CMD
    }
}
void config_div(int value)
{
    // using divider 0
    *div0_register = (value - 1) << 8; // Integer division by (1+INT16_DIV). Allows for integer divisions in the range [1, 65,536]. Note: this type of divider does NOT allow for a fractional division.
}
void assignclk_peri(void)
{
    // Assign the configured divider to the required peripheral
    *peri_register = (1 << 6) | (3 << 0); // Specify Divider type 7:6 and Selected Divider 3:0 in register PERI_PCLK_CTL6 TCPWM1 is PERIPHERAL 6
}
void timer0_init(void)
{
    *timer_ctrl_reg &= ~(1 << 0);               // Disable Timer 0  in TCPWM_CTRL Register
    *t0_ctrl_cnt_reg = 0;                       // Clear the counter register of  TCPWM0 TCPWM_CNT0_COUNTER Register
    *t0_ctrl2_cnt_reg = 0;                      // Clear the  register of  TCPWM0 TCPWM_CNT0_TR_CTRL2 Register
    *t0_cnt_prd_reg = (TIMER0_PERIOD_MSEC - 1); // Set the Period Register of TCPWM0 TCPWM_CNT0_PERIOD Register
    *t0_cnt0_ctrl_reg |= (0 << 24);             // Mode configuration of for TCPWM0, TCPWM_CNT2_CTRL Regsiter
    *timer_ctrl_reg |= (1 << 0);                // Enable Timer 0  in TCPWM_CTRL Register
    *timer_cmd_reg = (1 << 24);                 // Triger start Timer 0  in TCPWM_CMD Register
}
void timer1_init(void)
{
    // Timer 1 initialization code can be added here
}
void timer2_init(void)
{
    // Timer 2 initialization code can be added here
}
void LEDs_init(void)
{
    // GPIO pin init for RGB LED
    *((uint32_t *)0x40040000) = (0 << 0) | (0 << 1) | (0 << 2); // Set default output value of P0.0, P0.1, P0.2 to 0 in GPIO_PRT0_DR
    *((uint32_t *)0x40040008) = (6 << 0) | (6 << 4) | (6 << 8); // Set drive mode of P0.0, P0.1, P0.2 to Digital OP Push Pull in GPIO_PRT0_PC
}