# Timers_without_interrupt_CY8CKIT-149_PSOC4100S_Plus_Prototyping_Kit
# PSoC 4 Baremetal Dual Timer Control

This repository demonstrates a purely baremetal (register-direct) C implementation for the Cypress PSoC 4100S Plus microcontroller. The code configures hardware timers using the TCPWM block to independently blink two LEDs, demonstrating precise 16-bit Hardware Clock Divider routing, direct GPIO register manipulation, and real-time polling.

## Features
- **Pure Baremetal**: Exclusively operates via Memory Mapped I/O (MMIO) with zero reliance on higher-level abstractions (PDL, HAL, or PSoC Creator auto-generated APIs).
- **TCPWM Hardware Routing**: Configures the Internal Main Oscillator (IMO) directly into hardware Clock Dividers to generate precise `1kHz` reference clocks.
- **Dynamic Reconfiguration**: The main polling loop securely pauses, reprograms, completely resets, and relaunches the Timer blocks on the fly via the `RELOAD` command based on an active-low switch press.
- **Debounced Polling**: Validates and tracks the resistive pull-up switch state to prevent continuous erratic toggling from mechanical contact bounce.

## Pin Mapping
| Component     | PSoC 4 Pin | Description                                      | Timer Block |
|---------------|------------|--------------------------------------------------|-------------|
| **Red LED**   | `P0.0`     | Strong Drive Output. Active Low/High Indifferent | `TCPWM_CNT1`|
| **Green LED** | `P0.1`     | Strong Drive Output. Active Low/High Indifferent | `TCPWM_CNT2`|
| **Switch**    | `P1.0`     | Input Mode: Resistive Pull-Up (Active Low)       | N/A         |

## Behavior 
### 1. Default State (Startup)
When the board boots up, the two timers behave asynchronously according to their default settings:
- **Red LED (P0.0):** Blinks every **1 second** (1000 ms limit).
- **Green LED (P0.1):** Blinks every **2 seconds** (2000 ms limit).

### 2. Toggled State (Button Pressed)
When the switch on **P1.0** is pressed down to ground (`0` logic state), the firmware intercepts the falling edge and dynamically overwrites the `PERIOD` registers of both timers:
- **Red LED (P0.0):** Blinks every **2 seconds**.
- **Green LED (P0.1):** Blinks every **1 second**.

Pressing the switch again will toggle the timers back to their original behavior.

## How It Works
The system takes advantage of the PSoC 4 Architectural Memory Map:
- **Clocks**: Configures `PERI_DIV_16_CTL1` and `CTL2` identically (divider of 24000) and maps them to `PERI_PCLK_CTL7` and `CTL8`.
- **GPIO**: Directly manages Data output via `GPIO_PRTx_DR` and Drive Mode (Strong Drive = 6, Resistive Pull-up = 3) via `GPIO_PRTx_PC`.
- **Initialization**: Timers are uniquely launched using the `TCPWM_CMD` bitmask `RELOAD` trigger instead of the `START` trigger to guarantee the uninitialized internal counter state machines behave accurately from zero.
- **Hardware Wrapping**: Instead of utilizing interrupts, the `main.c` program spins at roughly 24MHz reading the memory-mapped `TCPWM_CNTx_COUNTER`. Every time the current hardware counter value is smaller than its previous recorded value, the loop infers a Period Wrap has occurred and safely `XOR (^=)` toggles the respective GPIO Data bit.

## Building and Flashing
This project uses a standard Make environment paired with the `arm-none-eabi-gcc` toolchain.

```bash
# Clean previous build artifacts
make clean

# Build the executable (timer.elf / timer.hex)
make all
#flash
make program
```

Flash the generated `timer.hex` file using the Cypress Programmer or a standard SWD programming tool.
