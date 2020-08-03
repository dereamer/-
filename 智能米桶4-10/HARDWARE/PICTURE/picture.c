#include "picture.h"
#include "piclib.h"
#include "tjpgd.h"
#include "string.h"
#include "os.h"
#include <cpu.h>
#include <cpu_core.h>
#include <lib_def.h>
#include <os_type.h>
#include <os_cpu.h>
void show_picture(const u8 *filename,u16 x,u16 y,u16 width,u16 height)
{
 	DIR picdir;	 		//ͼƬĿ¼
	FILINFO picfileinfo;//�ļ���Ϣ
	u8 *fn;   			//���ļ���
	u8 *pname;			//��·�����ļ���
	  	    									       
  piclib_init();		//��ʼ����ͼ	
	
	f_opendir(&picdir,filename);
	
	f_readdir(&picdir,&picfileinfo); 
	
	picfileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
	pname=mymalloc(picfileinfo.lfsize);				//Ϊ��·�����ļ��������ڴ�
	picfileinfo.lfname=mymalloc(picfileinfo.lfsize);	//Ϊ���ļ������������ڴ�
	fn=(u8*)(*picfileinfo.lfname?picfileinfo.lfname:picfileinfo.fname);		
 /*
	strcpy((char*)pname,"0:/PICTURE/");				//����·��(Ŀ¼)
	strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
 	pname=mymalloc(picfileinfo.lfsize);				//Ϊ��·�����ļ��������ڴ�
	*/
 //	picindextbl=mymalloc(2*totpicnum);				//����2*totpicnum���ֽڵ��ڴ�,���ڴ��ͼƬ����
	ai_load_picfile(filename,x,y,width,height,1);//��ʾͼƬ
	
	
	myfree(picfileinfo.lfname);	//�ͷ��ڴ�			    
	myfree(pname);				//�ͷ��ڴ�			    
	//myfree(picindextbl);		//�ͷ��ڴ�		
}
/*void show_bigmap()
{
	LCD_Clear(WHITE);
  show_picture("0:/PICTURE1/xiaohui.jpg",0,0,lcddev.width,lcddev.height);
	delay_ms(9000);
	LCD_Clear(WHITE);
	show_picture("0:/PICTURE2/xiaohui.jpg",0,0,lcddev.width,lcddev.height);
	delay_ms(9000);

}*/