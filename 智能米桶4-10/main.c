/*  ��������ʱ��һ�������ͼ������������ģ��ֱ�̳��ڸ��Եĺ����У�Ȼ�����ģ���ĵ��ù�ϵд�����ɺõĺ�����Ȼ�����������е��ü��ɣ�ѭ������Ҫ�Ǹ���ģ��ĳ�ʼ����*/
#include "stdio.h"
#include "stdlib.h"
#include "sys.h"
#include "includes.h"
#include "stm32f10x_exti.h"
//#include "stm32f10x_tim.h"
#include "chumi.h"
#include "weight.h"
#include "lcd.h"
#include "led.h"
#include "temp.h"
#include "usart.h"
#include "picture.h"
//yixia wei tupian xianshi diaoyong 
#include "delay.h"
#include "malloc.h"  
#include "MMC_SD.h" 
#include "ff.h"  
#include "exfuns.h"
#include "fontupd.h"
#include "text.h"	
#include "piclib.h"	
#include "string.h"	
#include "sound.h"
#include "communication.h"

#define WK PAin(0)

//��ʼ����
#define START_TASK_PRIO		10
//�����ջ��С	
#define START_STK_SIZE 		256
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);



//��������
#define CHUMI_TASK_PRIO		4
//�����ջ��С	
#define CHUMI_STK_SIZE 		256
//������ƿ�
OS_TCB CHUMI_TaskTCB;
//�����ջ	
CPU_STK CHUMI_TASK_STK[CHUMI_STK_SIZE];
void chumi_task(void *p_arg);

//��ʾͼƬ����
#define SHOWPIC_TASK_PRIO		5
//�����ջ��С	
#define SHOWPIC_STK_SIZE 		512
//������ƿ�
OS_TCB SHOWPIC_TaskTCB;
//�����ջ	
CPU_STK SHOWPIC_TASK_STK[SHOWPIC_STK_SIZE];
void showpic_task(void *p_arg);


	
////////////////////////////////////////////////////////

u8 temp;
u8 humi;
int count_time=0;
int wk_flag=1;
int wk_count=0;
int sleep_count=0;
char temp_str[20]="1@temp:";
char humi_str[10]="humi:";
extern s32 Weight_Shiwu;
extern char Uart2_Buf[Buf2_Max];//����2���ջ��
extern   int chumi_flag;


void WKUP_Init(void)
{	
  GPIO_InitTypeDef  GPIO_InitStructure;  		  
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;


	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	 //PA.0
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��IO
    //ʹ���ⲿ�жϷ�ʽ
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//�ж���0����GPIOA.0

  EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//���ð������е��ⲿ��·
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	// ��ʼ���ⲿ�ж�

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

}


void send_temp_humi()
{
	   char *t;
		 DHT11_Read_Data(&temp,&humi);	
		 t=convert_temp_humi(temp,humi,temp_str,humi_str);
	   sendmes_to_server(t);
	   myfree(t);
	   
}
//������
int main(void)
{
	
	OS_ERR err;
	CPU_SR_ALLOC();
	
	  delay_init();
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    uart_init(115200); 
	  uart2_init(115200);
	  check_server("1:");
		show_initial();
	  	   
 
	
		mem_init();			//��ʼ���ڲ��ڴ��	
 	  exfuns_init();		//Ϊfatfs��ر��������ڴ�  
    f_mount(fs[0],"0:",1);	//����SD��
	  VS_Init();
	  
    Init_HX711pin();
    Get_Maopi();	
    delay_ms(1000);
  	delay_ms(1000);	 //��ëƤ����
	  Get_Maopi();

     motor_pin_init(); 		 
		 DHT11_Init();
		 
    while(temp==0|humi==0)  //ģ��ոճ�ʼ������Ϊ0
		 {
		 DHT11_Read_Data(&temp,&humi);
		 }	
		 LCD_Clear(WHITE);
		 mp3_play_begin();
		 WKUP_Init();
	OSInit(&err);		    	//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();	//�����ٽ���			 
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);      //����UCOSIII
}


//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif	
	
	
	OS_CRITICAL_ENTER();	//�����ٽ���
								
	//������������
	OSTaskCreate((OS_TCB 	* )&CHUMI_TaskTCB,		
				 (CPU_CHAR	* )"chumi_task", 		
                 (OS_TASK_PTR )chumi_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )CHUMI_TASK_PRIO,     
                 (CPU_STK   * )&CHUMI_TASK_STK[0],	
                 (CPU_STK_SIZE)CHUMI_STK_SIZE/10,	
                 (CPU_STK_SIZE)CHUMI_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);	

		
  //������ʾͼƬ����
		OSTaskCreate((OS_TCB 	* )&SHOWPIC_TaskTCB,		
				 (CPU_CHAR	* )"showpic_task", 		
                 (OS_TASK_PTR )showpic_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )SHOWPIC_TASK_PRIO,     
                 (CPU_STK   * )&SHOWPIC_TASK_STK[0],	
                 (CPU_STK_SIZE)SHOWPIC_STK_SIZE/10,	
                 (CPU_STK_SIZE)SHOWPIC_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);		
								 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
}


//����������
void chumi_task(void *p_arg)
{
    OS_ERR err;
	   while(1)
	 {
		if(chumi_flag==1)
		{
		 LCD_LED=1;
		 OSTaskSuspend((OS_TCB*)&SHOWPIC_TaskTCB,&err);
   	 judge_ser_data();
		 chumi_flag=0;
		 respone_server("1:ok");
		 OSTaskResume((OS_TCB*)&SHOWPIC_TaskTCB,&err);
		 OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
		}
   else
    {
			if((!WK)&&wk_flag==1)  sleep_count++;
			if(sleep_count==100) 
			{
				LCD_LED=0;
				OSTaskSuspend((OS_TCB*)&SHOWPIC_TaskTCB,&err);
				wk_flag=0;
				sleep_count=0;
			}
			if(wk_flag==1)
			{
			   OSTaskResume((OS_TCB*)&SHOWPIC_TaskTCB,&err);
			}
		  OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
		}	
    count_time++;
    if(count_time==100) 
   { 
	  send_temp_humi();
	  count_time=0;
	 }		
	 }		
}

void showpic_task(void *p_arg)
{
	  OS_ERR err;
    CPU_SR_ALLOC();	
	  while(1)
		{
		if(wk_flag==1)
		{
		OS_CRITICAL_ENTER();
    LCD_Clear(WHITE);
    show_picture("0:/PICTURE1/xiaohui.jpg",0,0,lcddev.width,lcddev.height);
		OS_CRITICAL_EXIT();
	  OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_HMSM_STRICT,&err);
		OS_CRITICAL_ENTER();
	  LCD_Clear(WHITE);
	  show_picture("0:/PICTURE2/xiaohui.jpg",0,0,lcddev.width,lcddev.height);
		OS_CRITICAL_EXIT();
	  OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_HMSM_STRICT,&err);
		OS_CRITICAL_ENTER();
		LCD_Clear(WHITE);
    LCD_ShowString(30,40,200,24,24,"WELCOME TO SWJTU...");
	  LCD_ShowString(50,100,150,16,16,"temp:  C,humi:  %");
	  LCD_ShowxNum(90,100,temp,2,16,0); 
	  LCD_ShowxNum(162,100,humi,2,16,0); 	
		OS_CRITICAL_EXIT();	
		OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_HMSM_STRICT,&err);	
    }			
	}

}


void EXTI0_IRQHandler(void)
{
	   if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
		 {
			  wk_count++;
     }
		 if(wk_count==3)
		 {
		    LCD_LED=1;
			  wk_flag=1;
			  wk_count=0;
		 }
		 EXTI_ClearITPendingBit(EXTI_Line0); 
}





