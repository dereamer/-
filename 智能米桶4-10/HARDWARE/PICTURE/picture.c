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
 	DIR picdir;	 		//图片目录
	FILINFO picfileinfo;//文件信息
	u8 *fn;   			//长文件名
	u8 *pname;			//带路径的文件名
	  	    									       
  piclib_init();		//初始化画图	
	
	f_opendir(&picdir,filename);
	
	f_readdir(&picdir,&picfileinfo); 
	
	picfileinfo.lfsize=_MAX_LFN*2+1;						//长文件名最大长度
	pname=mymalloc(picfileinfo.lfsize);				//为带路径的文件名分配内存
	picfileinfo.lfname=mymalloc(picfileinfo.lfsize);	//为长文件缓存区分配内存
	fn=(u8*)(*picfileinfo.lfname?picfileinfo.lfname:picfileinfo.fname);		
 /*
	strcpy((char*)pname,"0:/PICTURE/");				//复制路径(目录)
	strcat((char*)pname,(const char*)fn);  			//将文件名接在后面
 	pname=mymalloc(picfileinfo.lfsize);				//为带路径的文件名分配内存
	*/
 //	picindextbl=mymalloc(2*totpicnum);				//申请2*totpicnum个字节的内存,用于存放图片索引
	ai_load_picfile(filename,x,y,width,height,1);//显示图片
	
	
	myfree(picfileinfo.lfname);	//释放内存			    
	myfree(pname);				//释放内存			    
	//myfree(picindextbl);		//释放内存		
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