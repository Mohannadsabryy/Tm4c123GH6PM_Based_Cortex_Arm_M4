#include "std_types.h"
#include "tm4c123gh6pm_registers.h"

/*
 * Example: Make a Bus fault to show what will occur.
*/

#define GLOBAL_ENABLE_FAULTS()  __asm(" CPSIE F ")

void Leds_Init(void);
void BusFault_Init(void);
void main(void)
{
    //    SYSCTL_RCGCGPIO_REG |= 0x20;
    //    while(!(SYSCTL_PRGPIO_REG & 0x20));
    /* i will try to access portf registers without enabling clk this will cause a bus fault
     * i created a busfaultIsr in the startup code so that when the fault occurs processor was halted in this isr
    */
    GLOBAL_ENABLE_FAULTS();
    BusFault_Init();
    Leds_Init();
    while(1){

    }

}
void BusFault_Init(void){
    NVIC_SYSTEM_SYSHNDCTRL |= (1<<17);
    NVIC_SYSTEM_PRI1_REG   |= (1<<13);
}
void Leds_Init(void){
    GPIO_PORTF_DATA_REG  &=0xF1;
    GPIO_PORTF_DIR_REG   |=0x0E;
    GPIO_PORTF_AFSEL_REG &=0xF1;
    GPIO_PORTF_DEN_REG   |=0x0E;
    GPIO_PORTF_PCTL_REG  &=0xFFFF000F;
}
