/*************************************************************/
//2014.07.15�޸İ�
/*************************************************************/
#include "fm175xx.h"
#include "type_a.h"
#include "hw_nfc.h"
#include "hw_uart.h"

struct PICC_A_STR PICC_A;   
/****************************************************************************************/
/*���ƣ�TypeA_Request																	*/
/*���ܣ�TypeA_Request��ƬѰ��															*/
/*���룺																				*/
/*       			    			     												*/
/*	       								 												*/
/*�����																			 	*/
/*	       	pTagType[0],pTagType[1] =ATQA                                         		*/					
/*       	OK: Ӧ����ȷ                                                              	*/
/*	 		ERROR: Ӧ�����																*/
/****************************************************************************************/
unsigned char TypeA_Request(unsigned char *pTagType)
{
	unsigned char result,send_buff[1],rece_buff[2];
	unsigned int rece_bitlen;  
	Clear_BitMask(TxModeReg,0x80);//�ر�TX CRC
	Clear_BitMask(RxModeReg,0x80);//�ر�RX CRC
	Set_BitMask(RxModeReg, 0x08);//�ر�λ����
	Clear_BitMask(Status2Reg,0x08);
	Write_Reg(BitFramingReg,0x07);
	send_buff[0] = 0x26;
  Pcd_SetTimer(1);
	result = Pcd_Comm(Transceive,send_buff,1,rece_buff,&rece_bitlen);
	if ((result == OK) && (rece_bitlen == 2*8))
	{    
		*pTagType     = rece_buff[0];
		*(pTagType+1) = rece_buff[1];
	}
 	return result;
}
/****************************************************************************************/
/*���ƣ�TypeA_WakeUp																	*/
/*���ܣ�TypeA_WakeUp��ƬѰ��															*/
/*���룺																				*/
/*       			    			     												*/
/*	       								 												*/
/*�����																			 	*/
/*	       	pTagType[0],pTagType[1] =ATQA                                         		*/					
/*       	OK: Ӧ����ȷ                                                              	*/
/*	 		ERROR: Ӧ�����																*/
/****************************************************************************************/
unsigned char TypeA_WakeUp(unsigned char *pTagType)
{
	unsigned char result,send_buff[1],rece_buff[2];
	unsigned int rece_bitlen;  
	Clear_BitMask(TxModeReg,0x80);//�ر�TX CRC
	Clear_BitMask(RxModeReg,0x80);//�ر�RX CRC
	Set_BitMask(RxModeReg, 0x08);//�ر�λ����
	Clear_BitMask(Status2Reg,0x08);
	Write_Reg(BitFramingReg,0x07);
 	send_buff[0] = 0x52;
   	Pcd_SetTimer(1);
	result = Pcd_Comm(Transceive,send_buff,1,rece_buff,&rece_bitlen);

	if ((result == OK) && (rece_bitlen == 2*8))
	{    
		*pTagType     = rece_buff[0];
		*(pTagType+1) = rece_buff[1];
	}
	return result;
}
/****************************************************************************************/
/*���ƣ�TypeA_Anticollision																*/
/*���ܣ�TypeA_Anticollision��Ƭ����ͻ													*/
/*���룺selcode =0x93��0x95��0x97														*/
/*       			    			     												*/
/*	       								 												*/
/*�����																			 	*/
/*	       	pSnr[0],pSnr[1],pSnr[2],pSnr[3]pSnr[4] =UID                            		*/					
/*       	OK: Ӧ����ȷ                         o'p'l                                     	*/
/*	 		ERROR: Ӧ�����																*/
/****************************************************************************************/
 unsigned char TypeA_Anticollision(unsigned char selcode,unsigned char *pSnr)
{
    unsigned char result,i,send_buff[2],rece_buff[5];
    unsigned int rece_bitlen;
	Clear_BitMask(TxModeReg,0x80);
	Clear_BitMask(RxModeReg,0x80);
    Clear_BitMask(Status2Reg,0x08);
    Write_Reg(BitFramingReg,0x00);
    Clear_BitMask(CollReg,0x80);
 
    send_buff[0] = selcode;
    send_buff[1] = 0x20;
    Pcd_SetTimer(1);
    result = Pcd_Comm(Transceive,send_buff,2,rece_buff,&rece_bitlen);

    if (result == OK)
    {
    	 for (i=0; i<5; i++)
         	*(pSnr+i)  = rece_buff[i];
         if (pSnr[4] != (pSnr[0]^pSnr[1]^pSnr[2]^pSnr[3]))
    		result = ERROR;    
    }
  return result;
}
/****************************************************************************************/
/*���ƣ�TypeA_Select																	*/
/*���ܣ�TypeA_Select��Ƭѡ��															*/
/*���룺selcode =0x93��0x95��0x97														*/
/*      pSnr[0],pSnr[1],pSnr[2],pSnr[3]pSnr[4] =UID 			    			     	*/
/*	       								 												*/
/*�����																			 	*/
/*	       	pSak[0],pSak[1],pSak[2] =SAK			                            		*/					
/*       	OK: Ӧ����ȷ                                                              	*/
/*	 		ERROR: Ӧ�����																*/
/****************************************************************************************/
unsigned char TypeA_Select(unsigned char selcode,unsigned char *pSnr,unsigned char *pSak)
{
    unsigned char result,i,send_buff[7],rece_buff[5];
    unsigned int rece_bitlen;
	Write_Reg(BitFramingReg,0x00);
  	Set_BitMask(TxModeReg,0x80);
	Set_BitMask(RxModeReg,0x80);
    Clear_BitMask(Status2Reg,0x08);
	
	send_buff[0] = selcode;
    send_buff[1] = 0x70;
    
    for (i=0; i<5; i++)
       	send_buff[i+2] = *(pSnr+i);
    
   	Pcd_SetTimer(1);
    result = Pcd_Comm(Transceive,send_buff,7,rece_buff,&rece_bitlen);
    if (result == OK)
    	*pSak=rece_buff[0]; 
	return result;
}
/****************************************************************************************/
/*���ƣ�TypeA_Halt																		*/
/*���ܣ�TypeA_Halt��Ƭֹͣ																*/
/*���룺																				*/
/*       			    			     												*/
/*	       								 												*/
/*�����																			 	*/
/*	       											                            		*/					
/*       	OK: Ӧ����ȷ                                                              	*/
/*	 		ERROR: Ӧ�����																*/
/****************************************************************************************/
unsigned char TypeA_Halt(void)
{
    unsigned char result,send_buff[2],rece_buff[1];
	unsigned int rece_bitlen;
    send_buff[0] = 0x50;
    send_buff[1] = 0x00;
   
   	Write_Reg(BitFramingReg,0x00);
  	Set_BitMask(TxModeReg,0x80);
	Set_BitMask(RxModeReg,0x80);
    Clear_BitMask(Status2Reg,0x08);
	Pcd_SetTimer(1);
    result = Pcd_Comm(Transmit,send_buff,2,rece_buff,&rece_bitlen);
    return result;
}
/****************************************************************************************/
/*���ƣ�TypeA_CardActivate																*/
/*���ܣ�TypeA_CardActivate��Ƭ����														*/
/*���룺																				*/
/*       			    			     												*/
/*	       								 												*/
/*�����	pTagType[0],pTagType[1] =ATQA 											 	*/
/*	       	pSnr[0],pSnr[1],pSnr[2],pSnr[3]pSnr[4] =UID 		                   		*/
/*	       	pSak[0],pSak[1],pSak[2] =SAK			                            		*/					
/*       	OK: Ӧ����ȷ                                                              	*/
/*	 		ERROR: Ӧ�����																*/
/****************************************************************************************/
unsigned char TypeA_CardActivate(struct PICC_A_STR *type_a_card)
{
unsigned char result;
uint8_t i=0;
	
		result=TypeA_Request(type_a_card->ATQA);//Ѱ�� Standard	 send request command Standard mode
		if (result==ERROR)
			return ERROR;
		printf("result ok ATQA=0x%x 0x%x\r\n",type_a_card->ATQA[0],type_a_card->ATQA[1]);	
    //print_msg("type_a_card->ATQA[0]&0xC0= 0x%x\r\n",type_a_card->ATQA[0]&0xC0);	
	//print_msg("UID = ");
	//for(i = 0;i<12;i++)
  //{
  //  print_msg(" 0x%x",type_a_card->UID[i]);
  //}
  //print_msg("\r\n");		
			if 	((type_a_card->ATQA[0]&0xC0)==0x00)	//һ��UID
				{
				result=TypeA_Anticollision(0x93,type_a_card->UID);//1������ͻ
				//print_msg("TypeA_Anticollision= 0x%x\r\n",result);		
				if (result==ERROR)
					return ERROR;
				result=TypeA_Select(0x93,type_a_card->UID,&type_a_card->SAK);//1��ѡ��
				//print_msg("TypeA_Select= 0x%x\r\n",result);	
				if (result==ERROR)
					return ERROR;
				type_a_card->UID_len=4;
				}
	//print_msg("TypeA_Select ok\r\n");
			if 	((type_a_card->ATQA[0]&0xC0)==0x40)	//����UID
				{
				
				result=TypeA_Anticollision(0x93,type_a_card->UID);//1������ͻ
				if (result==ERROR)
					return ERROR;
					
				result=TypeA_Select(0x93,type_a_card->UID,&type_a_card->SAK);//1��ѡ��
				if (result==ERROR)
					return ERROR;
				
				result=TypeA_Anticollision(0x95,type_a_card->UID+4);//2������ͻ
				if (result==ERROR)
					return ERROR;
				
				result=TypeA_Select(0x95,type_a_card->UID+4,&type_a_card->SAK);//2��ѡ��
				if (result==ERROR)
					return ERROR;
				type_a_card->UID_len=8;
				}

			if 	((type_a_card->ATQA[0]&0xC0)==0x80)	//����UID
				{
				result=TypeA_Anticollision(0x93,type_a_card->UID);//1������ͻ
				if (result==ERROR)
					return ERROR;
				
				result=TypeA_Select(0x93,type_a_card->UID,&type_a_card->SAK);//1��ѡ��
				if (result==ERROR)
					return ERROR;
				
				result=TypeA_Anticollision(0x95,type_a_card->UID+4);//2������ͻ
				if (result==ERROR)
					return ERROR;
				
				result=TypeA_Select(0x95,type_a_card->UID+4,&type_a_card->SAK);//2��ѡ��
				if (result==ERROR)
					return ERROR;
					
				result=TypeA_Anticollision(0x97,type_a_card->UID+8);//3������ͻ
				if (result==ERROR)
					return ERROR;
					
				result=TypeA_Select(0x97,type_a_card->UID+8,&type_a_card->SAK);//3��ѡ��
				if (result==ERROR)
					return ERROR;
				type_a_card->UID_len=12;
				}
		return result;
}
