
#include "std_types.h"
#include "tm4c123gh6pm_registers.h"

/*
 *Example:Using Bit Banding a roll action is performed using the Leds each led for second.The first Led is lit and roll down to the last led
 *        then back to the first Led.This operation will be done Continuously
*/

#define PINS_RED_BLUE_GREEN *(volatile uint32 *)(0x40025038)

#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 364
void Delay_MS(unsigned long long n)
{
    volatile unsigned long long count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}

void LEDS_Init(void);

int main(void)
{
    /*i will enable clock of portf in order to use the registers of this port*/
    SYSCTL_RCGCGPIO_REG |= 0x20;
    /*i will make polling on the flag to make sure that clock was enabled*/
    while(!(SYSCTL_PRGPIO_REG & 0x20));
    LEDS_Init();

    while(1){
        /*lit Red and close blue and green*/
        PINS_RED_BLUE_GREEN=0x02;
        Delay_MS(1000);
        /*lit blue and close Red and green*/
        PINS_RED_BLUE_GREEN=0x04;
        Delay_MS(1000);
        /*lit green and close Red and blue*/
        PINS_RED_BLUE_GREEN=0x08;
        Delay_MS(1000);
    }
}
/*
 * Configurations of Leds.
 * RED(PF1) BLUE(PF2) GREEN(PF3).
*/
void LEDS_Init(void){
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
