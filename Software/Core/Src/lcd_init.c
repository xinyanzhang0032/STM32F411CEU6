#include "lcd_init.h"

void delay(int t)
{
	while(t--);
}


/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{	
	LCD_CS_Clr();
	SPI1_WriteData(dat);
	//delay(1);
	LCD_CS_Set();
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	LCD_DC_Clr();//写命令
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//写数据
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+16);
		LCD_WR_DATA(x2+16);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+16);
		LCD_WR_DATA(x2+16);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+16);
		LCD_WR_DATA(y2+16);
		LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+16);
		LCD_WR_DATA(y2+16);
		LCD_WR_REG(0x2c);//储存器写
	}
}

void LCD_Init(void)
{	
	LCD_RES_Clr();//复位
	HAL_Delay(100);
	LCD_RES_Set();
	HAL_Delay(100);
	
	LCD_BLK_Set();//打开背光
  HAL_Delay(100);
	
	LCD_WR_REG(0xFE);       //0xFE对应RGB 0xEF对应BGR
	LCD_WR_REG(0xEF);
	LCD_WR_REG(0xb0);
	LCD_WR_DATA8(0xc0);
	LCD_WR_REG(0xb2);
	LCD_WR_DATA8(0x2f);
	LCD_WR_REG(0xb3);
	LCD_WR_DATA8(0x03);
	LCD_WR_REG(0xb6);
	LCD_WR_DATA8(0x19);
	LCD_WR_REG(0xb7);
	LCD_WR_DATA8(0x01);
	LCD_WR_REG(0xAC);
	LCD_WR_DATA8(0xCB);
	LCD_WR_REG(0xAB);
	LCD_WR_DATA8(0x07);

	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x60);
	else LCD_WR_DATA8(0xA0);

	LCD_WR_REG(0xB4); 
	LCD_WR_DATA8(0x00);  

	LCD_WR_REG(0xA8);
	LCD_WR_DATA8(0x0C);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0xB8); 
	LCD_WR_DATA8(0x08);  

	LCD_WR_REG(0xE8);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0xE9);
	LCD_WR_DATA8(0x47);

	LCD_WR_REG(0xEA);
	LCD_WR_DATA8(0x44);

	LCD_WR_REG(0xEB);
	LCD_WR_DATA8(0xE0);   

	LCD_WR_REG(0xED);
	LCD_WR_DATA8(0x03);  

	LCD_WR_REG(0xC6); 
	LCD_WR_DATA8(0x19);    

	LCD_WR_REG(0xC7); 
	LCD_WR_DATA8(0x10);

	LCD_WR_REG(0xf0);
	LCD_WR_DATA8(0x0b);
	LCD_WR_DATA8(0x2f);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x4b);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x3f);
	LCD_WR_DATA8(0x3e);
	LCD_WR_DATA8(0x60);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x12);
	LCD_WR_DATA8(0x12);
	LCD_WR_DATA8(0x0f);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x1f);

	LCD_WR_REG(0xf1);
	LCD_WR_DATA8(0x0e);
	LCD_WR_DATA8(0x3d);
	LCD_WR_DATA8(0x2a);
	LCD_WR_DATA8(0x40);
	LCD_WR_DATA8(0xfa);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x60);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x1F);

	LCD_WR_REG(0x11); 

	LCD_WR_REG(0x29); 
}









