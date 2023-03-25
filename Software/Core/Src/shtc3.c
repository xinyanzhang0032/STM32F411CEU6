#include "shtc3.h"
#include "i2c.h"

#define SHTC3_ADDR					(0x70)
#define SHTC3_CMD_DEV_WRITE 		(0xE0)		//(SHTC3_ADDR<<1)
#define SHTC3_CMD_DEV_READ			(0xE1)		//((SHTC3_ADDR<<1)|0x01)
#define SHTC3_CMD_SOFT_RESET		0x805D		//软件复位命令
#define SHTC3_CMD_READ_ID			0xEFC8		//读取SHTC3 ID命令
#define SHTC3_CMD_WAKEUP			0x3517
#define SHTC3_CMD_READ_HUM_DATA 	0x5c24
#define SHTC3_CMD_READ_TEMP_DATA	0x7866
#define SHTC3_CMD_READ_SLEEP		0xB098
typedef struct
{
	uint8_t HumidityMSB;
	uint8_t HumidityLSB;
	uint8_t HumidityCRC;
 
	uint8_t temperatureMSB;
	uint8_t temperatureLSB;
	uint8_t temperatureCRC;
}Sthc3MeasureData;

	
uint8_t Sthc3WriteByte(uint16_t Cmd)
{
	uint8_t WriteCmd[2];
	WriteCmd[0] = (uint8_t)(Cmd>>8);
	WriteCmd[1] = (uint8_t)(Cmd);

	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)SHTC3_CMD_DEV_WRITE , (uint8_t*)&WriteCmd, sizeof(WriteCmd), 1000) != HAL_OK)
	{
		Error_Handler();
	}

	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
	return 0;
}

 
uint16_t Sthc3ReadWord(uint16_t Cmd)
{
	uint8_t WriteCmd[2];
	uint16_t ReadData;
	
	WriteCmd[0] = (uint8_t)(Cmd>>8);
	WriteCmd[1] = (uint8_t)(Cmd);
	
	
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)SHTC3_CMD_DEV_WRITE , (uint8_t*)WriteCmd,  sizeof(WriteCmd), 1000) != HAL_OK) 
	{
		Error_Handler();
	}
 
	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
 
	if(HAL_I2C_Master_Receive(&hi2c1, SHTC3_CMD_DEV_READ , (uint8_t*)&ReadData, 2, 1000) != HAL_OK) //接收word数据
	{
		Error_Handler();
	}
	return ReadData;
}
 
 
void Sthc3ReadHumiAndTemp(SHTC3_Data_TypeDef* SHTC3_Data)
{
	uint8_t WriteCmd[2];
	float hum,temp;
	Sthc3MeasureData Sthc3Data = {0};
 
	
	WriteCmd[0] = (uint8_t)(SHTC3_CMD_READ_HUM_DATA>>8);
	WriteCmd[1] = (uint8_t)(SHTC3_CMD_READ_HUM_DATA);
 
	if(HAL_I2C_Master_Transmit(&hi2c1, SHTC3_CMD_DEV_WRITE , (uint8_t*)WriteCmd,  sizeof(WriteCmd), 1000) != HAL_OK) 
	{
		Error_Handler();
	}
 
	
	HAL_I2C_Master_Receive(&hi2c1, SHTC3_CMD_DEV_READ, (uint8_t *)&Sthc3Data, sizeof(Sthc3Data), 1000);
 
	hum = (Sthc3Data.HumidityMSB << 8)|(Sthc3Data.HumidityLSB);
	temp = (Sthc3Data.temperatureMSB << 8 )|Sthc3Data.temperatureLSB;
	SHTC3_Data->humi =(100*((float)hum/65535));				//湿度
	SHTC3_Data->temp =((175*((float)temp/65535)) - 45);		//温度
}

