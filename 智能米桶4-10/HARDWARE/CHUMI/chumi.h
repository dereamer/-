#ifndef __CHUMI_H
#define __CHUMI_H

#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

static u8 upstep8_table[]={0x08,0x0a,0x02,0x06,0x04,0x05,0x01,0x09};  //��ת˳���
static u8 downstep8_table[]={0x09,0x01,0x05,0x04,0x06,0x02,0x0a,0x08};       //��ת˳���
void motor_pin_init(void);   //��ʼ���������
void SetMotor_1(unsigned char InputData);  //��ֵ1�ŵ�����ŵ�ƽ
void SetMotor_2(unsigned char InputData);  //��ֵ2�ŵ�����ŵ�ƽ
void SetMotor_3(unsigned char InputData);  //��ֵ3�ŵ�����ŵ�ƽ
void motorNcircle(int n,int position,int speed,void (*SetMotor)(unsigned char));   

#endif

