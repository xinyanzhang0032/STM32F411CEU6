#include "w25q256.h" 
#include "spi.h"
#include "gpio.h"

#define Dummy_Byte		0xFF

uint16_t W25QXX_TYPE=0;	

uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;	  
	W25QXX_CS_Enable;
	SPI1_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(Dummy_Byte)<<8;  
	Temp|=SPI1_ReadWriteByte(Dummy_Byte);	 
	W25QXX_CS_Disable;
	return Temp;
}   		    

void W25QXX_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)   
{ 
 	uint16_t i;   										    
	W25QXX_CS_Enable;                            //ʹ������   
   SPI1_ReadWriteByte(W25X_ReadData);      //���Ͷ�ȡ����  
    if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        SPI1_ReadWriteByte((u8)((ReadAddr)>>24));    
    }
    SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>16));   //����24bit��ַ    
    SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>8));   
    SPI1_ReadWriteByte((uint8_t)ReadAddr);   
    for(i=0; i<NumByteToRead; i++)
	{ 
        pBuffer[i]=SPI1_ReadWriteByte(0XFF);    //ѭ������  
    }
	W25QXX_CS_Disable;  				    	      
}  

//W25QXXдʹ��	
//��WEL��λ   
void W25QXX_Write_Enable(void)   
{
	W25QXX_CS_Enable;                            //ʹ������   
  SPI1_ReadWriteByte(W25X_WriteEnable);   //����дʹ��  
	W25QXX_CS_Disable;                            //ȡ��Ƭѡ     	      
}
//W25QXXд��ֹ	
//��WEL����  
void W25QXX_Write_Disable(void)   
{  
	W25QXX_CS_Enable;                            //ʹ������   
  SPI1_ReadWriteByte(W25X_WriteDisable);  //����д��ָֹ��    
	W25QXX_CS_Disable;                            //ȡ��Ƭѡ     	      
}

uint8_t W25QXX_ReadSR(uint8_t regno)   
{  
	uint8_t byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=W25X_ReadStatusReg1;    //��״̬�Ĵ���1ָ��
            break;
        case 2:
            command=W25X_ReadStatusReg2;    //��״̬�Ĵ���2ָ��
            break;
        case 3:
            command=W25X_ReadStatusReg3;    //��״̬�Ĵ���3ָ��
            break;
        default:
            command=W25X_ReadStatusReg1;    
            break;
    }    
	W25QXX_CS_Enable;                            //ʹ������   
	SPI1_ReadWriteByte(command);            //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI1_ReadWriteByte(0Xff);          //��ȡһ���ֽ�  
	W25QXX_CS_Disable;                            //ȡ��Ƭѡ     
	return byte;   
}

//�ȴ�����
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   // �ȴ�BUSYλ���
}  

//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ������������ʱ��:150ms
void W25QXX_Erase_Sector(uint32_t Dst_Addr)   
{  
	//����falsh�������,������   
 	//printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
    W25QXX_Write_Enable();                  //SET WEL 	 
    W25QXX_Wait_Busy();   
  	W25QXX_CS_Enable;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_SectorErase);   //������������ָ�� 
    if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>24)); 
    }
    SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>16));  //����24bit��ַ    
    SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>8));   
    SPI1_ReadWriteByte((uint8_t)Dst_Addr);  
		W25QXX_CS_Disable;                            //ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				    //�ȴ��������
} 

//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
    W25QXX_Write_Enable();                  //SET WEL 
	W25QXX_CS_Enable;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_PageProgram);   //����дҳ����   
    if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>24)); 
    }
    SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>16)); //����24bit��ַ    
    SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>8));   
    SPI1_ReadWriteByte((uint8_t)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	W25QXX_CS_Disable;                            //ȡ��Ƭѡ 
	W25QXX_Wait_Busy();					   //�ȴ�д�����
}
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 

//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
uint8_t W25QXX_BUFFER[4096];		 
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)
		secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}
		else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}




