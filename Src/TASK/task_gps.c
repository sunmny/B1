
#include "task_gps.h"
#include <stdint.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "task_meerwespe.h"
#include "app_meerwespe.h"
#include "app_gps.h"
#include "hw_bd2.h"
#include "app_power.h"

#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "app_bd2.h"



extern WORK_SEND_MODE  work_send_mode;
extern uint8_t first_flag;

extern osMessageQId GpsQueueHandle;
extern osMessageQId LoRaQueueHandle;

extern uint8_t wakeup_init_flag;

GPS_MODE gps_state = gps_close;
extern  WORK_SEND_MODE work_send_mode_huxi ;
extern  UART_HandleTypeDef huart1;
extern GpsLocation gpslocation;

osStatus task_gps_message_put(uint32_t info)
{
  	osStatus  status = osOK;
	 if(gps_state == gps_close)
	 {
		 //hw_gps_open();
		 	  gps_state = gps_open;
    status = osMessagePut(GpsQueueHandle, info, 5000);

	 }else
	 {
	   //查看当前发送状态
	   status = osMessagePut(GpsQueueHandle, info, 5000);
	 
	 }
	  return status;
}

osEvent task_gps_message_get(void)
{
  return osMessageGet(GpsQueueHandle,osWaitForever);
}






/* GpsTask function */
uint8_t report_data[128]={0};
uint8_t report_data1[200]={0};
extern uint8_t dis_data[10];
extern osTimerId LteTimerHandle;
static uint8_t len ;
 static uint8_t pack_head_andid[11] = "+BINFOBEAT:";
extern uint8_t gsn_buf[12];
extern uint8_t response_location[128];
extern uint8_t bd_totlelen;
void set_local_data(void)
{
		//	uint8_t i =0;
	///	uint8_t temp1,temp2;
	//	int ret;
	//	uint8_t pucData[32];
		//unsigned int uiDataLen,puicDataLen=30;
		//uint8_t pucCData[64];
	get_zdevice_location_response();
	memcpy(report_data1,response_location,bd_totlelen);
	#if 0
		memcpy(report_data,pack_head_andid,11);
		memcpy(&report_data[11],gsn_buf,12);
		report_data[23] = ',';
		memcpy(&report_data[24],response_location,48);
		report_data[72] = ',';
		report_data[73] = '0';
		report_data[74] = ',';
		report_data[75] = '3';
		memcpy(&report_data[76],"##\n",3);
	
		//memcpy(pucData,report_data,16);
	//	uiDataLen= 16;
	//char pucData[16] = {0x26,0x11,0x0D,0x21,0x2E,0x25,0x00,0x00,0x79,0x27,0x48,0x63,0x29,0x5A,0x00,0x00};
	//	ret = TF_EncData((unsigned char *)pucData,uiDataLen,pucCData,&puicDataLen);
		//printf("jiami ret = %d \r\n",ret);

		memcpy(report_data1,pucCData,30);
	
		memcpy(pucData,&report_data[16],16);
		uiDataLen= 16;
		//TF_EncData((unsigned char *)pucData,uiDataLen,pucCData,&puicDataLen);
		memcpy(&report_data1[30],pucCData,30);
		
		memcpy(pucData,&report_data[32],16);
		uiDataLen= 16;
		//TF_EncData((unsigned char *)pucData,uiDataLen,pucCData,&puicDataLen);
		memcpy(&report_data1[60],pucCData,30);
		
		
		memcpy(pucData,&report_data[48],16);
		uiDataLen= 16;
		//TF_EncData((unsigned char *)pucData,uiDataLen,pucCData,&puicDataLen);
		memcpy(&report_data1[90],pucCData,30);
		
		
			memcpy(pucData,&report_data[64],16);
		uiDataLen= 16;
		//TF_EncData((unsigned char *)pucData,uiDataLen,pucCData,&puicDataLen);
		memcpy(&report_data1[120],pucCData,30);
		
		#endif
	
}
void GpsTask(void const * argument)
{

	uint8_t len;
	printf("GpsTask\r\n");
	
  for(;;)
  {
		//printf("GpsTask ---1\r\n");
		//osDelay(1000);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
			//printf("GpsTask\r\n");
		rn_receive_and_handle();
	}

	 

}
