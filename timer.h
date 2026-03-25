#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

// Clock and Routing Registers
#define CLK_SELECT        0x40030028
#define PERI_DIV_16_CTL0  0x40010300
#define PERI_DIV_16_CTL1  0x40010304
#define PERI_DIV_16_CTL2  0x40010308
#define PERI_DIV_CMD      0x40010000
#define PERI_PCLK_CTL6    0x40010118
#define PERI_PCLK_CTL7    0x4001011C
#define PERI_PCLK_CTL8    0x40010120

#define TCPWM_CTRL        0x40200000
#define TCPWM_CMD         0x40200008

// TCPWM Registers (Timer 1)
#define TCPWM_CNT1_CTRL   0x40200140
#define TCPWM_CNT1_COUNTER 0x40200148
#define TCPWM_CNT1_PERIOD  0x40200154

// TCPWM Registers (Timer 2)
#define TCPWM_CNT2_CTRL   0x40200180
#define TCPWM_CNT2_COUNTER 0x40200188
#define TCPWM_CNT2_PERIOD  0x40200194

// GPIO Registers (Port 0 and Port 1)
#define GPIO_PRT0_DR      0x40040000
#define GPIO_PRT0_PC      0x40040008

#define GPIO_PRT1_DR      0x40040100
#define GPIO_PRT1_PC      0x40040108
#define GPIO_PRT1_PS      0x40040110

void hardware_init(void);

#endif
