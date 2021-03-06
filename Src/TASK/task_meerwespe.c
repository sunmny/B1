#include "app_meerwespe.h"
#include "task_meerwespe.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"

#include "task_refresh.h"
#include "task_sensor.h"
#include "task_gps.h"
#include "task_lora.h"
#include "task_timer.h"
#include "task_lte.h"
#include "task_ble.h"
#include "task_bd1.h"

#include "app_power.h"
#include "app_ble.h"
#include "task_test.h"


#define WorkQueueLen 2
#define GpsQueueLen  2
#define LoRaQueueLen 2
#define LteQueueLen 2
#define BleQueueLen 16
#define mytestQueueLen 10

#define BleMailQueueLen 10

osThreadId mytestHandle;
osThreadId mytestHandle1;

//osMessageQId WorkQueueHandle;
//osMessageQId GpsQueueHandle;
//osMessageQId LoRaQueueHandle;
//osMessageQId LteQueueHandle;
//osMessageQId BleQueueHandle;
//osMailQId    BleMailQueueHandle;
//osMailQId    mytestQueueHandle;

osTimerId LteTimerHandle;
osTimerId BuzzerTimerHandle;
osTimerId PowerTimerHandle;
osTimerId LedTimerHandle;
osTimerId testTimerHandle;
osTimerId rdTimerHandle;


extern void LteTimerCallback(void const * argument);
extern void mytestTask1(void const * argument);
extern void testTimerCallback(void const * argument);
extern void SdTask(void const * argument);

extern void flashTask(void const * argument);

 TaskHandle_t bd1task = NULL;
 TaskHandle_t gpstask = NULL;
 TaskHandle_t bletask = NULL;
 TaskHandle_t ltetask = NULL;
  TaskHandle_t flashtask = NULL;
 static void prvTask1( void *pvParameters )
{
    for( ;; )
    {
    
       // xTaskNotifyGive( xTask2 );
 
			   printf("prvTask0 \r\n");
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
			
				printf("prvTask1 \r\n");
    }
}
 
static void prvTask2( void *pvParameters )
{
    for( ;; )
    {
			
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
				printf("prvTask2 \r\n");
        //xTaskNotifyGive( xTask1 );
    }
}



extern uint8_t gsn_buf[12];
extern uint8_t apn_buf[50];
//extern uint8_t phone_num_buf[11];

void task_init(void)
{

	get_nvram_sn(gsn_buf);
printf("sunmny gsn num is %s  \r\n",gsn_buf);
//get_nvram_id(phone_num_buf);
//memcpy(&apn_buf[13],phone_num_buf,11);
//printf("sunmny lte num is %s  \r\n",phone_num_buf);
	
	osTimerDef(LteTimer, LteTimerCallback);
	LteTimerHandle = osTimerCreate(osTimer(LteTimer), osTimerPeriodic, NULL);
	
  osTimerDef(BuzzerTimer, BuzzerTimerCallback);
  BuzzerTimerHandle = osTimerCreate(osTimer(BuzzerTimer), osTimerPeriodic, NULL);

  osTimerDef(PowerTimer, PowerTimerCallback);
  PowerTimerHandle = osTimerCreate(osTimer(PowerTimer), osTimerOnce, NULL);

    osTimerDef(LedTimer, LedTimerCallback);
  LedTimerHandle = osTimerCreate(osTimer(LedTimer), osTimerPeriodic, NULL);
	
	 osTimerDef(rdTimer, rdTimerCallback);
  rdTimerHandle = osTimerCreate(osTimer(rdTimer), osTimerOnce, NULL);
	
	   osTimerDef(testTimer, testTimerCallback);
  testTimerHandle = osTimerCreate(osTimer(testTimer), osTimerPeriodic, NULL);
	

		xTaskCreate( Bd1Task, "Task1",200, NULL, tskIDLE_PRIORITY, &bd1task );
   xTaskCreate(GpsTask, "Task2",512, NULL, tskIDLE_PRIORITY, &gpstask );
	 
	 	xTaskCreate( BleTask, "Task3",200, NULL, tskIDLE_PRIORITY, &bletask );
   xTaskCreate( LteTask, "Task4",200, NULL, tskIDLE_PRIORITY, &ltetask );
	 // xTaskCreate(flashTask, "Task5",200, NULL, tskIDLE_PRIORITY, &flashtask);

//	osThreadDef(my_test, mytestTask, osPriorityNormal, 0, 128);
 // mytestHandle = osThreadCreate(osThread(my_test), NULL);
	
		//osThreadDef(my_test1, mytestTask1, osPriorityNormal, 0, 128);
    //mytestHandle1 = osThreadCreate(osThread(my_test1), NULL);
		
	
  //osMessageQDef(WorkQueue, WorkQueueLen, uint16_t);
  //WorkQueueHandle = osMessageCreate(osMessageQ(WorkQueue), NULL);

  /* definition and creation of GPSQueue */
  //osMessageQDef(GpsQueue, GpsQueueLen, uint16_t);
  //GpsQueueHandle = osMessageCreate(osMessageQ(GpsQueue), NULL);

  /* definition and creation of LteQueue */
  //osMessageQDef(LteQueue, LteQueueLen, uint16_t);
 //LteQueueHandle = osMessageCreate(osMessageQ(LteQueue), NULL);

  /* definition and creation of BleUartQueue */
 // osMessageQDef(BleQueue, BleQueueLen, uint16_t);
 // BleQueueHandle = osMessageCreate(osMessageQ(BleQueue), NULL);


 // osMailQDef(BleMailQueue, BleMailQueueLen, blemail);
 // BleMailQueueHandle = osMailCreate(osMailQ(BleMailQueue), NULL);
	
	
	 // osMailQDef(mytestQueue, mytestQueueLen, mytestmail);
 // mytestQueueHandle = osMailCreate(osMailQ(mytestQueue), NULL);


//set_nvram_id_data(phone_num_buf1);

//set_nvram_id_data(phone_num_buf1);
//set_nvram_id_data(phone_num_buf1);


//	printf("lte timer \r\n");
	  osTimerStart(LteTimerHandle, 1000);//开始定时
	  //osTimerStart(PowerTimerHandle, 10000);//开始定时
	//	osTimerStart (testTimerHandle, 1000);//开始定时
		osTimerStart(LedTimerHandle, 50000);//开始定�
		
	//buzzer_timer_en(1);
	}


