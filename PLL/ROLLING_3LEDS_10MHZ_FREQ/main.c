#include "std_types.h"
#include "tm4c123gh6pm_registers.h"

/*
 * make a Leds Rolling using systick with clk freq 10MHZ using pll to reach this frequency.
*/
void Systick_Init(void);
void Leds_Init(void);
void Pll_Init(void);



#define SYSCTL_RCC_XTAL_MASK           0x000007C0
#define SYSCTL_RCC_XTAL_POS               6
#define SYSCTL_RCC_XTAL_VALUE            0x15
#define SYSCTL_RCC_USESYSDIV_MASK      0x00400000
#define SYSCTL_RCC2_USERCC2_MASK       0x80000000
#define SYSCTL_RCC2_DIV400_MASK        0x40000000
#define SYSCTL_RCC2_SYSDIV_MASK        0x1FC00000
#define SYSCTL_RCC2_SYSDIV_POS            22
#define SYSCTL_RCC2_SYSDIV_VAL            39
#define SYSCTL_RCC2_PWRDN_MASK        0x00002000
#define SYSCTL_RCC2_BYPASS_MASK        0x00000800
#define SYSCTL_RCC2_OSCSRC_MASK        0x00000070
#define SYSCTL_RCC2_OSCSRC_POS            4


int main(void)
{
    /*i will enable clock of portf in order to use the registers of this port*/
    SYSCTL_RCGCGPIO_REG |=0x20;
    /*i will make polling on the flag to make sure that clock was enabled*/
    while(!(SYSCTL_PRGPIO_REG & 0x20));
    Leds_Init();
    Pll_Init();
    Systick_Init();

    while(1){
        /*Led On Red Led and close others*/
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | (0x02);
        while(!((SYSTICK_CTRL_REG) & (1<<16)));

        /*Led On blue Led and close others*/
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | (0x04);
        while(!((SYSTICK_CTRL_REG) & (1<<16)));

        /*Led On Green Led and close others*/
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | (0x08);
        while(!((SYSTICK_CTRL_REG) & (1<<16)));
    }
}
/*Configurations of Systick Timer*/
void Systick_Init(void){
    /*disable systick*/
    SYSTICK_CTRL_REG &=(0x00);
    /*Clear current register*/
    SYSTICK_CURRENT_REG &=(0x000000);
    /*set the number of counts that systick must count to reach 1 second*/
    SYSTICK_RELOAD_REG=9999999;
    /*enable systick*/
    SYSTICK_CTRL_REG |= (0x05);
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
/*PLL Configurations*/
void Pll_Init(void){
    /*Set USERCC2 bit (when something common between RCC,RCC2 and this bit was set so it will be taken from Rcc2)*/
    SYSCTL_RCC2_REG |= (SYSCTL_RCC2_USERCC2_MASK);

    /*I will choose the Main Osc as the clk that will drive pll*/
    SYSCTL_RCC2_REG &= ~(SYSCTL_RCC2_OSCSRC_MASK);

    /*I will set xtal bits to 0x15 as external crystal of MC is 16MHZ so from data sheet page 256 i found that value must be 0x15 */
    SYSCTL_RCC_REG &= ~(SYSCTL_RCC_XTAL_MASK);
    SYSCTL_RCC_REG |= (SYSCTL_RCC_XTAL_VALUE<<SYSCTL_RCC_XTAL_POS);

    /*I will clear DIV400 bit in order to not divide by 2*/
    SYSCTL_RCC2_REG &= ~(SYSCTL_RCC2_DIV400_MASK);

    /*I will set SYSDIV bits by 39 in order to divide by 40 to reach 10MHz */
    SYSCTL_RCC2_REG &= ~(SYSCTL_RCC2_SYSDIV_MASK);
    SYSCTL_RCC2_REG |=  (SYSCTL_RCC2_SYSDIV_VAL<<SYSCTL_RCC2_SYSDIV_POS);

    /*I Will clear PWRDN Bit in order to operate PLL*/
    SYSCTL_RCC2_REG &= ~(SYSCTL_RCC2_PWRDN_MASK);

    /*I will poll on the PLLRIS Bit until PLL READY*/
    while(!(SYSCTL_RIS_REG&0x40));

    /*when Pll ready i will clear bypass bit to make clk src; the clk that coming out from pll*/
    SYSCTL_RCC2_REG &= ~(SYSCTL_RCC2_BYPASS_MASK);

    /*i will set USESYSDIV Bit*/
    SYSCTL_RCC_REG  |=  (SYSCTL_RCC_USESYSDIV_MASK);
}
