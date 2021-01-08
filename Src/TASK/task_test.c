
#include "cmsis_os.h"
#include "app_ble.h"
#include <stdint.h>
#include "task_meerwespe.h"


extern  UART_HandleTypeDef huart1;
extern osStatus task_lte_message_put(uint32_t info);
extern osStatus osTimerStop (osTimerId timer_id);
extern osTimerId rdTimerHandle;
extern osTimerId testTimerHandle;
extern osTimerId LedTimerHandle;

extern uint8_t tcp_connect_flag =0;

osStatus task_test_message_put(uint32_t info)
{
  	osStatus  status = osOK;
	
	 
	return status;
}

osEvent task_test_message_get(void)
{
  
}

osStatus task_test_mail_put(uint8_t *data, uint16_t len)
{
	
}

osEvent task_test_mail_get(void)
{

}

/* SensorTask function */

uint8_t lte_network_flag =0;

uint8_t lte_init_flag =0;

extern uint8_t phone_num_buf[11];
void mytestTask(void const * argument)
{
	osEvent event;
	blemail *pblemail;
	
	uint32_t bd_receiver_num=136166;
	
  /* USER CODE BEGIN GPSTask */
  /* Infinite loop */
  for(;;)
  {
			//printf("mytestTask ---5\r\n");
	
			event = task_test_mail_get();
			pblemail=event.value.p;
			if(memcmp(pblemail->data,"stoprdtimer",11) == 0){
							osTimerStop(rdTimerHandle);		
						printf("rd timer stop \r\n");
			}else if(memcmp(pblemail->data,"stoptesttimer",13) == 0){
						osTimerStop(testTimerHandle);	
			
			}else if(memcmp(pblemail->data,"stopledtimer",12) == 0){
						osTimerStop(LedTimerHandle);	
			}else if(memcmp(pblemail->data,"nosim",5) == 0){
						 printf("sim is on insterted\r\n");
							lte_network_flag =0;
			}if(memcmp(pblemail->data,"insim",5) == 0){
						 printf("sim is insterted\r\n");
							lte_network_flag =1;
							//lte_init_network();
			}else if(memcmp(pblemail->data,"nobd1",5) == 0){
						 printf("bd1 card no in \r\n");
							set_rd_led_red();
					//	osTimerStop(PowerTimerHandle);
				hw_bd1_close();
					//	send_bd1_get_card_num();
			}else if(memcmp(pblemail->data,"inbd1",5) == 0){
						set_rd_led_red_close();
				//osTimerStop(PowerTimerHandle);
				hw_bd1_close();
						 printf("bd1 card in \r\n");
						//rd_send_msg(bd_receiver_num,"sunmny",6);
			}else if(memcmp(pblemail->data,"connect",7) == 0){
				printf("lte connect \r\n");
			//	lte_init_network();
				
			
				lte_init_flag =1;
						//rd_send_msg(bd_receiver_num,"sunmny",6);
			}else if(memcmp(pblemail->data,"tcpconnect",10) == 0){
				printf("lte tcp connect \r\n");
			//	lte_tcp_connect_network();
			//	lte_tcp_send_data();
			}else if(memcmp(pblemail->data,"ltesend",7) == 0){
					//set_local_data();
					// osTimerStart(PowerTimerHandle, 10000);
				printf("lte ltesend \r\n");
						//rd_send_msg(bd_receiver_num,"sunmny",6);
			}else if(memcmp(pblemail->data,"tcpconnectagain",15)){
							printf("tcp connect again99\r\n");
							//lte_close_connect_network();
							//lte_open_connect_network();
						//	lte_tcp_connect_network();
							//lte_tcp_send_data();
					//	lte_tcp_connect_network_again();
			}else if(memcmp(pblemail->data,"tcpsend",7) == 0){
				//	set_local_data();
					//osTimerStart(PowerTimerHandle, 10000);
				printf("lte ltesend \r\n");
						//rd_send_msg(bd_receiver_num,"sunmny",6);
			}
		
			 // osMailFree(mytestQueueHandle, pblemail); 

	

  }
  /* USER CODE END GPSTask */
}
#if 1
//extern uint32_t wwdog_delay;
extern  IWDG_HandleTypeDef   IwdgHandle;
extern uint8_t binded_flag;
 //extern uint32_t TimeoutCalculation(uint32_t timevalue);

extern uint8_t tempbuf[11];
uint8_t save_flag =0;
void mytestTask1(void const * argument)
{
	while(1){
		
				if(phone_num_buf[0]!='1' && tempbuf[0] == '1'){
				if(save_flag ==0){
					set_nvram_id_data(phone_num_buf);
					osDelay(500);
					set_nvram_id_data(phone_num_buf);
					save_flag =1;
					get_nvram_id(phone_num_buf);
				  }
				}
	
	}
	#if 0
	app_nfc_init();
	 IwdgHandle.Instance = IWDG;
  IwdgHandle.Init.Prescaler = IWDG_PRESCALER_256;
  IwdgHandle.Init.Reload = 2048;
  IwdgHandle.Init.Window = IWDG_WINDOW_DISABLE;

  if(HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
  {
    /* Initialization Error */
    //Error_Handler();
  }

	
	while(1){
	//	printf("mytestTask1 ---6\r\n");
	//	HAL_Delay(8000);
	//	app_test();
		osDelay(500);
		 if(HAL_IWDG_Refresh(&IwdgHandle) != HAL_OK)
    {
      
    }
	}
	
	#endif
}
#endif