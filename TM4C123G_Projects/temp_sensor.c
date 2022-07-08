#include<tm4c123gh6pm.h>

void I2c_Setup(void)
{
     SYSCTL_RCGCI2C_R |= 0x01;
     SYSCTL_RCGCGPIO_R |= 0x02;
     GPIO_PORTB_PCTL_R |= 0x00003300;
     GPIO_PORTB_CR_R |= 0x1;
     GPIO_PORTB_ODR_R |= 0x08;
     I2C0_MCR_R |= 0x10;
     I2C0_MTPR_R |= 0x9;
     I2C0_MSA_R |= 0x9A;


}