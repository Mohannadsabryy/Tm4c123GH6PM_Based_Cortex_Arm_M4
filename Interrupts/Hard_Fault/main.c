#include "std_types.h"
#include "tm4c123gh6pm_registers.h"

/*
 * Example: Make a fault to show what will occur.
*/

void Leds_Init(void);

int main(void)
{
//    SYSCTL_RCGCGPIO_REG |= 0x20;
//    while(!(SYSCTL_PRGPIO_REG & 0x20));
    /* i will try to access portf registers without enabling clk this will cause a bus fault but since bus fault wasn't enabled hardfault
     * will be triggered by default.
     */
    Leds_Init();
    while(1){

    }
}
void Leds_Init(void){
    GPIO_PORTF_DATA_REG  &=0xF1;
    GPIO_PORTF_DIR_REG   |=0x0E;
    GPIO_PORTF_AFSEL_REG &=0xF1;
    GPIO_PORTF_DEN_REG   |=0x0E;
    GPIO_PORTF_PCTL_REG  &=0xFFFF000F;
}
