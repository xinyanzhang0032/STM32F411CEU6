#ifndef BL24CXX_H__
#define BL24CXX_H__
/*****************************************
						本驱动文件仅适配HAL库版本
******************************************/
#include "stm32f4xx_hal.h"	//链接HAL库

//typedef enum
//{
//	bl24cxx_false,
//	bl24cxx_true,
//}bool;


#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	  8191
#define AT24C128	16383
#define AT24C256	32767  
//我使用的是AT24C64
#define EE_TYPE AT24C64


uint8_t HAL_AT24CXX_ReadOneByte(uint16_t ReadAddr);

uint8_t HAL_AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t WriteData);

void HAL_AT24CXX_WriteLenByte(uint16_t WriteAddr,uint8_t *pData,uint8_t dataLen);

void HAL_AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t *pData,uint8_t dataLen);

uint8_t HAL_AT24CXX_Check(void);

#endif



//#ifndef __I2C_EE_H
//#define	__I2C_EE_H

//#include "stm32f4xx.h"

///* AT24C01/02每页有8个字节 */
////#define EEPROM_PAGESIZE    32
//#define EEPROM_PAGESIZE 	   32
///* AT24C04/08A/16A每页有16个字节 */
////#define EEPROM_PAGESIZE           16			

///* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
//#define I2C_OWN_ADDRESS7      0X0A   

////#define I2Cx                             I2C1
////#define I2Cx_CLK_ENABLE()                __HAL_RCC_I2C1_CLK_ENABLE()
////#define I2Cx_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
////#define I2Cx_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE() 

////#define I2Cx_FORCE_RESET()               __HAL_RCC_I2C1_FORCE_RESET()
////#define I2Cx_RELEASE_RESET()             __HAL_RCC_I2C1_RELEASE_RESET()

/////* Definition for I2Cx Pins */
////#define I2Cx_SCL_PIN                    GPIO_PIN_8
////#define I2Cx_SCL_GPIO_PORT              GPIOB
////#define I2Cx_SCL_AF                     GPIO_AF4_I2C1
////#define I2Cx_SDA_PIN                    GPIO_PIN_9
////#define I2Cx_SDA_GPIO_PORT              GPIOB
////#define I2Cx_SDA_AF                     GPIO_AF4_I2C1


///*等待超时时间*/
//#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
//#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

//#define I2Cx_TIMEOUT_MAX                300
///* Maximum number of trials for HAL_I2C_IsDeviceReady() function */
//#define EEPROM_MAX_TRIALS               300

///*信息输出*/
//#define EEPROM_DEBUG_ON         0

//#define EEPROM_INFO(fmt,arg...)           printf("<<-EEPROM-INFO->> "fmt"\n",##arg)
//#define EEPROM_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
//#define EEPROM_DEBUG(fmt,arg...)          do{\
//                                          if(EEPROM_DEBUG_ON)\
//                                          printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
//                                          }while(0)

///* 
// * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
// * 32 pages of 8 bytes each
// *
// * Device Address
// * 1 0 1 0 A2 A1 A0 R/W
// * 1 0 1 0 0  0  0  0 = 0XA0
// * 1 0 1 0 0  0  0  1 = 0XA1 
// */

///* EEPROM Addresses defines */
//#define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */
//#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 0 */
//#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
//#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */

//#define EEPROM_ADDRESS        0xA0

//void I2C_EE_Init(void);
//										  
//void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
//uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr);
//uint32_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite);
//uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

//#endif /* __I2C_EE_H */
