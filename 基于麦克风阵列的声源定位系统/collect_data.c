#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include "collect_data.h"
#define FALSE 0
#define TRUE 1
extern float timeDiff[3];
extern pthread_barrier_t a_t;
int fd1,fd2;
int speed_arr[] = { B38400, B19200,B115200, B9600, B4800, B2400, B1200, B300,
	B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {38400,  19200,  115200,9600,  4800,  2400,  1200,  300,
	38400,  19200,  9600, 4800, 2400, 1200,  300, };
char buff_temp1[20];
char buff_temp2[20];
int k1,k2,k3,k4;
k1=0;
k2=0;
k3=0;
k4=0;
int opendev(const char *s)
{
	int fd;
	fd=open(s,O_RDWR);
	if(fd==-1)
	{
		perror("can not open usb");
		return -1;
	}
	else
		return fd;
}


void set_speed(int fd, int speed)   //Linux 下串口USB等设备通信编程入门2中有终端属性的获取设置函数
{
	int   i;
	int   status;
	struct termios   Opt;
	tcgetattr(fd, &Opt);
	for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
	{
		if  (speed == name_arr[i])
		{
			tcflush(fd, TCIOFLUSH);//Update the options and do it NOW
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);
			status = tcsetattr(fd, TCSANOW, &Opt);
			if  (status != 0)
				perror("tcsetattr fd1");
			return;
		}
		tcflush(fd,TCIOFLUSH);
	}
}

int set_Parity(int fd,int databits,int stopbits,int parity)
{
	struct termios options;
	if  ( tcgetattr( fd,&options)  !=  0)
	{
		perror("SetupSerial 1");
		return(FALSE);
	}
	options.c_cflag &= ~CSIZE;
	switch (databits) /*设置数据位数*/
	{
		case 7:
			options.c_cflag |= CS7;
			break;
		case 8:
			options.c_cflag |= CS8;
			break;
		default:
			fprintf(stderr,"Unsupported data size\n");
			return (FALSE);
	}
	switch (parity)
	{
		case 'n':
		case 'N':
			options.c_cflag &= ~PARENB;   /* Clear parity enable */
			options.c_iflag &= ~INPCK;     /* Enable parity checking */
			break;
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB);  /* 设置为奇效验*/ 
			options.c_iflag |= INPCK;             /* Disnable parity checking */
			break;
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;     /* Enable parity */
			options.c_cflag &= ~PARODD;   /* 转换为偶效验*/  
			options.c_iflag |= INPCK;       /* Disnable parity checking */
			break;
		case 'S':
		case 's':  /*as no parity*/
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
		default:
			fprintf(stderr,"Unsupported parity\n");
			return (FALSE);
	}
	/* 设置停止位*/   
	switch (stopbits)
	{
		case 1:
			options.c_cflag &= ~CSTOPB;
			break;
		case 2:
			options.c_cflag |= CSTOPB;
			break;
		default:
			fprintf(stderr,"Unsupported stop bits\n");
			return (FALSE);
	}
	/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;
	options.c_cc[VTIME] = 150; // 15 seconds
	options.c_cc[VMIN] = 0;

	tcflush(fd,TCIFLUSH); /* Update the options and do it NOW */
	if (tcsetattr(fd,TCSANOW,&options) != 0)
	{
		perror("SetupSerial 3");
		return (FALSE);
	}
	return (TRUE);
}
int link_to_usb(char *s)
{
	int fd;
	fd=opendev(s);
	if(fd>0)
		set_speed(fd,115200);
	else
	{
		printf("can not open serial port\n");
		exit(0);
	}
	if(set_Parity(fd,8,1,'N')==0)
	{
		printf("set PARITY error\n");
		exit(1);
	}
	return fd;

} 
void data_process_1_ab(void *arg)
{ 
    	int nread,j,k;
			j=0;
                        while(j<data_size)
                       {
			       
			       k=0;
                          nread=read(fd1,buff_temp1,20);
                          while(k<nread)
                         {
                            switch(buff_temp1[k])
                            {
                                case 'a':
                                       {
                                        buff_temp1[k]='\0';
                                        micro_data_1[k1++]=atof(buff_temp1);
                                        break;
                                        }
                                case 'b':
                                        {
                                        buff_temp1[k]='\0';
                                        micro_data_2[k2++]=atof(buff_temp1)/10;
                                       // printf("*:%f\n",micro_data_2[k2-1]);
					break;

                                        }
                                case 'c':
                                        {
                                          buff_temp1[k]='\0';
                                          micro_data_3[k3++]=atof(buff_temp1);
                                          break;
                                         }
                                case 'd':
                                        {
                                        buff_temp1[k]='\0';
                                        micro_data_4[k4++]=atof(buff_temp1);
                                        break;
                                         }
                                default:k++;
                                        break;
                            }
                         }
	            memset(buff_temp1,0,sizeof(buff_temp1));
		                 j++;
		              k=0;
                    }
						pthread_exit((void *)1);
		
	
}
void  data_process_1_cd(void *arg)
{  
	 int nread,j,k;
			j=0;
                        while(j<data_size)
                       {
			               k=0;
                          nread=read(fd2,buff_temp2,20);
                          while(k<nread)
                         {
                            switch(buff_temp2[k])
                            {
                                case 'a':
                                       {
                                        buff_temp2[k]='\0';
                                        micro_data_1[k1++]=atof(buff_temp2);
                                        break;
                                        }
                                case 'b':
                                        {
                                        buff_temp2[k]='\0';
                                        micro_data_2[k2++]=atof(buff_temp2)/10;
                                        //printf("*:%f\n",micro_data_2[k2-1]);
					break;
                                        }
                                case 'c':
                                        {
                                          buff_temp2[k]='\0';
                                          micro_data_3[k3++]=atof(buff_temp2);
                                          break;
                                         }
                                case 'd':
                                        {
                                        buff_temp2[k]='\0';
                                        micro_data_4[k4++]=atof(buff_temp2);
                                        break;
                                         }
                                default:k++;
                                        break;
                            }
			    
                         }
                    memset(buff_temp2,0,sizeof(buff_temp2));
                    j++;
                    k=0;
                    }
        
        pthread_exit((void *)2);
}

int min(int x,int y)
{
   if(x<y)
	   return x;
   else return y;
}
int min_4(int m1,int m2,int m3,int m4)
{
   int min_temp;
   min_temp=min(m1,m2);
   min_temp=min(min_temp,m3);
   min_temp=min(min_temp,m4);
   return min_temp;
}

void data_process()
{
   int i=0;
   int l1,l2,l3,l4;
   l1=0;
   l2=0;
   l3=0;
   l4=0;
   int p;
   int temp;
   int recv_flag=0;
         while((fabs(micro_data_1[i])<d_value)&&(fabs(micro_data_2[i])<d_value)&&(fabs(micro_data_3[i])<d_value)&&(fabs(micro_data_4[i])<d_value)&&(i<min_4(k1,k2,k3,k4)))
         {
            i++;
         }
         if((i<min_4(k1,k2,k3,k4)))
        {
           p=i;
           temp=i;
           if(fabs(micro_data_1[i])>d_value)
           {
             for(;i<k1;i++)
             {
               if(fabs(micro_data_1[i])>d_value)
               {
                  final_data_1[l1++]=micro_data_1[i];
               }
             }
           }
           else {
              while(fabs(micro_data_1[i++])<d_value&&(i<(k1-2)));
              while(fabs(micro_data_1[i++])>d_value&&(i<(k1-1)));
              for(;p<i;p++)
              {
                final_data_1[l1++]=micro_data_1[p];
              }
           }
           i=temp;
           p=temp;
           if(fabs(micro_data_2[i])>d_value)
           {
             for(;i<k2;i++)
             {
               if(fabs(micro_data_2[i])>d_value)
               {
                  final_data_2[l2++]=micro_data_2[i];
               }
             }
           }
           else {
              while(fabs(micro_data_2[i++])<d_value&&(i<(k2-2)));
              while(fabs(micro_data_2[i++])>d_value&&(i<(k2-1)));
              for(;p<i;p++)
              {
                final_data_2[l2++]=micro_data_2[p];
              }
           }
           i=temp;
           p=temp;

           if(fabs(micro_data_3[i])>d_value)
           {
             for(;i<k3;i++)
             {
               if(fabs(micro_data_3[i])>d_value)
               {
                  final_data_3[l3++]=micro_data_3[i];
               }
             }
           }
           else {
              while(fabs(micro_data_3[i++])<d_value&&(i<(k3-2)));
              while(fabs(micro_data_3[i++])>d_value&&(i<(k3-1)));
              for(;p<i;p++)
              {
                final_data_3[l3++]=micro_data_3[p];
              }
           }
           i=temp;
           p=temp;

           if(fabs(micro_data_4[i])>d_value)
           {
             for(;i<k4;i++)
             {
               if(fabs(micro_data_4[i])>d_value)
               {
                  final_data_4[l4++]=micro_data_4[i];
               }
             }
           }
       else {
              while(fabs(micro_data_4[i++])<d_value&&(i<(k4-2)));
              while(fabs(micro_data_4[i++])>d_value&&(i<(k4-1)));
              for(;p<i;p++)
              {
                final_data_4[l4++]=micro_data_4[p];
              }
           }
           i=0;
           temp=0;
           p=0;

        }
	 if(min_4(l1,l2,l3,l4)>10)
	 {
	    recv_flag=1;
	 }
        memset(micro_data_1,0,sizeof(micro_data_1));
        memset(micro_data_2,0,sizeof(micro_data_2));
        memset(micro_data_3,0,sizeof(micro_data_3));
        memset(micro_data_4,0,sizeof(micro_data_4));
        k1=0;
        k2=0;
        k3=0;
        k4=0;
        if(recv_flag==1)
        {
        for(int i=0;i<l1;i++)
                printf("data1:%f\n",final_data_1[i]);
        for(int i=0;i<l2;i++)
                printf("data2:%f\n",final_data_2[i]);
         for(int i=0;i<l3;i++)
                printf("data3:%f\n",final_data_3[i]);
        for(int i=0;i<l4;i++)
                printf("data4:%f\n",final_data_4[i]);
       
        print_position(final_data_1,final_data_2,final_data_3,final_data_4);
        memset(final_data_1,0,sizeof(final_data_1));
        memset(final_data_2,0,sizeof(final_data_2));
        memset(final_data_3,0,sizeof(final_data_3));
        memset(final_data_4,0,sizeof(final_data_4));
		memset(timeDiff,0,sizeof(timeDiff));
        }
       
}





























