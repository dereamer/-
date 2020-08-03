#ifndef __COMMUNI_H
#define __COMMUNI_H

#define COMMUNITE_ERROR       -1 
#define NO_SIM_CARD_ERROR     -2
#define SIM_CARD_NO_REG_ERROR -3
#define CALL_ERROR			  -4
#define AT_CPMS_ERROR		  -5
#define AT_CMGF_ERROR		  -6
#define AT_CSCS_ERROR		  -7
#define AT_CMGS_ERROR         -8
#define END_CHAR_ERROR		  -9
#define AT_CGATT_ERROR   	  -11
#define AT_CGACT_ERROR	      -12
#define AT_CIPSTART_ERROR	  -13
#define AT_CIPSEND_ERROR	  -14
#define AT_CIPMUX_ERROR       -15
#define AT_CIPQSEND_ERROR     -16
#define AT_CSTT_ERROR		  -17
#define AT_CIICR_ERROR		  -18
#define AT_CGATT1_ERROR       -50
#define EAT0_ERROR            -51
#define Buf2_Max  200
#define USART1_REC_MAXLEN  200
#define STABLE_TIMES  10  //等待系统上电后的稳定

#include "sys.h"

static char Uart2_Buf[Buf2_Max];//串口2接收缓存
extern struct rice rice_info;
static u8 First_Int = 0;
//static u8 server_data_flag=0;
/*struct rice{
	char type;
	int weight;    
};
*/



/*************	本地函数声明	**************/
void System_RESET(void);//软件复位函数
static int receive_flag=0;
void find_substring(char str[],char substr[]);
void ser_data_process(char temp_rec[50]);
void CLR_Buf(void);     //清除串口2接收缓存
void judge_ser_data(void);
u8 Wait_CREG(u8 query_times);    //等待模块注册成功
u8 Find(char *a);
int check_server(char *mes);
int check_status(void);
int link_to_server(void);
int sendmes_to_server(char *mes);  //发送数据
void UART1_SendString(char* s);
void uart2_init(u32 bound);
void UART2_SendString(char* s);
void UART1_SendLR(void);
int respone_server(char *mes);
void UART2_SendLR(void);
void UART2_Send_Command(char* s);
u8 UART2_Send_AT_Command(char *b,char *a,u8 wait_time,u32 interval_time);
void UART2_Send_Command_END(char* s); 
u8 UART2_Send_AT_Command_End(char *b,char *a,u8 wait_time,u32 interval_time);
int chumi(struct rice rice_info);    //负责出米

//串口1发送回车换行
#define UART1_SendLR() UART1_SendString("\r\n")

//串口2发送回车换行
#define UART2_SendLR() UART2_SendString("\r\n")
#endif

