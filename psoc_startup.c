#include <stdint.h>

// Linker script symbols
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

// Function prototypes
void Reset_Handler(void);
void Default_Handler(void);
int main(void);

// Vector Table
__attribute__((section(".isr_vector")))
const void *vector_table[] = {
    &_estack,            // Initial Stack Pointer
    Reset_Handler,       // Reset Handler
    Default_Handler,     // NMI Handler
    Default_Handler,     // Hard Fault Handler
    0,                   // Reserved
    0,                   // Reserved
    0,                   // Reserved
    0,                   // Reserved
    0,                   // Reserved
    0,                   // Reserved
    0,                   // Reserved
    Default_Handler,     // SVC Handler
    0,                   // Reserved
    0,                   // Reserved
    Default_Handler,     // PendSV Handler
    Default_Handler,     // SysTick Handler
    
    // External Interrupts (PSoC 4 specific - filling with Default_Handler)
    // PSoC 4100S Plus has up to 32 external interrupts.
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler
};

void Reset_Handler(void)
{
    // Copy .data section from Flash to RAM
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    // Zero fill .bss section
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    // Call main
    main();

    // Infinite loop if main returns
    while (1);
}

void Default_Handler(void)
{
    while (1);
}
