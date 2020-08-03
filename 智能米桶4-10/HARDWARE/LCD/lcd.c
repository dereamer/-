#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"
#include "delay.h"	 
#include "weight.h"
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK MiniSTM32F103¿ª·¢°å
//2.4´ç/2.8´ç/3.5´ç/4.3´ç/7´ç TFTÒº¾§Çı¶¯	  
//Ö§³ÖÇı¶¯ICĞÍºÅ°üÀ¨:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/
//                  SPFD5408/1505/B505/C505/NT35310/NT35510/SSD1963µÈ		    
//ÕıµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//´´½¨ÈÕÆÚ:2010/7/4
//°æ±¾£ºV3.0
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾ 2014-2024
//All rights reserved	
//********************************************************************************
//V1.2ĞŞ¸ÄËµÃ÷
//Ö§³ÖÁËSPFD5408µÄÇı¶¯,ÁíÍâ°ÑÒº¾§IDÖ±½Ó´òÓ¡³ÉHEX¸ñÊ½.·½±ã²é¿´LCDÇı¶¯IC.
//V1.3
//¼ÓÈëÁË¿ìËÙIOµÄÖ§³Ö
//ĞŞ¸ÄÁË±³¹â¿ØÖÆµÄ¼«ĞÔ£¨ÊÊÓÃÓÚV1.8¼°ÒÔºóµÄ¿ª·¢°å°æ±¾£©
//¶ÔÓÚ1.8°æ±¾Ö®Ç°(²»°üÀ¨1.8)µÄÒº¾§Ä£¿é,ÇëĞŞ¸ÄLCD_Initº¯ÊıµÄLCD_LED=1;ÎªLCD_LED=1;
//V1.4
//ĞŞ¸ÄÁËLCD_ShowCharº¯Êı£¬Ê¹ÓÃ»­µã¹¦ÄÜ»­×Ö·û¡£
//¼ÓÈëÁËºáÊúÆÁÏÔÊ¾µÄÖ§³Ö
//V1.5 20110730
//1,ĞŞ¸ÄÁËB505Òº¾§¶ÁÑÕÉ«ÓĞÎóµÄbug.
//2,ĞŞ¸ÄÁË¿ìËÙIO¼°ºáÊúÆÁµÄÉèÖÃ·½Ê½.
//V1.6 20111116
//1,¼ÓÈë¶ÔLGDP4535Òº¾§µÄÇı¶¯Ö§³Ö
//V1.7 20120713
//1,Ôö¼ÓLCD_RD_DATAº¯Êı
//2,Ôö¼Ó¶ÔILI9341µÄÖ§³Ö
//3,Ôö¼ÓILI9325µÄ¶ÀÁ¢Çı¶¯´úÂë
//4,Ôö¼ÓLCD_Scan_Dirº¯Êı(É÷ÖØÊ¹ÓÃ)	  
//6,ÁíÍâĞŞ¸ÄÁË²¿·ÖÔ­À´µÄº¯Êı,ÒÔÊÊÓ¦9341µÄ²Ù×÷
//V1.8 20120905
//1,¼ÓÈëLCDÖØÒª²ÎÊıÉèÖÃ½á¹¹Ìålcddev
//2,¼ÓÈëLCD_Display_Dirº¯Êı,Ö§³ÖÔÚÏßºáÊúÆÁÇĞ»»
//V1.9 20120911
//1,ĞÂÔöRM68042Çı¶¯£¨ID:6804£©£¬µ«ÊÇ6804²»Ö§³ÖºáÆÁÏÔÊ¾£¡£¡Ô­Òò£º¸Ä±äÉ¨Ãè·½Ê½£¬
//µ¼ÖÂ6804×ø±êÉèÖÃÊ§Ğ§£¬ÊÔ¹ıºÜ¶à·½·¨¶¼²»ĞĞ£¬ÔİÊ±ÎŞ½â¡£
//V2.0 20120924
//ÔÚ²»Ó²¼ş¸´Î»µÄÇé¿öÏÂ,ILI9341µÄID¶ÁÈ¡»á±»Îó¶Á³É9300,ĞŞ¸ÄLCD_Init,½«ÎŞ·¨Ê¶±ğ
//µÄÇé¿ö£¨¶Áµ½IDÎª9300/·Ç·¨ID£©,Ç¿ÖÆÖ¸¶¨Çı¶¯ICÎªILI9341£¬Ö´ĞĞ9341µÄ³õÊ¼»¯¡£
//V2.1 20120930
//ĞŞÕıILI9325¶ÁÑÕÉ«µÄbug¡£
//V2.2 20121007
//ĞŞÕıLCD_Scan_DirµÄbug¡£
//V2.3 20130120
//ĞÂÔö6804Ö§³ÖºáÆÁÏÔÊ¾
//V2.4 20131120
//1,ĞÂÔöNT35310£¨ID:5310£©Çı¶¯Æ÷µÄÖ§³Ö
//2,ĞÂÔöLCD_Set_Windowº¯Êı,ÓÃÓÚÉèÖÃ´°¿Ú,¶Ô¿ìËÙÌî³ä,±È½ÏÓĞÓÃ,µ«ÊÇ¸Ãº¯ÊıÔÚºáÆÁÊ±,²»Ö§³Ö6804.
//V2.5 20140211
//1,ĞÂÔöNT35510£¨ID:5510£©Çı¶¯Æ÷µÄÖ§³Ö
//V2.6 20140504
//1,ĞÂÔöASCII 24*24×ÖÌåµÄÖ§³Ö(¸ü¶à×ÖÌåÓÃ»§¿ÉÒÔ×ÔĞĞÌí¼Ó)  
//2,ĞŞ¸Ä²¿·Öº¯Êı²ÎÊı,ÒÔÖ§³ÖMDK -O2ÓÅ»¯
//3,Õë¶Ô9341/35310/35510,Ğ´Ê±¼äÉèÖÃÎª×î¿ì,¾¡¿ÉÄÜµÄÌá¸ßËÙ¶È
//4,È¥µôÁËSSD1289µÄÖ§³Ö,ÒòÎª1289ÊµÔÚÊÇÌ«ÂıÁË,¶ÁÖÜÆÚÒª1us...¼òÖ±Ææİâ.²»ÊÊºÏF4Ê¹ÓÃ
//5,ĞŞÕı68042¼°C505µÈICµÄ¶ÁÑÕÉ«º¯ÊıµÄbug.
//V2.7 20140710
//1,ĞŞÕıLCD_Color_Fillº¯ÊıµÄÒ»¸öbug. 
//2,ĞŞÕıLCD_Scan_Dirº¯ÊıµÄÒ»¸öbug.
//V2.8 20140721
//1,½â¾öMDKÊ¹ÓÃ-O2ÓÅ»¯Ê±LCD_ReadPointº¯Êı¶ÁµãÊ§Ğ§µÄÎÊÌâ.
//2,ĞŞÕıLCD_Scan_DirºáÆÁÊ±ÉèÖÃµÄÉ¨Ãè·½Ê½ÏÔÊ¾²»È«µÄbug.
//V2.9 20141130
//1,ĞÂÔö¶ÔSSD1963 LCDµÄÖ§³Ö.
//2,ĞÂÔöLCD_SSD_BackLightSetº¯Êı
//3,È¡ÏûILI93XXµÄRxx¼Ä´æÆ÷¶¨Òå
//V3.0 20150423
//ĞŞ¸ÄSSD1963 LCDÆÁµÄÇı¶¯²ÎÊı.
//////////////////////////////////////////////////////////////////////////////////	 

 
//LCDµÄ»­±ÊÑÕÉ«ºÍ±³¾°É«	   
u16 POINT_COLOR=0x0000;	//»­±ÊÑÕÉ«
u16 BACK_COLOR=0xFFFF;  //±³¾°É« 

//¹ÜÀíLCDÖØÒª²ÎÊı
//Ä¬ÈÏÎªÊúÆÁ
_lcd_dev lcddev;
	 					    
//Ğ´¼Ä´æÆ÷º¯Êı
//data:¼Ä´æÆ÷Öµ
void LCD_WR_REG(u16 data)
{ 
	LCD_RS_CLR;//Ğ´µØÖ·  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}
//Ğ´Êı¾İº¯Êı
//¿ÉÒÔÌæ´úLCD_WR_DATAXºê,ÄÃÊ±¼ä»»¿Õ¼ä.
//data:¼Ä´æÆ÷Öµ
void LCD_WR_DATAX(u16 data)
{
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}
//¶ÁLCDÊı¾İ
//·µ»ØÖµ:¶Áµ½µÄÖµ
u16 LCD_RD_DATA(void)
{										   
	u16 t;
 	GPIOB->CRL=0X88888888; //PB0-7  ÉÏÀ­ÊäÈë
	GPIOB->CRH=0X88888888; //PB8-15 ÉÏÀ­ÊäÈë
	GPIOB->ODR=0X0000;     //È«²¿Êä³ö0

	LCD_RS_SET;
	LCD_CS_CLR;
	//¶ÁÈ¡Êı¾İ(¶Á¼Ä´æÆ÷Ê±,²¢²»ĞèÒª¶Á2´Î)
	LCD_RD_CLR;
	if(lcddev.id==0X8989)delay_us(2);//FOR 8989,ÑÓÊ±2us					   
	t=DATAIN;  
	LCD_RD_SET;
	LCD_CS_SET; 

	GPIOB->CRL=0X33333333; //PB0-7  ÉÏÀ­Êä³ö
	GPIOB->CRH=0X33333333; //PB8-15 ÉÏÀ­Êä³ö
	GPIOB->ODR=0XFFFF;    //È«²¿Êä³ö¸ß
	return t;  
}
//Ğ´¼Ä´æÆ÷
//LCD_Reg:¼Ä´æÆ÷±àºÅ
//LCD_RegValue:ÒªĞ´ÈëµÄÖµ
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}   
//¶Á¼Ä´æÆ÷
//LCD_Reg:¼Ä´æÆ÷±àºÅ
//·µ»ØÖµ:¶Áµ½µÄÖµ
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
 	LCD_WR_REG(LCD_Reg);  //Ğ´ÈëÒª¶ÁµÄ¼Ä´æÆ÷ºÅ  
	return LCD_RD_DATA(); 
} 
//¿ªÊ¼Ğ´GRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
} 
//LCDĞ´GRAM
//RGB_Code:ÑÕÉ«Öµ
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);//Ğ´Ê®ÁùÎ»GRAM
}
//´ÓILI93xx¶Á³öµÄÊı¾İÎªGBR¸ñÊ½£¬¶øÎÒÃÇĞ´ÈëµÄÊ±ºòÎªRGB¸ñÊ½¡£
//Í¨¹ı¸Ãº¯Êı×ª»»
//c:GBR¸ñÊ½µÄÑÕÉ«Öµ
//·µ»ØÖµ£ºRGB¸ñÊ½µÄÑÕÉ«Öµ
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}	
//µ±mdk -O1Ê±¼äÓÅ»¯Ê±ĞèÒªÉèÖÃ
//ÑÓÊ±i
void opt_delay(u8 i)
{
	while(i--);
}
//¶ÁÈ¡¸öÄ³µãµÄÑÕÉ«Öµ	 
//x,y:×ø±ê
//·µ»ØÖµ:´ËµãµÄÑÕÉ«
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r,g,b;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//³¬¹ıÁË·¶Î§,Ö±½Ó·µ»Ø		   
	LCD_SetCursor(x,y);
	LCD_WR_REG(0X2E);//9341/6804/3510/1963 ·¢ËÍ¶ÁGRAMÖ¸Áî
	      		 		
	GPIOB->CRL=0X88888888; 							//PB0-7  ÉÏÀ­ÊäÈë
	GPIOB->CRH=0X88888888; 							//PB8-15 ÉÏÀ­ÊäÈë
	GPIOB->ODR=0XFFFF;     							//È«²¿Êä³ö¸ß

	LCD_RS_SET;
	LCD_CS_CLR;	    
	//¶ÁÈ¡Êı¾İ(¶ÁGRAMÊ±,µÚÒ»´ÎÎª¼Ù¶Á)	
	LCD_RD_CLR;		   
	opt_delay(2);									//ÑÓÊ±		
 	r=DATAIN;  										//Êµ¼Ê×ø±êÑÕÉ« 
	LCD_RD_SET;
 	//dummy READ
	LCD_RD_CLR;					   
	opt_delay(2);//ÑÓÊ±					   
 	r=DATAIN;  	//Êµ¼Ê×ø±êÑÕÉ«
	LCD_RD_SET;
 	if(lcddev.id==0X9341)	//9341/NT35310/NT35510Òª·Ö2´Î¶Á³ö
	{	 
		LCD_RD_CLR;					   
		opt_delay(2);//ÑÓÊ±			   
		b=DATAIN;//¶ÁÈ¡À¶É«Öµ  	  
	 	LCD_RD_SET;
		g=r&0XFF;//¶ÔÓÚ9341,µÚÒ»´Î¶ÁÈ¡µÄÊÇRGµÄÖµ,RÔÚÇ°,GÔÚºó,¸÷Õ¼8Î»
		g<<=8;
	} 
	LCD_CS_SET;
	GPIOB->CRL=0X33333333; 		//PB0-7  ÉÏÀ­Êä³ö
	GPIOB->CRH=0X33333333; 		//PB8-15 ÉÏÀ­Êä³ö
	GPIOB->ODR=0XFFFF;    		//È«²¿Êä³ö¸ß  

	 if(lcddev.id==0X9341)return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/NT35510ĞèÒª¹«Ê½×ª»»Ò»ÏÂ
	 return 0;
}		 
//LCD¿ªÆôÏÔÊ¾
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	//¿ªÆôÏÔÊ¾
					 
}	 
//LCD¹Ø±ÕÏÔÊ¾
void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0X28);	//¹Ø±ÕÏÔÊ¾
}   
//ÉèÖÃ¹â±êÎ»ÖÃ
//Xpos:ºá×ø±ê
//Ypos:×İ×ø±ê
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
 			    
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 		
	
} 		 
//ÉèÖÃLCDµÄ×Ô¶¯É¨Ãè·½Ïò
//×¢Òâ:ÆäËûº¯Êı¿ÉÄÜ»áÊÜµ½´Ëº¯ÊıÉèÖÃµÄÓ°Ïì(ÓÈÆäÊÇ9341/6804ÕâÁ½¸öÆæİâ),
//ËùÒÔ,Ò»°ãÉèÖÃÎªL2R_U2D¼´¿É,Èç¹ûÉèÖÃÎªÆäËûÉ¨Ãè·½Ê½,¿ÉÄÜµ¼ÖÂÏÔÊ¾²»Õı³£.
//dir:0~7,´ú±í8¸ö·½Ïò(¾ßÌå¶¨Òå¼ûlcd.h)
//9320/9325/9328/4531/4535/1505/b505/5408/9341/5310/5510/1963µÈICÒÑ¾­Êµ¼Ê²âÊÔ	   	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
	if((lcddev.dir==1&&lcddev.id!=0X6804&&lcddev.id!=0X1963)||(lcddev.dir==0&&lcddev.id==0X1963))//ºáÆÁÊ±£¬¶Ô6804ºÍ1963²»¸Ä±äÉ¨Ãè·½Ïò£¡ÊúÆÁÊ±1963¸Ä±ä·½Ïò
	{			   
		switch(dir)//·½Ïò×ª»»
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	} 
	if(lcddev.id==0x9341)//9341/6804/5310/5510/1963,ÌØÊâ´¦Àí
	{
		switch(dir)
		{
			case L2R_U2D://´Ó×óµ½ÓÒ,´ÓÉÏµ½ÏÂ
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://´Ó×óµ½ÓÒ,´ÓÏÂµ½ÉÏ
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://´ÓÓÒµ½×ó,´ÓÉÏµ½ÏÂ
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://´ÓÓÒµ½×ó,´ÓÏÂµ½ÉÏ
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://´ÓÉÏµ½ÏÂ,´Ó×óµ½ÓÒ
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://´ÓÉÏµ½ÏÂ,´ÓÓÒµ½×ó
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://´ÓÏÂµ½ÉÏ,´Ó×óµ½ÓÒ
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://´ÓÏÂµ½ÉÏ,´ÓÓÒµ½×ó
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcddev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510)&&(lcddev.id!=0X1963))regval|=0X08;//5310/5510/1963²»ĞèÒªBGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804µÄBIT6ºÍ9341µÄ·´ÁË	   
		LCD_WriteReg(dirreg,regval);
		if(lcddev.id!=0X1963)//1963²»×ö×ø±ê´¦Àí
		{
			if(regval&0X20)
			{
				if(lcddev.width<lcddev.height)//½»»»X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}else  
			{
				if(lcddev.width>lcddev.height)//½»»»X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}  
		}
		if(lcddev.id==0X5510)
		{
			LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA((lcddev.width-1)>>8); 
			LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA((lcddev.width-1)&0XFF); 
			LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA((lcddev.height-1)>>8); 
			LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA((lcddev.height-1)&0XFF);
		}else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
			LCD_WR_REG(lcddev.setycmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
		}
  	}else 
	{
		switch(dir)
		{
			case L2R_U2D://´Ó×óµ½ÓÒ,´ÓÉÏµ½ÏÂ
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U://´Ó×óµ½ÓÒ,´ÓÏÂµ½ÉÏ
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D://´ÓÓÒµ½×ó,´ÓÉÏµ½ÏÂ
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U://´ÓÓÒµ½×ó,´ÓÏÂµ½ÉÏ
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R://´ÓÉÏµ½ÏÂ,´Ó×óµ½ÓÒ
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L://´ÓÉÏµ½ÏÂ,´ÓÓÒµ½×ó
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R://´ÓÏÂµ½ÉÏ,´Ó×óµ½ÓÒ
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L://´ÓÏÂµ½ÉÏ,´ÓÓÒµ½×ó
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		} 
		dirreg=0X03;
		regval|=1<<12; 
		LCD_WriteReg(dirreg,regval);
	}
}     
//»­µã
//x,y:×ø±ê
//POINT_COLOR:´ËµãµÄÑÕÉ«
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//ÉèÖÃ¹â±êÎ»ÖÃ 
	LCD_WriteRAM_Prepare();	//¿ªÊ¼Ğ´ÈëGRAM
	LCD_WR_DATA(POINT_COLOR); 
}	 
//¿ìËÙ»­µã
//x,y:×ø±ê
//color:ÑÕÉ«
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);  			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		 	 
	
	LCD_RS_CLR;
 	LCD_CS_CLR; 
	DATAOUT(lcddev.wramcmd);//Ğ´Ö¸Áî  
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET; 
	LCD_WR_DATA(color);		//Ğ´Êı¾İ
	}
}
//SSD1963 ±³¹âÉèÖÃ
//pwm:±³¹âµÈ¼¶,0~100.Ô½´óÔ½ÁÁ.
void LCD_SSD_BackLightSet(u8 pwm)
{	
	LCD_WR_REG(0xBE);	//ÅäÖÃPWMÊä³ö
	LCD_WR_DATA(0x05);	//1ÉèÖÃPWMÆµÂÊ
	LCD_WR_DATA(pwm*2.55);//2ÉèÖÃPWMÕ¼¿Õ±È
	LCD_WR_DATA(0x01);	//3ÉèÖÃC
	LCD_WR_DATA(0xFF);	//4ÉèÖÃD
	LCD_WR_DATA(0x00);	//5ÉèÖÃE
	LCD_WR_DATA(0x00);	//6ÉèÖÃF
}
//ÉèÖÃLCDÏÔÊ¾·½Ïò
//dir:0,ÊúÆÁ£»1,ºáÆÁ
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//ÊúÆÁ
	{
		lcddev.dir=0;	//ÊúÆÁ
		lcddev.width=240;
		lcddev.height=320;
		if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
			if(lcddev.id==0X6804||lcddev.id==0X5310)
			{
				lcddev.width=320;
				lcddev.height=480;
			}
		}else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00; 
			lcddev.width=480;
			lcddev.height=800;
		}else if(lcddev.id==0X1963)
		{
			lcddev.wramcmd=0X2C;	//ÉèÖÃĞ´ÈëGRAMµÄÖ¸Áî 
			lcddev.setxcmd=0X2B;	//ÉèÖÃĞ´X×ø±êÖ¸Áî
			lcddev.setycmd=0X2A;	//ÉèÖÃĞ´Y×ø±êÖ¸Áî
			lcddev.width=480;		//ÉèÖÃ¿í¶È480
			lcddev.height=800;		//ÉèÖÃ¸ß¶È800  
		}else
		{
			lcddev.wramcmd=0X22;
	 		lcddev.setxcmd=0X20;
			lcddev.setycmd=0X21;  
		}
	}else 				//ºáÆÁ
	{	  				
		lcddev.dir=1;	//ºáÆÁ
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X9341||lcddev.id==0X5310)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
		}else if(lcddev.id==0X6804)	 
		{
 			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2B;
			lcddev.setycmd=0X2A; 
		}else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00; 
			lcddev.width=800;
			lcddev.height=480;
		}else if(lcddev.id==0X1963)
		{
			lcddev.wramcmd=0X2C;	//ÉèÖÃĞ´ÈëGRAMµÄÖ¸Áî 
			lcddev.setxcmd=0X2A;	//ÉèÖÃĞ´X×ø±êÖ¸Áî
			lcddev.setycmd=0X2B;	//ÉèÖÃĞ´Y×ø±êÖ¸Áî
			lcddev.width=800;		//ÉèÖÃ¿í¶È800
			lcddev.height=480;		//ÉèÖÃ¸ß¶È480  
		}else
		{
			lcddev.wramcmd=0X22;
	 		lcddev.setxcmd=0X21;
			lcddev.setycmd=0X20;  
		}
		if(lcddev.id==0X6804||lcddev.id==0X5310)
		{ 	 
			lcddev.width=480;
			lcddev.height=320; 			
		}
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Ä¬ÈÏÉ¨Ãè·½Ïò
}	 
//ÉèÖÃ´°¿Ú,²¢×Ô¶¯ÉèÖÃ»­µã×ø±êµ½´°¿Ú×óÉÏ½Ç(sx,sy).
//sx,sy:´°¿ÚÆğÊ¼×ø±ê(×óÉÏ½Ç)
//width,height:´°¿Ú¿í¶ÈºÍ¸ß¶È,±ØĞë´óÓÚ0!!
//´°Ìå´óĞ¡:width*height. 
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{    
	u16 twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;
	if(lcddev.id==0X9341)
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA(twidth>>8); 
		LCD_WR_DATA(twidth&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(theight>>8); 
		LCD_WR_DATA(theight&0XFF); 
	}
}

/*
void LCD_Set_Window(u16 sx,u16 sy,u16 ex,u16 ey)       
{
         u8 hsareg,heareg,vsareg,veareg;
         u16 hsaval,heaval,vsaval,veaval; 
#if USE_HORIZONTAL  //????       
         //???
         hsaval=sy;                                   
         heaval=ey;
         vsaval=319-ex;
         veaval=319-sx;        
#else                                     //??
         //???
         hsaval=sx;                                   
         heaval=ex;
         vsaval=sy;
         veaval=ey;       
#endif
         {
                   hsareg=0X50;heareg=0X51;//?????????
                   vsareg=0X52;veareg=0X53;//?????????       
         }                                                                          
         LCD_WriteReg(hsareg,hsaval);
         LCD_WriteReg(heareg,heaval);
         LCD_WriteReg(vsareg,vsaval);
         LCD_WriteReg(veareg,veaval);
}
*/
//³õÊ¼»¯lcd
//¸Ã³õÊ¼»¯º¯Êı¿ÉÒÔ³õÊ¼»¯¸÷ÖÖALIENTEK³öÆ·µÄLCDÒº¾§ÆÁ
//±¾º¯ÊıÕ¼ÓÃ½Ï´óflash,ÓÃ»§¿ÉÒÔ¸ù¾İ×Ô¼ºµÄÊµ¼ÊÇé¿ö,É¾µôÎ´ÓÃµ½µÄLCD³õÊ¼»¯´úÂë.ÒÔ½ÚÊ¡¿Õ¼ä.
void LCD_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); //Ê¹ÄÜPORTB,CÊ±ÖÓºÍAFIOÊ±ÖÓ
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//¿ªÆôSWD£¬Ê§ÄÜJTAG
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;	   ///PORTC6~10¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC	

	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  PORTBÍÆÍìÊä³ö
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
 
	GPIO_SetBits(GPIOB,GPIO_Pin_All);

  delay_ms(50); // delay 50 ms 
	LCD_WriteReg(0x0000,0x0001);
	delay_ms(50); // delay 50 ms 
  lcddev.id = LCD_ReadReg(0x0000);   
	if(lcddev.id<0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)//¶Áµ½ID²»ÕıÈ·,ĞÂÔölcddev.id==0X9300ÅĞ¶Ï£¬ÒòÎª9341ÔÚÎ´±»¸´Î»µÄÇé¿öÏÂ»á±»¶Á³É9300
	{	
 		//³¢ÊÔ9341 IDµÄ¶ÁÈ¡		
		LCD_WR_REG(0XD3);				   
		LCD_RD_DATA(); 				//dummy read 	
 		LCD_RD_DATA();   	    	//¶Áµ½0X00
  		lcddev.id=LCD_RD_DATA();   	//¶ÁÈ¡93								   
 		lcddev.id<<=8;
		lcddev.id|=LCD_RD_DATA();  	//¶ÁÈ¡41 	   			   
 		 	
	}
 	printf(" LCD ID:%x\r\n",lcddev.id); //´òÓ¡LCD ID  
	if(lcddev.id==0X9341)	//9341³õÊ¼»¯
	{	 
		LCD_WR_REG(0xCF);  
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0xC1); 
		LCD_WR_DATAX(0X30); 
		LCD_WR_REG(0xED);  
		LCD_WR_DATAX(0x64); 
		LCD_WR_DATAX(0x03); 
		LCD_WR_DATAX(0X12); 
		LCD_WR_DATAX(0X81); 
		LCD_WR_REG(0xE8);  
		LCD_WR_DATAX(0x85); 
		LCD_WR_DATAX(0x10); 
		LCD_WR_DATAX(0x7A); 
		LCD_WR_REG(0xCB);  
		LCD_WR_DATAX(0x39); 
		LCD_WR_DATAX(0x2C); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x34); 
		LCD_WR_DATAX(0x02); 
		LCD_WR_REG(0xF7);  
		LCD_WR_DATAX(0x20); 
		LCD_WR_REG(0xEA);  
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_REG(0xC0);    //Power control 
		LCD_WR_DATAX(0x1B);   //VRH[5:0] 
		LCD_WR_REG(0xC1);    //Power control 
		LCD_WR_DATAX(0x01);   //SAP[2:0];BT[3:0] 
		LCD_WR_REG(0xC5);    //VCM control 
		LCD_WR_DATAX(0x30); 	 //3F
		LCD_WR_DATAX(0x30); 	 //3C
		LCD_WR_REG(0xC7);    //VCM control2 
		LCD_WR_DATAX(0XB7); 
		LCD_WR_REG(0x36);    // Memory Access Control 
		LCD_WR_DATAX(0x48); 
		LCD_WR_REG(0x3A);   
		LCD_WR_DATAX(0x55); 
		LCD_WR_REG(0xB1);   
		LCD_WR_DATAX(0x00);   
		LCD_WR_DATAX(0x1A); 
		LCD_WR_REG(0xB6);    // Display Function Control 
		LCD_WR_DATAX(0x0A); 
		LCD_WR_DATAX(0xA2); 
		LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
		LCD_WR_DATAX(0x00); 
		LCD_WR_REG(0x26);    //Gamma curve selected 
		LCD_WR_DATAX(0x01); 
		LCD_WR_REG(0xE0);    //Set Gamma 
		LCD_WR_DATAX(0x0F); 
		LCD_WR_DATAX(0x2A); 
		LCD_WR_DATAX(0x28); 
		LCD_WR_DATAX(0x08); 
		LCD_WR_DATAX(0x0E); 
		LCD_WR_DATAX(0x08); 
		LCD_WR_DATAX(0x54); 
		LCD_WR_DATAX(0XA9); 
		LCD_WR_DATAX(0x43); 
		LCD_WR_DATAX(0x0A); 
		LCD_WR_DATAX(0x0F); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x00); 		 
		LCD_WR_REG(0XE1);    //Set Gamma 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x15); 
		LCD_WR_DATAX(0x17); 
		LCD_WR_DATAX(0x07); 
		LCD_WR_DATAX(0x11); 
		LCD_WR_DATAX(0x06); 
		LCD_WR_DATAX(0x2B); 
		LCD_WR_DATAX(0x56); 
		LCD_WR_DATAX(0x3C); 
		LCD_WR_DATAX(0x05); 
		LCD_WR_DATAX(0x10); 
		LCD_WR_DATAX(0x0F); 
		LCD_WR_DATAX(0x3F); 
		LCD_WR_DATAX(0x3F); 
		LCD_WR_DATAX(0x0F); 
		LCD_WR_REG(0x2B); 
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x01);
		LCD_WR_DATAX(0x3f);
		LCD_WR_REG(0x2A); 
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0xef);	 
		LCD_WR_REG(0x11); //Exit Sleep
		delay_ms(120);
		LCD_WR_REG(0x29); //display on	
	}	
	LCD_Display_Dir(0);		 	//Ä¬ÈÏÎªÊúÆÁ
	LCD_LED=1;					//µãÁÁ±³¹â
	LCD_Clear(WHITE);
}  		  
  
//ÇåÆÁº¯Êı
//color:ÒªÇåÆÁµÄÌî³äÉ«
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//µÃµ½×ÜµãÊı
	LCD_SetCursor(0x00,0x0000);	//ÉèÖÃ¹â±êÎ»ÖÃ 
	LCD_WriteRAM_Prepare();     		//¿ªÊ¼Ğ´ÈëGRAM	  	  
	for(index=0;index<totalpoint;index++)LCD_WR_DATA(color);	
}  
//ÔÚÖ¸¶¨ÇøÓòÄÚÌî³äÖ¸¶¨ÑÕÉ«
//ÇøÓò´óĞ¡:(xend-xsta+1)*(yend-ysta+1)
//xsta
//color:ÒªÌî³äµÄÑÕÉ«
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
		xlen=ex-sx+1;	 
		for(i=sy;i<=ey;i++)
		{
		 	LCD_SetCursor(sx,i);      				//ÉèÖÃ¹â±êÎ»ÖÃ 
			LCD_WriteRAM_Prepare();     			//¿ªÊ¼Ğ´ÈëGRAM	  
			for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//ÉèÖÃ¹â±êÎ»ÖÃ 	    
		}
}  
//ÔÚÖ¸¶¨ÇøÓòÄÚÌî³äÖ¸¶¨ÑÕÉ«¿é			 
//(sx,sy),(ex,ey):Ìî³ä¾ØĞÎ¶Ô½Ç×ø±ê,ÇøÓò´óĞ¡Îª:(ex-sx+1)*(ey-sy+1)   
//color:ÒªÌî³äµÄÑÕÉ«
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//µÃµ½Ìî³äµÄ¿í¶È
	height=ey-sy+1;			//¸ß¶È
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//ÉèÖÃ¹â±êÎ»ÖÃ 
		LCD_WriteRAM_Prepare();     //¿ªÊ¼Ğ´ÈëGRAM
		for(j=0;j<width;j++)LCD_WR_DATA(color[i*width+j]);//Ğ´ÈëÊı¾İ 
	}	  
} 
//»­Ïß
//x1,y1:Æğµã×ø±ê
//x2,y2:ÖÕµã×ø±ê  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //¼ÆËã×ø±êÔöÁ¿ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //ÉèÖÃµ¥²½·½Ïò 
	else if(delta_x==0)incx=0;//´¹Ö±Ïß 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//Ë®Æ½Ïß 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //Ñ¡È¡»ù±¾ÔöÁ¿×ø±êÖá 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//»­ÏßÊä³ö 
	{  
		LCD_DrawPoint(uRow,uCol);//»­µã 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//»­¾ØĞÎ	  
//(x1,y1),(x2,y2):¾ØĞÎµÄ¶Ô½Ç×ø±ê
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//ÔÚÖ¸¶¨Î»ÖÃ»­Ò»¸öÖ¸¶¨´óĞ¡µÄÔ²
//(x,y):ÖĞĞÄµã
//r    :°ë¾¶
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //ÅĞ¶ÏÏÂ¸öµãÎ»ÖÃµÄ±êÖ¾
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//Ê¹ÓÃBresenhamËã·¨»­Ô²     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸ö×Ö·û
//x,y:ÆğÊ¼×ø±ê
//num:ÒªÏÔÊ¾µÄ×Ö·û:" "--->"~"
//size:×ÖÌå´óĞ¡ 12/16/24
//mode:µş¼Ó·½Ê½(1)»¹ÊÇ·Çµş¼Ó·½Ê½(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//µÃµ½×ÖÌåÒ»¸ö×Ö·û¶ÔÓ¦µãÕó¼¯ËùÕ¼µÄ×Ö½ÚÊı	
 	num=num-' ';//µÃµ½Æ«ÒÆºóµÄÖµ£¨ASCII×Ö¿âÊÇ´Ó¿Õ¸ñ¿ªÊ¼È¡Ä££¬ËùÒÔ-' '¾ÍÊÇ¶ÔÓ¦×Ö·ûµÄ×Ö¿â£©
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//µ÷ÓÃ1206×ÖÌå
		else if(size==16)temp=asc2_1608[num][t];	//µ÷ÓÃ1608×ÖÌå
		else if(size==24)temp=asc2_2412[num][t];	//µ÷ÓÃ2412×ÖÌå
		else return;								//Ã»ÓĞµÄ×Ö¿â
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//³¬ÇøÓòÁË
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//³¬ÇøÓòÁË
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^nº¯Êı
//·µ»ØÖµ:m^n´Î·½.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//ÏÔÊ¾Êı×Ö,¸ßÎ»Îª0,Ôò²»ÏÔÊ¾
//x,y :Æğµã×ø±ê	 
//len :Êı×ÖµÄÎ»Êı
//size:×ÖÌå´óĞ¡
//color:ÑÕÉ« 
//num:ÊıÖµ(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//ÏÔÊ¾Êı×Ö,¸ßÎ»Îª0,»¹ÊÇÏÔÊ¾
//x,y:Æğµã×ø±ê
//num:ÊıÖµ(0~999999999);	 
//len:³¤¶È(¼´ÒªÏÔÊ¾µÄÎ»Êı)
//size:×ÖÌå´óĞ¡
//mode:
//[7]:0,²»Ìî³ä;1,Ìî³ä0.
//[6:1]:±£Áô
//[0]:0,·Çµş¼ÓÏÔÊ¾;1,µş¼ÓÏÔÊ¾.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//ÏÔÊ¾×Ö·û´®
//x,y:Æğµã×ø±ê
//width,height:ÇøÓò´óĞ¡  
//size:×ÖÌå´óĞ¡
//*p:×Ö·û´®ÆğÊ¼µØÖ·		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//ÅĞ¶ÏÊÇ²»ÊÇ·Ç·¨×Ö·û!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//ÍË³ö
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}
void show_initial(void)  //??ê???á?µ?????2?·?
{
	 LCD_Init();
   POINT_COLOR=BLACK;
	 LCD_ShowString(60,100,200,16,16,"welcome...");
	 LCD_ShowString(60,120,200,16,16,"Initialation...");
}





