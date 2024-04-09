
#include "std_types.h"
#include "tm4c123gh6pm_registers.h"

/*
 * Example: we will turn on Red Led when switch2 (PF0) pressed and when released led will be turned off
 */

void Led_Init(void);

void Switch_Init(void);

int main(void)
{
    /*i will enable clock of portf in order to use the registers of this port*/
    SYSCTL_RCGCGPIO_REG |= 0x20;
    /*i will make polling on the flag to make sure that clock was enabled*/
    while(!(SYSCTL_PRGPIO_REG & 0x20));
    Led_Init();
    Switch_Init();

    while(1){
        if(GPIO_PORTF_DATA_REG & 0x01){
            GPIO_PORTF_DATA_REG &= ~(1<<1); /*LED OFF*/
        }
        else{
            GPIO_PORTF_DATA_REG |= (1<<1); /*LED ON*/
        }

    }


}
/*Configure Red Led*/
void Led_Init(void){
    /*I will configure pin as an output pin*/
    GPIO_PORTF_DIR_REG |= (1<<1);
    /*I will configure pin as it will work as a gpio pin*/
    GPIO_PORTF_AFSEL_REG &= ~(1<<1);
    /* The digital functions for the corresponding pin are enabled*/
    GPIO_PORTF_DEN_REG |=(1<<1);
    /*The analog function of the pin is disabled*/
    GPIO_PORTF_AMSEL_REG |= ~(1<<1);
    /*since AFSEL was cleared so that this pin will work as a gpio pin so that we put 0 in its corresponding bits in PCTL_REG*/
    GPIO_PORTF_PCTL_REG &= (0xFFFFFF0F);
    /*I will make sure that the bit corresponding red led in data register was cleared*/
    GPIO_PORTF_DATA_REG |= ~(1<<1);
}

/*Configure SW2*/
void Switch_Init(void){
    /*since SW2 connected to PF0 so that we must unlock commit register to put 1 to the corresponding bit*/
    GPIO_PORTF_LOCK_REG = 0x4C4F434B; /*0x4C4F434B this is a value that we used it to put it in the lock register in order to unlock commit register*/
    GPIO_PORTF_CR_REG  |= (1<<0);
    /*I will configure pin as an input pin*/
    GPIO_PORTF_DIR_REG |= ~(1<<0);
    /*I will configure pin as it will work as a gpio pin*/
    GPIO_PORTF_AFSEL_REG &= ~(1<<0);
    /* The digital functions for the corresponding pin are enabled*/
    GPIO_PORTF_DEN_REG |=(1<<0);
    /*The analog function of the pin is disabled*/
    GPIO_PORTF_AMSEL_REG |= ~(1<<0);
    /*since AFSEL was cleared so that this pin will work as a gpio pin so that we put 0 in its corresponding bits in PCTL_REG*/
    GPIO_PORTF_PCTL_REG &= (0xFFFFFFF0);
    /*i will enable pur*/
    GPIO_PORTF_PUR_REG |= (1<<0);
}
