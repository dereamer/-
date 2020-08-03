#include "chumi.h"


void motor_pin_init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_Init(GPIOB,&GPIO_InitStructure);  
	
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_0|GPIO_Pin_13;
    GPIO_Init(GPIOC,&GPIO_InitStructure);  

}

void SetMotor_1(unsigned char InputData)
{
    if(InputData&0x01)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_0);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_0);
    }
    if(InputData&0x02)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_1);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_1);

    }
    if(InputData&0x04)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_2);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_2);
    }
    if(InputData&0x08)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_3);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_3);
    }
}

void SetMotor_2(unsigned char InputData)
{
    if(InputData&0x01)
    {
        GPIO_SetBits(GPIOC,GPIO_Pin_3);
    }
    else
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_3);
    }
    if(InputData&0x02)
    {
        GPIO_SetBits(GPIOC,GPIO_Pin_2);
    }
    else
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_2);

    }
    if(InputData&0x04)
    {
        GPIO_SetBits(GPIOC,GPIO_Pin_0);
    }
    else
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_0);
    }
    if(InputData&0x08)
    {
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
    }
    else
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
    }
}

void SetMotor_3(unsigned char InputData)
{
    if(InputData&0x01)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_12);
		//	delay_ms(20000);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_12);
    }
    if(InputData&0x02)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_13);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_13);

    }
    if(InputData&0x04)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_14);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_14);
    }
    if(InputData&0x08)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_15);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_15);
    }
}

void motorNcircle(int n,int position,int speed,void (*SetMotor)(unsigned char))
{
    int m,l,k=0;
    for(l=0;l<n;l++)
    {
        //for(i=0;i<64*8;i++)
        for(m=0;m<8;m++)
        {
            for(k=0;k<8;k++)
            {
                if(1 == position)
                {
                    (*SetMotor)(upstep8_table[k]);
                }
                else
               {
                    (*SetMotor)(downstep8_table[k]);
               }
                delay_us(speed);
            }
        }
    }
}


