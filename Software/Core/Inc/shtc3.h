#ifndef __SHTC3_H
#define __SHTC3_H
#include "stdint.h"
typedef struct
{
	float humi;
	float temp;	
}SHTC3_Data_TypeDef;

uint8_t SHTC3_ReadData(SHTC3_Data_TypeDef* SHTC3_Data);
 
#endif  //SHTC3
