#include "std_types.h"
#include "tm4c123gh6pm_registers.h"
/*
 *  Example:A roll action is performed by leds each led is lit for 1 second. use systick to count one second using interrupt technique.
*/

#define GLOBAL_INTERRUPT_ENABLE() __asm(" CPSIE I ")

void Systick_Init(void);
void Leds_Init(void);

int counter=1;

void Systick_Handler(void){
    if(counter==1){
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | (0x02);
        counter=2;
    }
    else if(counter==2){
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | (0x04);
        counter=3;
    }
    else if(counter==3){
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | (0x08);
        counter=1;
    }
}
void main(void)
{
    /*i will enable clock of portf in order to use the registers of this port*/
    SYSCTL_RCGCGPIO_REG |=0x20;
    /*i will make polling on the flag to make sure that clock was enabled*/
    while(!(SYSCTL_PRGPIO_REG & 0x20));
    Leds_Init();
    GLOBAL_INTERRUPT_ENABLE();
    Systick_Init();
    while(1){
        __asm(" WFI ");
    }


}
/*Configurations of Systick Timer*/
void Systick_Init(void){
    /*disable systick*/
    SYSTICK_CTRL_REG &=(0x00);
    /*Clear current register*/
    SYSTICK_CURRENT_REG &=(0x000000);
    /*set the number of counts that systick must count to reach 1 second*/
    SYSTICK_RELOAD_REG=15999999;
    /*enable systick*/
    SYSTICK_CTRL_REG |= (0x07);

    /*SET Pirority of systick to be 0*/
    NVIC_SYSTEM_PRI3_REG &= ~(1<<29);
}
/*
 * Configurations of Leds.
 * RED(PF1) BLUE(PF2) GREEN(PF3).
*/
void Leds_Init(void){
    /*I will configure pins as an output pins*/
    GPIO_PORTF_DIR_REG |= (0x0E);
    /*I will configure pins as they will will work as a gpio pins*/
    GPIO_PORTF_AFSEL_REG &= ~(0x0E);
    /* The digital functions for the corresponding pins are enabled*/
    GPIO_PORTF_DEN_REG |= (0x0E);
    /*The analog function of the pins is disabled*/
    GPIO_PORTF_AMSEL_REG &= ~(0x0E);
    /*since AFSEL was cleared so that this pin will work as a gpio pin so that we put 0 in its corresponding bits in PCTL_REG*/
    GPIO_PORTF_PCTL_REG &= (0xFFFF000F);
    /*I will make sure that the bit corresponding red led in data register was cleared*/
    GPIO_PORTF_DATA_REG &= ~(0x0E);
}
