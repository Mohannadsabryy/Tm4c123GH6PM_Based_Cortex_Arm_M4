#include "std_types.h"
#include "tm4c123gh6pm_registers.h"

/*
 * Example: Make a usage fault to show what will occur.
*/

#define GLOBAL_ENABLE_FAULTS()  __asm(" CPSIE F ")

void Func(uint8 x,uint8 y);
void Leds_Init(void);
void UsageFault_Init(void);
void main(void)
{

    /*
     * here i will trying to divide by zero
     * i created a UsagefaultIsr in the startup code so that when the fault occurs processor was halted in this isr
    */
    GLOBAL_ENABLE_FAULTS();
    UsageFault_Init();
    Func(5,0);
    while(1){

    }

}
void UsageFault_Init(void){
    NVIC_SYSTEM_SYSHNDCTRL |= (1<<18);
    NVIC_SYSTEM_PRI1_REG   |= (1<<21);
    //enable a trap if div by 0 occurs
    NVIC_SYSTEM_CFGCTRL    |= (1<<4);
}
void Func(uint8 x,uint8 y){
    volatile z=x/y;
}
