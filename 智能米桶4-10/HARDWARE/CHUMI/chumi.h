#ifndef __CHUMI_H
#define __CHUMI_H

#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

static u8 upstep8_table[]={0x08,0x0a,0x02,0x06,0x04,0x05,0x01,0x09};  //正转顺序表
static u8 downstep8_table[]={0x09,0x01,0x05,0x04,0x06,0x02,0x0a,0x08};       //逆转顺序表
void motor_pin_init(void);   //初始化电机引脚
void SetMotor_1(unsigned char InputData);  //设值1号电机引脚电平
void SetMotor_2(unsigned char InputData);  //设值2号电机引脚电平
void SetMotor_3(unsigned char InputData);  //设值3号电机引脚电平
void motorNcircle(int n,int position,int speed,void (*SetMotor)(unsigned char));   

#endif

