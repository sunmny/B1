
#include "task_lora.h"
#include <stdint.h>
#include "hw_lora.h"
#include "string.h"
#include "delay.h"
#include "cmsis_os.h"
#include "app_meerwespe.h"
#include "app_lora.h"
#include "task_refresh.h"
#include "task_gps.h"
#include "app_meerwespe.h"
#include "app_lora.h"

extern osMessageQId LoRaQueueHandle;
//static uint8_t lora_timeout = 0;
extern WORK_MODE work_mode;
extern WORK_SEND_MODE work_send_mode;

extern uint8_t gps_in;

		 extern uint8_t gps_nub[4];
		 
//static uint16_t lora_queue_count = 0;


extern GPS_MODE gps_state;
LORA_MODE lora_state = lora_close;

/*
��ʽ��
Bit0��Latitude
Bit1��Longitude
Bit2��Altitude
Bit3��Accuracy
Bit4������
Bit5��SOS
Bit6:  ʱ��
Bit7:  ����&Gsensor&�Ƿ�λ
Bit8: �ϱ�����
Bit9: �¼�


   falg   -----ά��---------  -----����---------- -------����-------- -------��λ����---- ����  17    8   1    15   55   28       ����  ���
0x01,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0x11,0x08,0x01,0x0f,0x37,0x1c,0x02,0x00,0x01

00 1b     13 19 05 b9         46 b6 65 da         00 00 28 f0          4d 

*/
uint8_t aaaa5[5] = {0x00,0x30,0x4e,0x0d,0x0a};
uint8_t aaaa4[13] = {0x00,0x33,0x13,0x19,0x05,0xb9,0x46,0xb6,0x65,0xda,0x4e,0x0d,0x0a};
uint8_t aaaa6[13] = {0x00,0x13,0x13,0x19,0x05,0xb9,0x46,0xb6,0x65,0xda,0x4e,0x0d,0x0a};
uint8_t aaaa[17] = {0x00,0x1b,0x13,0x19,0x05,0xb9,0x46,0xb6,0x65,0xda,0x00,0x00,0x28,0xf0,0x4d,0x0d,0x0a};
uint8_t aaaa2[17] = {0x00,0x1b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x47,0x0d,0x0a};
uint8_t aaaa3[17] = {0x00,0x0b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x47,0x0d,0x0a};
uint8_t bb[10]="AT+ID=30\r\n";
uint8_t bb2[8]="AT+ID?\r\n";
uint8_t cc[10]="AT+NET=0\r\n";
uint8_t dd[8]="AT+NS?\r\n";
//--------------------------------------------------------------------------------------------------------------------------------
uint8_t aa[30]={0x01,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0x11,0x08,0x02,0x0a,0x1b,0x1e,0x02,0x00,0x01,0x0d,0x0a};
uint8_t ee[8]="AT+TX=0,";
uint8_t send[38]={0x00};

//static  WORK_SEND_MODE work_send_mode  =  no_send;
 WORK_SEND_MODE work_send_mode_huxi  =  no_send;

/*

	  no_send = 0,
	  call_send,
	  sos_send,
	  huxi_send,

*/

// ����  sos  ����
osStatus task_lora_message_put(uint32_t info)
{
  	osStatus  status;
	  switch(info)
		{
		  case call_send:
				 //�鿴gps��ǰ״̬����λ���򲻿���gps���鿴lora������״̬		

			     if((gps_state == gps_close)&&(lora_state == lora_close))
					 {		
            					 lora_state = lora_open;	
			                 work_send_mode = call_send;						 
			                 task_gps_message_put(gps_open);
			     }else{
					  					 lora_state = lora_open;	
			                 work_send_mode = call_send;
					 }

			 break;	
					 
			case sos_send:


			     if((gps_state == gps_close)&&(lora_state == lora_close))
					 {
						 								 lora_state = lora_open;
				                     work_send_mode = sos_send;
			                  task_gps_message_put(gps_open); 
					 }else{
					  								 lora_state = lora_open;
				                      work_send_mode = sos_send;
					 }

			 break;
					 
			case huxi_send:
				
				if(work_send_mode_huxi == no_send)
				{

					 if((gps_state == gps_close)&&(lora_state == lora_close))
					 {
						 					 lora_state = lora_open;
				   work_send_mode_huxi = huxi_send;
			     task_gps_message_put(gps_open); 
					 }else{
					 					 lora_state = lora_open;
				   work_send_mode_huxi = huxi_send;
					 }

					// osMessagePut (LoRaQueueHandle, work_send_mode_huxi, osWaitForever); //��Ӻ����¼�������
				}
				
			 break;
				
			default:
			
			  break;
		
		
		}
    status = osMessagePut(LoRaQueueHandle, info, 1000);
	  return status;
}

osEvent task_lora_message_get(void)
{
  return osMessageGet(LoRaQueueHandle, osWaitForever);
}



/* LedTask function */
void LoraTask(void const * argument)
{
		osEvent event;
  /* USER CODE BEGIN LedTask */
  /* Infinite loop */
	LORA_ERROR  ret = OK;
  for(;;)
  {
		
	  if((gps_state == gps_close)&&(( work_send_mode_huxi != no_send)||(work_send_mode != no_send)))
		{
	 //��ȡ����
 event = task_lora_message_get();
//osStatus osMessagePut (osMessageQId queue_id, uint32_t info, uint32_t millisec);
if(event.status == osEventMessage)
{
    //osDelay(1);
	//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
  //  osDelay(250);
	//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
		
		//����lora��Ϣ
//	lora_timeout = 0;
 //		gps_send,
 //	  lora_send,
	switch(event.value.v)
	{
			case lora_open://��ҳ
				
			if(work_send_mode != no_send)
			{
		    ret = app_lora_send((uint8_t)work_send_mode);
//		   lora_timeout = 0;
			 work_send_mode = no_send;
			if(work_send_mode_huxi == huxi_send)
			{
			  //�к�����Ҫ����
			}else
      {
		  	lora_state = lora_close;
				
	  	
			//�鿴�Ƿ�������״̬���ǣ������stop2
				if(work_mode == lcd_off)
				{
				task_refresh_message_put(work_mode);
				
				}
//					app_power_stop2_enter();
//         app_power_stop2_exit();
			}
			
				if((work_mode == sos_page)||(work_mode == call_page))
				{
				  //��ʾ�ɹ�����ʧ��
					if(ret == OK)
					{
					task_refresh_message_put(success);
					}else
					{
					task_refresh_message_put(error);
					}
				
				}
		}
			
    break;
	
    case huxi_send://����
			
       app_lora_send(huxi_send);
		
		   if(work_send_mode != no_send)
			{
			  //�б�����Ҫ����
			}else
      {
		  	lora_state = lora_close;
							//�鿴�Ƿ�������״̬���ǣ������stop2
				if(work_mode == lcd_off)
				{
				task_refresh_message_put(work_mode);
				
				}
			}

	  	 work_send_mode_huxi = no_send;
		    
      break;		
		
		default://���ض���
		 	//osMessagePut (WorkSendQueueHandle, event.value.v, osWaitForever);
		  break;
		
	}
		
	}
}
		
		osDelay(1000);
  }
  /* USER CODE END LedTask */
}




//�ɹ�
//void gui_send_success(void);

//ʧ��
//void gui_send_error(void);



void lora_send2(void)
{

  //	memcpy(send,ee,8);
		//memcpy(send+8,aa,30);
	//  memcpy(send+8,aaaa2,25);
	while(1)
	{
	  memcpy(send,ee,8);
		//memcpy(send+8,aa,30);
	  memcpy(send+8,aaaa4,13);
		hw_lora_send(bb, 10);
	  delay_ms(20);
		hw_lora_send(bb2, 8);
		delay_ms(20);
		hw_lora_send(cc, 10);
	  //osDelay(20000);
		delay_ms(15000);
		hw_lora_send(dd, 8);
		//osDelay(500);
		delay_ms(50);
		hw_lora_send(send, 21);
		
		delay_ms(30000);
		
		  	memcpy(send,ee,8);
		//memcpy(send+8,aa,30);
	  memcpy(send+8,aaaa5,5);
		hw_lora_send(bb, 10);
	  delay_ms(20);
		hw_lora_send(bb2, 8);
		delay_ms(20);
		hw_lora_send(cc, 10);
	  //osDelay(20000);
		delay_ms(15000);
		hw_lora_send(dd, 8);
		//osDelay(500);
		delay_ms(50);
		hw_lora_send(send, 13);
		delay_ms(30000);
		
				  	memcpy(send,ee,8);
		//memcpy(send+8,aa,30);
	  memcpy(send+8,aaaa6,13);
		hw_lora_send(bb, 10);
	  delay_ms(20);
		hw_lora_send(bb2, 8);
		delay_ms(20);
		hw_lora_send(cc, 10);
	  //osDelay(20000);
		delay_ms(15000);
		hw_lora_send(dd, 8);
		//osDelay(500);
		delay_ms(50);
		hw_lora_send(send, 21);
		delay_ms(30000);
   
	}
}
	
	void lora_send3(void)
{

	
	uint8_t rec1[10] ={0x00};
//		uint8_t rec2[10] ={0x00};
//			uint8_t rec3[10] ={0x00};
//			uint8_t rec4[10] ={0x00};

	  memcpy(send,ee,8);
			
		
		//memcpy(send+8,aa,30);
///			hw_lora_receive(rec1,6);
	  memcpy(send+8,aaaa4,13);
		
		hw_lora_send(bb, 10);
				/*
			hw_lora_receive(rec1,6);
			
			//hw_lora_receive(rec4,1);
			
	delay_ms(20);
		hw_lora_send(bb2, 8);
			hw_lora_receive(rec2,10);
	//	delay_ms(20);
		hw_lora_send(cc, 10);
			hw_lora_receive(rec3,6);
			delay_ms(20);
	  //osDelay(20000);
		//delay_ms(15000);
		hw_lora_send(dd, 8);
			delay_ms(20);
		//osDelay(500);
		delay_ms(50);
		hw_lora_send(send, 21);
		
*/
	
	
}

