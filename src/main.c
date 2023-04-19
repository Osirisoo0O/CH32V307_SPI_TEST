#include "debug.h"

static void SPI1_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1,ENABLE);
    GPIO_InitTypeDef gpio_def={0};

    gpio_def.GPIO_Pin=GPIO_Pin_4;   //CS
    gpio_def.GPIO_Mode=GPIO_Mode_Out_PP;
    gpio_def.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio_def);
    GPIO_SetBits(GPIOA,GPIO_Pin_4);

    gpio_def.GPIO_Pin=GPIO_Pin_5;   //SCK
    gpio_def.GPIO_Mode=GPIO_Mode_AF_PP;
    gpio_def.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio_def);

    gpio_def.GPIO_Pin=GPIO_Pin_6;   //MISO
    gpio_def.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    gpio_def.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio_def);

    gpio_def.GPIO_Pin=GPIO_Pin_7;   //MOSI
    gpio_def.GPIO_Mode=GPIO_Mode_AF_PP;
    gpio_def.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio_def);

    SPI_InitTypeDef spi_def={0};
    spi_def.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    spi_def.SPI_Mode=SPI_Mode_Master;
    spi_def.SPI_DataSize=SPI_DataSize_8b;
    spi_def.SPI_CPOL=SPI_CPOL_Low;
    spi_def.SPI_CPHA=SPI_CPHA_1Edge;
    spi_def.SPI_NSS=SPI_NSS_Soft;
    spi_def.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_64;
    spi_def.SPI_FirstBit=SPI_FirstBit_MSB;
    spi_def.SPI_CRCPolynomial=7;
    SPI_Init(SPI1,&spi_def);

    SPI_Cmd(SPI1,ENABLE);

}
static void SPI2_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

    GPIO_InitTypeDef gpio_def={0};

    gpio_def.GPIO_Pin=GPIO_Pin_12;   //CS
    gpio_def.GPIO_Mode=GPIO_Mode_IPD;
    gpio_def.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gpio_def);

    gpio_def.GPIO_Pin=GPIO_Pin_13;   //SCK
    gpio_def.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    gpio_def.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gpio_def);

    gpio_def.GPIO_Pin=GPIO_Pin_14;   //MISO
    gpio_def.GPIO_Mode=GPIO_Mode_AF_PP;
    gpio_def.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gpio_def);

    gpio_def.GPIO_Pin=GPIO_Pin_15;   //MOSI
    gpio_def.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    gpio_def.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gpio_def);

    SPI_InitTypeDef spi_def={0};
    spi_def.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    spi_def.SPI_Mode=SPI_Mode_Slave;
    spi_def.SPI_DataSize=SPI_DataSize_8b;
    spi_def.SPI_CPOL=SPI_CPOL_Low;
    spi_def.SPI_CPHA=SPI_CPHA_1Edge;
    spi_def.SPI_NSS=SPI_NSS_Hard;
    spi_def.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_64;
    spi_def.SPI_FirstBit=SPI_FirstBit_MSB;
    spi_def.SPI_CRCPolynomial=7;
    SPI_Init(SPI2,&spi_def);

    SPI_Cmd(SPI2,ENABLE);

}
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("System Clock: %d \r\n",(int)SystemCoreClock);


    SPI1_Init();
    SPI2_Init();


    while (1)
    {
        u8 data1=0;
        u8 data2 = 0;
        GPIO_ResetBits(GPIOA,GPIO_Pin_4);

        while (SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET){}
        SPI_I2S_SendData(SPI2,0x33);
        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET){}
        SPI_I2S_SendData(SPI1,0x22);
        while (SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET){}
        data2=SPI_I2S_ReceiveData(SPI2);

        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET){}
        data1=SPI_I2S_ReceiveData(SPI1);

        GPIO_SetBits(GPIOA,GPIO_Pin_4);

        printf("rx1 = %d, rx2 = %d \r\n",data1,data2);

        Delay_Ms(500);
    }
    
}