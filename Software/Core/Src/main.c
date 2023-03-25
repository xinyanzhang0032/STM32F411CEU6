/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "i2c.h"
#include "sdio.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "lcd_init.h"
#include "picture.h"
#include "shtc3.h"
#include "w25q256.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "sdio_test.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "vs1053.h"
#include "bl24cxx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
u8 Uart_Buff[13]="Hello Gerald!";
u8 Uart_Buff1[2];
SHTC3_Data_TypeDef SHTC3_Data;
u16 delay_pwm = 0;

//#define  DATA_Size			256
//#define  EEP_Firstpage      0x00
//uint8_t I2c_Buf_Write[DATA_Size];
//uint8_t I2c_Buf_Read[DATA_Size];
//uint8_t I2C_Test(void);

extern char Restart_Play_flag;
FATFS fs;													/* Work area (file system object) for logical drives */
char song_pt = 0;
uint8_t song_list[][20] = {"0:TestFile_1.mp3", 	//�������������Զ�����������û�ֻ��Ҫ�ڴ���Ӹ���������
													 "0:TestFile_2.mp3"};
						
char song_number_max = 0;
/**
  * @brief  �����������
  * @param  ��
  * @retval ��
 */							 
void song_list_init(void)
{
	song_number_max=sizeof(song_list)/sizeof(song_list[0]);
}
//char SDpath[4]; /* SD�߼�������·�� */
//FATFS fs;													/* FatFs�ļ�ϵͳ���� */
//FIL fnew;													/* �ļ����� */
//FRESULT res_sd;                /* �ļ�������� */
//UINT fnum;            			  /* �ļ��ɹ���д���� */
//BYTE ReadBuffer[1024]={0};        /* �������� */
//BYTE WriteBuffer[] =              /* д������*/
//"��ӭʹ��Ұ��STM32 F407������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";  
//extern FATFS flash_fs;
//extern const Diskio_drvTypeDef  SD_Driver;
//extern UART_HandleTypeDef huart1;   //��������
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define K1_3V3 		HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) //PA0
#define K2_GND 		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)	//PB0 
#define K1_3V3_PRES 	1
#define K2_GND_PRES	  2

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	uint16_t id = 0;
	const uint8_t test_TEXT[] = "W25Qxx test";
	uint8_t TXET_read[sizeof(test_TEXT)];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//void Uart1_printf(const char *format,...);


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == K2_GND_EXTI0_Pin) //�ж����������Ų����ж�
	{
		Restart_Play_flag = 1;
	}	

}

//uint8_t I2C_Test(void)
//{
//	uint16_t i;

//	EEPROM_INFO("д�������");

//	for ( i=0; i<DATA_Size; i++ ) //��仺��
//	{   
//		I2c_Buf_Write[i] =i;
//		printf("0x%02X ", I2c_Buf_Write[i]);
//		if(i%16 == 15)    
//		printf("\n\r");    
//	}

//	//��I2c_Buf_Write��˳�����������д��EERPOM�� 
//	I2C_EE_BufferWrite( I2c_Buf_Write, EEP_Firstpage, DATA_Size);

//	EEPROM_INFO("����������");
//	//��EEPROM��������˳�򱣳ֵ�I2c_Buf_Read��
//	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, DATA_Size); 
//	//��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
//	for (i=0; i<DATA_Size; i++)
//	{	
//		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
//		{
//			printf("0x%02X ", I2c_Buf_Read[i]);
//			EEPROM_ERROR("����:I2C EEPROMд������������ݲ�һ��");
//			return 0;
//		}
//		printf("0x%02X ", I2c_Buf_Read[i]);
//		if(i%16 == 15)    
//		printf("\n\r");

//	}
//	EEPROM_INFO("I2C(AT24C02)��д���Գɹ�");
//	return 1;
//}
//unsigned int count = 0;
//void TestSD(void)
//{
//    FRESULT f_res;
//    FIL file;
//    FATFS SD;
//		int len,len2;
//		
//		//--- ���� TF/SD �� ---
//    f_res = f_mount(&SD, "0:", 1); 
//    if(f_res == FR_OK) printf("SD mount OK!\r\n");
//    else printf("SD mount error = %d\r\n", f_res);

//    //FA_READ | FA_WRITE | FA_CREATE_ALWAYS | FA_CREATE_NEW | FA_OPEN_ALWAYS | FA_OPEN_APPEND | FA_SEEKEND
//    //f_res = f_open(&file,"0:test1.txt", FA_WRITE | FA_CREATE_ALWAYS);
//	
//		//��д�뷽ʽ��test1.txt�ļ�������������򴴽����ļ�
//		f_res = f_open(&file,"0:test1.txt", FA_WRITE | FA_OPEN_ALWAYS);
//    if(f_res == FR_OK) printf("'test1.txt' Open OK!\r\n");
//    else printf("'test1.txt' Open error = %d\r\n", f_res);
//	
//		//�ƶ����ļ�ĩβ
//		f_res = f_lseek(&file, (&file)->fsize);
//    
//		//����д���ļ�
//		count++;
//    len = f_printf(&file, "%s", "\r\n0123456789\t"); //?????
//		len2 = f_printf(&file, "%d", count); //?????
//	
//    if( len >= 0 ) printf("Write OK!write size = %d\t%d\r\n", (len+len2), count);
//    else printf("Write error!\r\n");

//		//д���ļ��󣬹ر��ļ�
//    f_res = f_close(&file); //?????,??????,??????
//    if(f_res == FR_OK) printf("'test1.txt' Close OK!\r\n");
//    else printf("'test1.txt' Close error = %d\r\n", f_res);
//		
//		//ȡ�����ش洢��
//		f_res = f_mount(NULL, "0:", 1);
//		if(f_res == FR_OK) printf("SD unmount OK!\r\n");
//    else printf("SD unmount error = %d\r\n", f_res);
//		
//		printf("\r\n");
//}
//void PWM_LED(void)
//{
//	while(delay_pwm<1000)
//	{
//			HAL_Delay(1);
//		  delay_pwm++;
//			TIM4->CCR1=delay_pwm;	
//	}
//	while(delay_pwm>0)
//	{
//			HAL_Delay(1);
//		  delay_pwm--;
//			TIM4->CCR1=delay_pwm;	
//	}
//}
//uint8_t KEY_Scan(uint8_t mode)
//{	 
//	static uint8_t key_up=1;//�������ɿ���־
//	if(mode)key_up=1;  //֧������		  
//	if(key_up&&(K1_3V3==1||K2_GND==0))
//	{
//		HAL_Delay(20);//ȥ���� 
//		key_up=0;
//		if(K1_3V3==1)return 1;
//		else if(K2_GND==0)return 2;
//	}else if(K1_3V3==0&&K2_GND==1)key_up=1; 	    
// 	return 0;// �ް�������
//}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

//	uint8_t key_value;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */




  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI4_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */


//	HAL_TIM_Base_Start_IT(&htim4);
//	HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);
//	HAL_TIM_Base_Start(&htim4);
//  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
//	LCD_Init();
//	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
//	HAL_UART_Transmit(&huart6,Uart_Buff,13,0xffff);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);	
//	Bmp_Init();

//	f_mount(&fs,"0:",1);
//	VS_Sine_Test();
//	VS_HD_Reset();
//	VS_Soft_Reset();
//	song_list_init();
	
	
	uint8_t buff[6];
	uint8_t res;
//	I2C_EE_Init();
	printf("���Գ���ʼ\r\n");
	res = HAL_AT24CXX_Check();
	if(res == 0)
	{
		printf("AT24CXX OK!\r\n");
	}
	else
	{
		printf("AT24CXX ERROR!\r\n");
	}
	HAL_Delay(1000);
	HAL_AT24CXX_WriteLenByte(0,(uint8_t *)"hello",5);

	HAL_AT24CXX_ReadLenByte(0,buff,5);
	
	printf("buff:%s\r\n",buff);
	
	printf("���Գ������\r\n");
	   //�����������������̷�
//	FATFS_LinkDriver(&SD_Driver, SDpath);
//   //���ⲿSD�������ļ�ϵͳ���ļ�ϵͳ����ʱ���SD����ʼ��
//	res_sd = f_mount(&fs,"0:",1);  
//   /*----------------------- ��ʽ������ ---------------------------*/  
//   /* ���û���ļ�ϵͳ�͸�ʽ�������ļ�ϵͳ */
//   if(res_sd == FR_NO_FILESYSTEM)
//   {
//      printf("��SD����û���ļ�ϵͳ���������и�ʽ��...\r\n");
//      /* ��ʽ�� */
//      res_sd= f_mkfs("0:",FM_FAT32,0,ReadBuffer,sizeof(ReadBuffer));					

//      if(res_sd == FR_OK)
//      {
//        printf("��SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
//        /* ��ʽ������ȡ������ */
//        res_sd = f_mount(NULL,"0:",1);			
//        /* ���¹���	*/			
//        res_sd = f_mount(&fs,"0:",1);
//      }
//      else
//      {
//        printf("������ʽ��ʧ�ܡ�����\r\n");
//        while(1);
//      }
//   }
//   else if(res_sd!=FR_OK)
//   {
//      printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
//      printf("��������ԭ��SD����ʼ�����ɹ���\r\n");
//      while(1);
//   }
//   else
//   {
//      printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
//   }	
//    /*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
//    /* ���ļ�������ļ��������򴴽��� */
//    printf("\r\n****** ���������ļ�д�����... ******\r\n");	
//    res_sd = f_open(&fnew, "0:FatFs��д�����ļ�.txt",FA_CREATE_ALWAYS | FA_WRITE );
//    if ( res_sd == FR_OK )
//    {
//      printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
//      /* ��ָ���洢������д�뵽�ļ��� */
//      res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
//      if(res_sd==FR_OK)
//      {
//        printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
//        printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
//      }
//      else
//      {
//        printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_sd);
//      }    
//      /* ���ٶ�д���ر��ļ� */
//      f_close(&fnew);
//    }
//    else
//    {	
//      printf("������/�����ļ�ʧ�ܡ�\r\n");
//    }
//    
//  /*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
//    printf("****** ���������ļ���ȡ����... ******\r\n");
//    res_sd = f_open(&fnew, "0:FatFs��д�����ļ�.txt", FA_OPEN_EXISTING | FA_READ); 	 
//    if(res_sd == FR_OK)
//    {
//      printf("�����ļ��ɹ���\r\n");
//      res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
//      if(res_sd==FR_OK)
//      {
//        printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
//        printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
//      }
//      else
//      {
//        printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_sd);
//      }		
//    }
//    else
//    {
//      printf("�������ļ�ʧ�ܡ�\r\n");
//    }
//    /* ���ٶ�д���ر��ļ� */
//    f_close(&fnew);	
//    
//    /* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
//    f_mount(NULL,"0:",1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//		vs1053_player_song(song_list[song_pt]);
//		if(HAL_UART_Receive(&huart6,Uart_Buff1,sizeof(Uart_Buff1),1000)==HAL_OK)
//		{
//			HAL_UART_Transmit(&huart6,Uart_Buff1,sizeof(Uart_Buff1),100);
//		}
//		HAL_UART_Transmit(&huart6, (uint8_t *)"hello windows!\r\n", 16 , 0xffff);
//		HAL_Delay(100);
//			if(I2C_Test() ==1)
//			{
////				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
//				HAL_UART_Transmit(&huart6, (uint8_t *)"hello windows!\r\n", 16 , 0xffff);
//		    HAL_Delay(1000);
//			}
//			else
//			{
////				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
//				HAL_UART_Transmit(&huart6, (uint8_t *)"Hello windows!\r\n", 16 , 0xffff);
//		    HAL_Delay(1000);
//			}
//		PWM_LED();
//		key_value = KEY_Scan(0);
//		if(key_value)
//		{
//			switch(key_value)
//			{				 
//				case K1_3V3_PRES:	//����LED1��ת
//					W25QXX_Write((uint8_t *)test_TEXT, 0x000000FF, sizeof(test_TEXT));
//				  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//					break;
//				case K2_GND_PRES:	//����LED1��ת	 
//					W25QXX_Read(TXET_read, 0x000000FF, sizeof(test_TEXT));
//				  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//					break;
//			}
//		}
//		Sthc3ReadHumiAndTemp(&SHTC3_Data);
////  LCD_ShowPicture(0,0,160,96,gImage_1);
//		LCD_ShowString(0,0,"temp:",RED,WHITE,16,0);
//		LCD_ShowFloatNum1(38,0,SHTC3_Data.temp,4,RED,WHITE,16);
//		LCD_ShowString(80,0,"C",RED,WHITE,16,0);
//		LCD_ShowString(0,16,"humi:",RED,WHITE,16,0);
//		LCD_ShowFloatNum1(38,16,SHTC3_Data.humi,4,RED,WHITE,16);
//		LCD_ShowString(80,16,"%",RED,WHITE,16,0);
//		while(BMP280_GetStatus(BMP280_MEASURING) != RESET);
//		while(BMP280_GetStatus(BMP280_IM_UPDATE) != RESET);
//		LCD_ShowString(0,32,"pres:",RED,WHITE,16,0);
//		LCD_ShowFloatNum1(38,32,BMP280_Get_Pressure()/1000,6,RED,WHITE,16);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//  /* Prevent unused argument(s) compilation warning */
//  UNUSED(huart);
//  /* NOTE: This function Should not be modified, when the callback is needed,
//           the HAL_UART_TxCpltCallback could be implemented in the user file
//   */
// 
//	if(Uart1_Rx_Cnt >= 255)  //����ж�
//	{
//		Uart1_Rx_Cnt = 0;
//		memset(RxBuffer,0x00,sizeof(RxBuffer));
//		HAL_UART_Transmit(&huart2, (uint8_t *)"�������", 10,0xFFFF); 	
//        
//	}
//	else
//	{
//		RxBuffer[Uart1_Rx_Cnt++] = aRxBuffer;   //��������ת��
//	
//		if((RxBuffer[Uart1_Rx_Cnt-1] == 0x0A)&&(RxBuffer[Uart1_Rx_Cnt-2] == 0x0D)) //�жϽ���λ
//		{
//			HAL_UART_Transmit(&huart2, (uint8_t *)&RxBuffer, Uart1_Rx_Cnt,0xFFFF); //���յ�����Ϣ���ͳ�ȥ
//            while(HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX);//���UART���ͽ���
//			Uart1_Rx_Cnt = 0;
//			memset(RxBuffer,0x00,sizeof(RxBuffer)); //�������
//		}
//	}
//	
//	HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);   //�ٿ��������ж�
//}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
