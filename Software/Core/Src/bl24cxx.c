

#include "bl24cxx.h"
#include "i2c.h"
#include <stdio.h>

#define IICx hi2c1 //IIC�ӿ�
#define AT24C_DEV_WRITEADDR 0xA0 //�豸��ַ
#define AT24C_DEV_READADDR 0xA1 //�豸��ַ
/*****************************************
��������void HAL_AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t WriteData)
������WriteAddr :Ҫд�����ݵĵ�ַ  WriteData��Ҫд�������
������������ָ����ַд��һ���ֽ�����
����ֵ����
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
��������uint8_t HAL_AT24CXX_ReadOneByte(uint16_t ReadAddr)
������ ReadAddr��Ҫ��ȡ���ݵĵ�ַ
������������ָ����ַ��ȡһ���ֽ�����
����ֵ�����ض�ȡ��ַ������
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
��������void HAL_AT24CXX_WriteLenByte(uint16_t WriteAddr,uint8_t *pData,uint8_t dataLen)
������WriteAddr :Ҫд�����ݵĵ�ַ  pData��Ҫд������ݵ��׵�ַ datalen��Ҫд�����ݵĳ���
������������ָ����ַ��ʼд�����ֽ�����
����ֵ����
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
��������void HAL_AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t *pData,uint8_t dataLen)
������ ReadAddr��Ҫ��ȡ���ݵĵ�ַ pData�����������׵�ַ dataLen:���ݳ���
������������ָ����ַ��ʼ��ȡ������ֽ�����
����ֵ����
*****************************************/
void HAL_AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t *pData,uint8_t dataLen)
{
	while(dataLen--)
	{
		*pData++ = HAL_AT24CXX_ReadOneByte(ReadAddr++);
	}
}
/*****************************************
��������uint8_t HAL_AT24CXX_Check(void)
��������
�������������AT24CXX�Ƿ���������������24XX�����һ����ַ(255)���洢��־��.���������24Cϵ��,�����ַҪ�޸�
����ֵ�����ɹ�����0 ʧ�ܷ���1
*****************************************/
uint8_t HAL_AT24CXX_Check(void)
{
	uint8_t temp;
	temp = HAL_AT24CXX_ReadOneByte(EE_TYPE);//����ÿ�ο�����дAT24CXX			   
	if(temp == 0XAB)
		return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		HAL_AT24CXX_WriteOneByte(EE_TYPE,0XAB);
	    temp = HAL_AT24CXX_ReadOneByte(EE_TYPE);	  
		if(temp == 0XAB)
			return 0;
	}
	return 1;											  
}




