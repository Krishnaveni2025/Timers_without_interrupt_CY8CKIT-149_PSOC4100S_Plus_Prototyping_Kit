#ifndef TIMER_H_
#define TIMER_H_
#include <stdint.h>
// RED LED connected to P0.0
// GREEN LED connected to P0.1
// BLUE LED connected to P0.2
// Addresses
#define CLK_SELECT 0x40030028       // Clock Select Register
#define PERI_DIV_16_CTL0 0x40010300 // Divider control register (for 16.0 divider)
#define PERI_DIV_CMD 0x4001000      // Divider command register
#define PERI_PCLK_CTL6 0x40010118   // Programmable clock control register of peripheral 6

#define TCPWM_CMD 0x40200008  // TCPWM command register.
#define TCPWM_CTRL 0x40200000 // TCPWM control register 0

#define TCPWM_CNT0_COUNTER 0x40200108  // Counter count register
#define TCPWM_CNT0_TR_CTRL2 0x40200128 // Counter trigger control register 2
#define TCPWM_CNT0_PERIOD 0x40200114   // Counter period register
#define TCPWM_CNT0_CTRL 0x40200100     // Counter control register

/* Timer period in milliseconds */
#define TIMER0_PERIOD_MSEC 2000U
#define TIMER1_PERIOD_MSEC 2000U
#define TIMER2_PERIOD_MSEC 2000U

#define ENABLE 1
#define DISABLE 0

volatile uint32_t *cs_register = (volatile uint32_t *)CLK_SELECT;

volatile uint32_t *cmd_register = (volatile uint32_t *)PERI_DIV_CMD;
volatile uint32_t *div0_register = (volatile uint32_t *)PERI_DIV_16_CTL0;
volatile uint32_t *peri_register = (volatile uint32_t *)PERI_PCLK_CTL6;

/*common Registers*/
volatile uint32_t *timer_ctrl_reg = (volatile uint32_t *)TCPWM_CTRL;
volatile uint32_t *timer_cmd_reg = (volatile uint32_t *)TCPWM_CMD;

volatile uint32_t *t0_ctrl_cnt_reg = (volatile uint32_t *)TCPWM_CNT0_COUNTER;
volatile uint32_t *t0_ctrl2_cnt_reg = (volatile uint32_t *)TCPWM_CNT0_TR_CTRL2;
volatile uint32_t *t0_cnt_prd_reg = (volatile uint32_t *)TCPWM_CNT0_PERIOD;
volatile uint32_t *t0_cnt0_ctrl_reg = (volatile uint32_t *)TCPWM_CNT0_CTRL;

void hfclksel(void);
void peri_clk_config(void);
void div_ED(int ED);
void config_div(int value);
void assignclk_peri(void);
void timer0_init(void);
void timer1_init(void);
void timer2_init(void);

void LEDs_init(void);

#endif