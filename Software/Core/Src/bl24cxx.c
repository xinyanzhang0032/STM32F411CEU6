

#include "bl24cxx.h"
#include "i2c.h"
#include <stdio.h>

#define IICx hi2c1 //IIC接口
#define AT24C_DEV_WRITEADDR 0xA0 //设备地址
#define AT24C_DEV_READADDR 0xA1 //设备地址
/*****************************************
函数名：void HAL_AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t WriteData)
参数：WriteAddr :要写入数据的地址  WriteData：要写入的数据
功能描述：在指定地址写入一个字节数据
返回值：无
*****************************************/
uint8_t HAL_AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t WriteData)
{
	uint8_t res = 0xff;
	if(EE_TYPE < AT24C16)
	{
		if(HAL_I2C_Mem_Write(&IICx,AT24C_DEV_WRITEADDR,WriteAddr,I2C_MEMADD_SIZE_8BIT,&WriteData,1,0xff) == HAL_OK)
			res = 0;
	}
	else
		if(HAL_I2C_Mem_Write(&IICx,AT24C_DEV_WRITEADDR,WriteAddr,I2C_MEMADD_SIZE_16BIT,&WriteData,1,0xff) == HAL_OK)
			 res = 0;
	HAL_Delay(10);
	return res;
}
/*****************************************
函数名：uint8_t HAL_AT24CXX_ReadOneByte(uint16_t ReadAddr)
参数： ReadAddr：要读取数据的地址
功能描述：在指定地址读取一个字节数据
返回值：返回读取地址的数据
*****************************************/
uint8_t HAL_AT24CXX_ReadOneByte(uint16_t ReadAddr)
{
	uint8_t rxData = 0;
	if(EE_TYPE < AT24C16)
	{
		HAL_I2C_Mem_Read(&IICx,AT24C_DEV_READADDR,ReadAddr,I2C_MEMADD_SIZE_8BIT,&rxData,1,0xff);
	}
	else 
		HAL_I2C_Mem_Read(&IICx,AT24C_DEV_READADDR,ReadAddr,I2C_MEMADD_SIZE_16BIT,&rxData,1,0xff);
	HAL_Delay(10);
	return rxData;
}
/*****************************************
函数名：void HAL_AT24CXX_WriteLenByte(uint16_t WriteAddr,uint8_t *pData,uint8_t dataLen)
参数：WriteAddr :要写入数据的地址  pData：要写入的数据的首地址 datalen：要写入数据的长度
功能描述：从指定地址开始写入多个字节数据
返回值：无
*****************************************/
void HAL_AT24CXX_WriteLenByte(uint16_t WriteAddr,uint8_t *pData,uint8_t dataLen)
{
	while(dataLen--)
	{
		HAL_AT24CXX_WriteOneByte(WriteAddr,*pData);
		WriteAddr++;
		pData++;
	}
}
/*****************************************
函数名：void HAL_AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t *pData,uint8_t dataLen)
参数： ReadAddr：要读取数据的地址 pData：回填数据首地址 dataLen:数据长度
功能描述：从指定地址开始读取多个个字节数据
返回值：无
*****************************************/
void HAL_AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t *pData,uint8_t dataLen)
{
	while(dataLen--)
	{
		*pData++ = HAL_AT24CXX_ReadOneByte(ReadAddr++);
	}
}
/*****************************************
函数名：uint8_t HAL_AT24CXX_Check(void)
参数：无
功能描述：检查AT24CXX是否正常，这里用了24XX的最后一个地址(255)来存储标志字.如果用其他24C系列,这个地址要修改
返回值：检测成功返回0 失败返回1
*****************************************/
uint8_t HAL_AT24CXX_Check(void)
{
	uint8_t temp;
	temp = HAL_AT24CXX_ReadOneByte(EE_TYPE);//避免每次开机都写AT24CXX			   
	if(temp == 0XAB)
		return 0;		   
	else//排除第一次初始化的情况
	{
		HAL_AT24CXX_WriteOneByte(EE_TYPE,0XAB);
	    temp = HAL_AT24CXX_ReadOneByte(EE_TYPE);	  
		if(temp == 0XAB)
			return 0;
	}
	return 1;											  
}




