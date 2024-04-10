#include "std_types.h"
#include "tm4c123gh6pm_registers.h"

/*
 * Example:Generate Edge triggered interrupt on PF4 pin using the SW1.Interrupt Should be triggered on the falling edge.
 *         Turn on blue red when an interrupt occurs.
*/

#define GLOBAL_INTERRUPT_ENABLE()  __asm(" CPSIE I ")

void Led_Init(void);
void Switch_Init(void);
void NVIC_Enable(void);

void GPIOF_ISR(void){
    /*Clear flag*/
    GPIO_PORTF_ICR_REG |= (1<<4);
    /*Lit blue Led*/
    GPIO_PORTF_DATA_REG |= (1<<2);
}

void main(void)
{
    /*Enable clk of portf*/
    SYSCTL_RCGCGPIO_REG |= 0x20;
    /*polling until clk of portf enabled*/
    while(!(SYSCTL_PRGPIO_REG & 0x20));
    /*Enable Led Configurations*/
    Led_Init();
    /*Allow Global Interrupt */
    GLOBAL_INTERRUPT_ENABLE();
    /*Enable External Interrupts from NVIC*/
    NVIC_Enable();
    /*Configure Switch*/
    Switch_Init();
    while(1){
        /*wait until interrupt occurs*/
        __asm(" WFI ");
    }



}
/*Configurations of Blue Led*/
void Led_Init(void){
    GPIO_PORTF_DATA_REG &= ~(1<<2);
    GPIO_PORTF_DIR_REG  |=  (1<<2);
    GPIO_PORTF_AFSEL_REG &=~(1<<2);
    GPIO_PORTF_DEN_REG  |= (1<<2);
    GPIO_PORTF_PCTL_REG &= 0xFFFFF0FF;
}
/*Configurations of Switch*/
void Switch_Init(void){
    GPIO_PORTF_DIR_REG &= ~(1<<4);
    GPIO_PORTF_AFSEL_REG &=~(1<<4);
    GPIO_PORTF_DEN_REG  |= (1<<4);
    GPIO_PORTF_PUR_REG  |= (1<<4);
    GPIO_PORTF_PCTL_REG &= 0xFFF0FFFF;

    GPIO_PORTF_IS_REG  &= ~(1<<4);
    GPIO_PORTF_IBE_REG &= ~(1<<4);
    GPIO_PORTF_IEV_REG &= ~(1<<4);
    GPIO_PORTF_IM_REG  |= (1<<4);
}
/*Configurations of Nvic to enable Edge Triggered Interrupts*/
void NVIC_Enable(void){
    NVIC_EN0_REG  |= (1<<30);
    NVIC_PRI7_REG |= (1<<21);
}
