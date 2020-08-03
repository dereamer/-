/*  Ö÷º¯Êýµ½Ê±ºò»­Ò»¸ö³ÌÐò¿òÍ¼£¬½«¸÷¸ö¹¦ÄÜÄ£¿é·Ö±ð¼Ì³ÐÔÚ¸÷×ÔµÄº¯ÊýÖÐ£¬È»ºó¸ù¾ÝÄ£¿é¼äµÄµ÷ÓÃ¹ØÏµÐ´³ö¼¯³ÉºÃµÄº¯Êý£¬È»ºóÔÚÖ÷º¯ÊýÖÐµ÷ÓÃ¼´¿É£¬Ñ­»·ÍâÖ÷ÒªÊÇ¸÷¸öÄ£¿éµÄ³õÊ¼»¯¡£*/
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

//¿ªÊ¼ÈÎÎñ
#define START_TASK_PRIO		10
//ÈÎÎñ¶ÑÕ»´óÐ¡	
#define START_STK_SIZE 		256
//ÈÎÎñ¿ØÖÆ¿é
OS_TCB StartTaskTCB;
//ÈÎÎñ¶ÑÕ»	
CPU_STK START_TASK_STK[START_STK_SIZE];
//ÈÎÎñº¯Êý
void start_task(void *p_arg);



//³öÃ×ÈÎÎñ
#define CHUMI_TASK_PRIO		4
//ÈÎÎñ¶ÑÕ»´óÐ¡	
#define CHUMI_STK_SIZE 		256
//ÈÎÎñ¿ØÖÆ¿é
OS_TCB CHUMI_TaskTCB;
//ÈÎÎñ¶ÑÕ»	
CPU_STK CHUMI_TASK_STK[CHUMI_STK_SIZE];
void chumi_task(void *p_arg);

//ÏÔÊ¾Í¼Æ¬ÈÎÎñ
#define SHOWPIC_TASK_PRIO		5
//ÈÎÎñ¶ÑÕ»´óÐ¡	
#define SHOWPIC_STK_SIZE 		512
//ÈÎÎñ¿ØÖÆ¿é
OS_TCB SHOWPIC_TaskTCB;
//ÈÎÎñ¶ÑÕ»	
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
extern char Uart2_Buf[Buf2_Max];//´®¿Ú2½ÓÊÕ»º´
extern   int chumi_flag;


void WKUP_Init(void)
{	
  GPIO_InitTypeDef  GPIO_InitStructure;  		  
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;


	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	 //PA.0
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;//ÉÏÀ­ÊäÈë
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//³õÊ¼»¯IO
    //Ê¹ÓÃÍâ²¿ÖÐ¶Ï·½Ê½
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//ÖÐ¶ÏÏß0Á¬½ÓGPIOA.0

  EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//ÉèÖÃ°´¼üËùÓÐµÄÍâ²¿ÏßÂ·
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//ÉèÍâÍâ²¿ÖÐ¶ÏÄ£Ê½:EXTIÏßÂ·ÎªÖÐ¶ÏÇëÇó
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //ÉÏÉýÑØ´¥·¢
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	// ³õÊ¼»¯Íâ²¿ÖÐ¶Ï

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //Ê¹ÄÜ°´¼üËùÔÚµÄÍâ²¿ÖÐ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //ÏÈÕ¼ÓÅÏÈ¼¶2¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //´ÓÓÅÏÈ¼¶2¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //Ê¹ÄÜÍâ²¿ÖÐ¶ÏÍ¨µÀ
	NVIC_Init(&NVIC_InitStructure); //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷

}


void send_temp_humi()
{
	   char *t;
		 DHT11_Read_Data(&temp,&humi);	
		 t=convert_temp_humi(temp,humi,temp_str,humi_str);
	   sendmes_to_server(t);
	   myfree(t);
	   
}
//Ö÷º¯Êý
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
	  	   
 
	
		mem_init();			//³õÊ¼»¯ÄÚ²¿ÄÚ´æ³Ø	
 	  exfuns_init();		//ÎªfatfsÏà¹Ø±äÁ¿ÉêÇëÄÚ´æ  
    f_mount(fs[0],"0:",1);	//¹ÒÔØSD¿¨
	  VS_Init();
	  
    Init_HX711pin();
    Get_Maopi();	
    delay_ms(1000);
  	delay_ms(1000);	 //³ÆÃ«Æ¤ÖØÁ¿
	  Get_Maopi();

     motor_pin_init(); 		 
		 DHT11_Init();
		 
    while(temp==0|humi==0)  //Ä£¿é¸Õ¸Õ³õÊ¼»¯¶ÁÊýÎª0
		 {
		 DHT11_Read_Data(&temp,&humi);
		 }	
		 LCD_Clear(WHITE);
		 mp3_play_begin();
		 WKUP_Init();
	OSInit(&err);		    	//³õÊ¼»¯UCOSIII
	OS_CRITICAL_ENTER();	//½øÈëÁÙ½çÇø			 
	//´´½¨¿ªÊ¼ÈÎÎñ
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//ÈÎÎñ¿ØÖÆ¿é
				 (CPU_CHAR	* )"start task", 		//ÈÎÎñÃû×Ö
                 (OS_TASK_PTR )start_task, 			//ÈÎÎñº¯Êý
                 (void		* )0,					//´«µÝ¸øÈÎÎñº¯ÊýµÄ²ÎÊý
                 (OS_PRIO	  )START_TASK_PRIO,     //ÈÎÎñÓÅÏÈ¼¶
                 (CPU_STK   * )&START_TASK_STK[0],	//ÈÎÎñ¶ÑÕ»»ùµØÖ·
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//ÈÎÎñ¶ÑÕ»Éî¶ÈÏÞÎ»
                 (CPU_STK_SIZE)START_STK_SIZE,		//ÈÎÎñ¶ÑÕ»´óÐ¡
                 (OS_MSG_QTY  )0,					//ÈÎÎñÄÚ²¿ÏûÏ¢¶ÓÁÐÄÜ¹»½ÓÊÕµÄ×î´óÏûÏ¢ÊýÄ¿,Îª0Ê±½ûÖ¹½ÓÊÕÏûÏ¢
                 (OS_TICK	  )0,					//µ±Ê¹ÄÜÊ±¼äÆ¬ÂÖ×ªÊ±µÄÊ±¼äÆ¬³¤¶È£¬Îª0Ê±ÎªÄ¬ÈÏ³¤¶È£¬
                 (void   	* )0,					//ÓÃ»§²¹³äµÄ´æ´¢Çø
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //ÈÎÎñÑ¡Ïî
                 (OS_ERR 	* )&err);				//´æ·Å¸Ãº¯Êý´íÎóÊ±µÄ·µ»ØÖµ
	OS_CRITICAL_EXIT();	//ÍË³öÁÙ½çÇø	 
	OSStart(&err);      //¿ªÆôUCOSIII
}


//¿ªÊ¼ÈÎÎñº¯Êý
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//Í³¼ÆÈÎÎñ                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//Èç¹ûÊ¹ÄÜÁË²âÁ¿ÖÐ¶Ï¹Ø±ÕÊ±¼ä
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //µ±Ê¹ÓÃÊ±¼äÆ¬ÂÖ×ªµÄÊ±ºò
	 //Ê¹ÄÜÊ±¼äÆ¬ÂÖ×ªµ÷¶È¹¦ÄÜ,Ê±¼äÆ¬³¤¶ÈÎª1¸öÏµÍ³Ê±ÖÓ½ÚÅÄ£¬¼È1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif	
	
	
	OS_CRITICAL_ENTER();	//½øÈëÁÙ½çÇø
								
	//´´½¨³öÃ×ÈÎÎñ
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

		
  //´´½¨ÏÔÊ¾Í¼Æ¬ÈÎÎñ
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
								 
	OS_CRITICAL_EXIT();	//ÍË³öÁÙ½çÇø
	OSTaskDel((OS_TCB*)0,&err);	//É¾³ýstart_taskÈÎÎñ×ÔÉí
}


//³öÃ×ÈÎÎñº¯Êý
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





