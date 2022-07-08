

#include<tm4c123gh6pm.h>
#include<stdint.h>

void I2c_Setup(void)
{
     SYSCTL_RCGCI2C_R |= 0x01;
     SYSCTL_RCGCGPIO_R |= 0x02;
     while((SYSCTL_PRGPIO_R&0x0002)==0){};
     GPIO_PORTB_AFSEL_R |= 0x0C;
     GPIO_PORTB_PCTL_R |= 0x00003300;
     GPIO_PORTB_CR_R |= 0x1;
     GPIO_PORTB_DEN_R |= 0x0C;
     GPIO_PORTB_ODR_R |= 0x08;
     I2C0_MCR_R = 0x10;
     I2C0_MTPR_R = (500/20 -1);
     I2C0_MSA_R |= 0x9A;


}

/*
 * Transmit to address 0x00 -> 0x7F, check acknowledge bit to see if slave responds, then return which address acknowledged the address
 */
uint32_t I2C_Detect(void)
{
    uint8_t address = 0;

    while(I2C0_MCS_R&0x00000001){}; //Ensure controller is avaliable

    for(address = 0; address <= 0x7F; address++)
    {
        printf("%d: ",address);
        while(I2C0_MCS_R&0x00000001){}; //Ensure controller is avaliable
        I2C0_MSA_R = (address<<1)&0xFE;

        I2C0_MSA_R &= ~0x01;

        I2C0_MCS_R = (I2C_MCS_START | I2C_MCS_RUN);

        while(I2C0_MCS_R&0x00000001){};
        if((I2C0_MCS_R&I2C_MCS_ADRACK)!=0){

            printf("Not Acknowledged\n");
            I2C0_MCS_R = (I2C_MCS_STOP);


        }
        else
            printf("Acknowledged\n");
        I2C0_MCS_R = (I2C_MCS_STOP);
    };
    return 0;
}

uint32_t I2C_Send2(uint8_t slave, uint8_t data1, uint8_t data2)
{
    while(I2C0_MCS_R&0x00000001){};
    I2C0_MSA_R = (slave<<1)&0xFE;
    I2C0_MSA_R &= ~0x01;
    I2C0_MCS_R = (I2C_MCS_START | I2C_MCS_RUN);

    while(I2C0_MCS_R&0X00000001){};

    if((I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0)
    {
        I2C0_MCS_R = I2C_MCS_STOP;
        return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));

    }
    I2C0_MDR_R = data2&0xFF;
    I2C0_MCS_R = (I2C_MCS_STOP | I2C_MCS_RUN);
    while(I2C0_MCS_R&0x00000001){};
    return(I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
}

int main(void)
  {

    int error = 0;

    I2c_Setup();

    uint32_t address = I2C_Detect();

    printf("%d", address);
    //0000 1110 1000 0000

	return 0;
}
