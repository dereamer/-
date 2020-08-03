#include "communication.h"
#include "usart.h"
#include "string.h"
#include "stdlib.h"
#include "chumi.h"
#include "LCD.h"
#include "led.h"
#include "weight.h"
#include "sound.h"
#include "os.h"
//PB11---RX   PB10---TX
struct rice{
  char type;
	long weight;
};
struct rice rice_info;
int chumi_flag=0;
void System_RESET(void)
{
   __set_FAULTMASK(1);//??????
   NVIC_SystemReset();//????
 
}
void UART1_SendString(char* s)
{
  while(*s)//?�2a�?�?'??��?�?
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); 
		USART_SendData(USART1 ,*s++);//�??�?��?�?
	}
}

void uart2_init(u32 bound)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   USART_InitTypeDef USART_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE );
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE );

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //UART5 TX;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //??????;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure); //??C;

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //UART5 RX;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //????;
   GPIO_Init(GPIOD, &GPIO_InitStructure); //??D;

   USART_InitStructure.USART_BaudRate = bound; //???;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b; //???8?;
   USART_InitStructure.USART_StopBits = USART_StopBits_1; //???1?;
   USART_InitStructure.USART_Parity = USART_Parity_No ; //????;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(UART5, &USART_InitStructure);//??????;

   NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn; //???;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //?????;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //?????;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

   USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
   USART_Cmd(UART5, ENABLE); //????;

}


void UART2_SendString(char* s)
{
  while(*s)//?�2a�?�?'??��?�?
	{
		while(USART_GetFlagStatus(UART5, USART_FLAG_TC)==RESET); 
		USART_SendData(UART5,*s++);//�??�?��?�?		
	}
}

void UART2_Send_Command(char* s)
{
  CLR_Buf(); //?????��?�y?Y�?buffer
	UART2_SendString(s); //�?3?�?�?'?
	UART2_SendString("\r\n"); //?��??��??� \r\n�???�?�?
	
}

u8 UART2_Send_AT_Command(char *b,char *a,u8 wait_time,u32 interval_time)////////////////////////////////////////////////////////////////////
{
   u8 i;
	i = 0;
	while(i < wait_time)    //�?1???�D?�? ?�?�D??��?�?'???�? ?�??DD2�?�??���?�?'?                
	{
		UART2_Send_Command(b);//'??�3�??� b �?�?'? ???��??��??�\r\n  ?൱���??��?�?????�?
		delay_ms(interval_time); //��'y�??��?? '?50�??�?��? 50*20ms = 1??
		if(Find(a))            //2�?�D��a�|'e�?�?�?'? a
		{
			return 1;
		}
		i++;
	}
	
	return 0;
}

void UART2_Send_Command_END(char* s)
{
  CLR_Buf(); //?????��?�y?Y�?buffer
	UART2_SendString(s); //�?3?�?�?'?
}

u8 UART2_Send_AT_Command_End(char *b,char *a,u8 wait_time,u32 interval_time)
{
    u8 i;
	i = 0;
	while(i < wait_time)    //�?1???�D?�? ?�?�D??��?�?'???�? ?�??DD2�?�??���?�?'?                
	{
		UART2_Send_Command_END(b);//'??�2�??� b �?�?'? ?a�?2?�??�\r\n 
		delay_ms(interval_time); //��'y�??��?? '?50�??�?��? 50*20ms = 1??
		if(Find(a))            //2�?�D��a�|'e�?�?�?'? a
		{
			//CLR_Buf();
			return 1;
		}
		i++;
	}
	
	return 0;
}

void CLR_Buf(void)
{
   u16 k;
	for(k=0;k<Buf2_Max;k++)      //???o'??��Y??�?
	{
		Uart2_Buf[k] = 0x00;
	}
    First_Int = 0;
}

u8 Wait_CREG(u8 query_times)
{
   u8 i;
	u8 k;
	u8 j;
	i = 0;
	CLR_Buf();
	while(i == 0)        			
	{

		UART2_Send_Command("AT+CREG?");
		delay_ms(100); 
		
		for(k=0;k<Buf2_Max;k++)      			
		{
			if((Uart2_Buf[k] == '+')&&(Uart2_Buf[k+1] == 'C')&&(Uart2_Buf[k+2] == 'R')&&(Uart2_Buf[k+3] == 'E')&&(Uart2_Buf[k+4] == 'G')&&(Uart2_Buf[k+5] == ':'))
			{
					 
				if((Uart2_Buf[k+7] == '1')&&((Uart2_Buf[k+9] == '1')||(Uart2_Buf[k+9] == '5')))
				{
					i = 1;
					return 1;
				}
				
			}
		}
		j++;
		if(j > query_times)
		{
			return 0;
		}
		
	}
	return 0;
}

u8 Find(char *a)
{
   if(strstr(Uart2_Buf, a)!=NULL)
	{
		return 1;
	}	
	else
	{
		return 0;
	}
}

int check_status(void)
{
  int ret;
	char esc_char[2];
	
	esc_char[0] = 0x1B;//�?3?�?�?
	esc_char[1] = '\0';
	
	ret = UART2_Send_AT_Command("AT","OK",5,50);//2a�?�D?�?�?3�1|
	if(ret == 0)
	{
		UART2_SendString(esc_char);//���???�?>�'�??????'??�??��???ESC�?�?
		return COMMUNITE_ERROR;
	}
	
	ret = UART2_Send_AT_Command("AT+CPIN?","READY",2,50);//2�?�?��?�?2?�?
	if(ret == 0)
	{
		return NO_SIM_CARD_ERROR;
	
	}
	
	ret = Wait_CREG(3);//2�?�?��?�?�?2�?�???
	if(ret == 0)
	{
		return SIM_CARD_NO_REG_ERROR;
		
	}
    
    ret = UART2_Send_AT_Command("ATE0","OK",2,50);//1?�?????1|?�
	if(ret == 0)
	{
		return EAT0_ERROR;
	}
    
	return 1;
}

int link_to_server(void)
{	
  u8 ret;
	ret = UART2_Send_AT_Command("ATE0","OK",2,50);//1?�?????1|?�
	if(ret == 0)
	{
      return 0;
	}

	ret = UART2_Send_AT_Command("AT+CGCLASS=\"B\"","OK",5,50*7);//?�??�???  �D?��a
	if(ret == 0)
	{
	    return 0;
	}
	ret = UART2_Send_AT_Command("AT+CGATT=1","OK",5,50*7);//?�??�???  �D?��a
	if(ret == 0)
	{
	    return 0;
	}
	ret = UART2_Send_AT_Command("AT+CIPSTART=\"TCP\",\"207.148.6.178\",\"8100\"","CONNECT OK",5,50*7);//�??�t???�
	if(ret == 0)
	{
		return 0;
		//return AT_CIPSTART_ERROR;
	}
	return 1;
  }

	int respone_server(char *mes)
{
	u8 i=0;
  u8 ret;
  ret = UART2_Send_AT_Command("AT+CIPSTATUS","CONNECT OK",3,50*2);
	if(ret==1)
	{
		ret = UART2_Send_AT_Command("AT+CIPSEND=4",">",3,50);//?a�?�??��y?Y
		UART2_SendString(mes);
		return 1;
	}
	else 
	{
		while(i<10&&link_to_server()!=1)
		{i++;
		 delay_ms(500);
		}
		//link_to_server();
		if(i>=5) return -1;
		else respone_server(mes);	
	}
	return 0;
}

int check_server(char *mes)
{
	u8 i=0;
  u8 ret;
  ret = UART2_Send_AT_Command("AT+CIPSTATUS","CONNECT OK",3,50*2);
	if(ret==1)
	{
		ret = UART2_Send_AT_Command("AT+CIPSEND=2",">",3,50);//?a�?�??��y?Y
		UART2_SendString(mes);
		return 1;
	}
	else 
	{
		while(i<10&&link_to_server()!=1)
		{i++;
		 delay_ms(500);
		}
		//link_to_server();
		if(i>=5) return -1;
		else check_server(mes);	
	}
	return 0;
}

int sendmes_to_server(char *mes)
{
	u8 i=0;
  u8 ret;
  ret = UART2_Send_AT_Command("AT+CIPSTATUS","CONNECT OK",3,50*2);
	//ret=1;
	if(ret==1)
	{
		ret = UART2_Send_AT_Command("AT+CIPSEND=16",">",3,50);//?a�?�??��y?Y
		UART2_SendString(mes);
		return 1;
	}
	else 
	{
		while(i<10&&link_to_server()!=1)
		{i++;
		 delay_ms(500);
		}
		//link_to_server();
		if(i>=5) return -1;
		else sendmes_to_server(mes);	
	}
	return 0;
}
void judge_ser_data(void)
{
	  if(Find("sd")&&chumi_flag==1)
		 {   
			 char *t1;
			 u8 i=0,j=0;
			 char wei[5];
			 t1=strstr(Uart2_Buf,"sd");
			 rice_info.type=t1[2];
       while(t1[i++]!=',');
			 while(t1[i]!='@')
		 {
		    wei[j++]=t1[i++];
		 }
		    wei[j]='\0';
		    rice_info.weight=atoi(wei);
        CLR_Buf();
		    LCD_Clear(WHITE);
	      LCD_ShowString(30,40,200,24,24,"Please wait...");
	  if(rice_info.type=='1')         //3?1o??�
		{  int n=30;
			 int i=100;			 
          while(Get_Weight()<rice_info.weight)
					{

						while(rice_info.weight-Get_Weight()>50)
						  {
						  	motorNcircle(n,1,1000,SetMotor_1);
							}
						 n=n-1;
						 i=i+10;
						 motorNcircle(n,1,1000+i,SetMotor_1);		
						
					}						
		}
	if(rice_info.type=='2')         //3?1o??�
		{  int n=30;
			 int i=100;			
          while(Get_Weight()<rice_info.weight)
					{
						while(rice_info.weight-Get_Weight()>50)
						  {
							motorNcircle(n,1,1000,SetMotor_2);
							}
						 n=n-1;
						 i=i+10;
						 motorNcircle(n,1,1000+i,SetMotor_2);						
					}						
		}
		if(rice_info.type=='3')         //3?1o??�
		{  int n=30;
			 int i=100;			 
          while(Get_Weight()<rice_info.weight)
					{

						while(rice_info.weight-Get_Weight()>50)
						  {
							motorNcircle(n,1,1000,SetMotor_3);
	
							}
						 n=n-1;
						 i=i+10;
						 motorNcircle(n,1,1000+i,SetMotor_3);						
					}						
		}
		LCD_Clear(WHITE);
	  LCD_ShowString(30,40,200,24,24,"Finished");
    mp3_play_end();
		}
		 else{}
}

void UART5_IRQHandler(void)
{
	u8 Res=0;
#ifdef SYSTEM_SUPPORT_OS	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART5,USART_IT_RXNE) != RESET) 
  {
	
	  Res = USART_ReceiveData(UART5);
		if(Res==0x40)  chumi_flag=1;
	  Uart2_Buf[First_Int] = Res;  	  //???��?�?�?�?�?'?'?�??o'??D
	  First_Int++;                	  //?o'??????�o��??�
	  if(First_Int > Buf2_Max)       	  //�?1??o'??�,???o'????????�?o'?�?�׵??�
	  {
		First_Int = 0;
	  }
		if(Uart2_Buf[First_Int]=='@') chumi_flag=1;

  }	
#ifdef SYSTEM_SUPPORT_OS	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
}

