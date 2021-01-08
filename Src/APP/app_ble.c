#include "hw_uart.h" 
#include "stm32l4xx_hal.h"
//#include "cmsis_os.h"
#include "string.h"
//#include "delay.h"
#include "app_ble.h"
#include "hw_config.h"
#include "task_ble.h"
#include "hw_buzzer.h"
#include "zb_common_data.h"
#include "hw_bd1.h"
#include "hw_bd2.h"
#include "app_nvram.h"
#include "app_nvram.h"
//#include "stdio.h"
//#include "math.h"

uint8_t binded_flag =0;
uint8_t ble_uart_data_loop[BLE_UART_DATA_LOOP_SIZE+1];

uint16_t ble_uart_data_loop_len;

extern osTimerId BuzzerTimerHandle;
extern osTimerId testTimerHandle;
extern osTimerId rdTimerHandle;
extern UART_HandleTypeDef huart2;
uint8_t ble_send_response(uint8_t *data, uint16_t len);
extern uint8_t rd_num[6];
uint16_t buzzer_alert_counter;
extern uint8_t rd_power_buf[50];
extern uint8_t rd_rece_buf[50];
extern void aw9523_set_light(uint8_t count,uint8_t val);
extern osTimerId LedTimerHandle;
extern osStatus task_gps_message_put(uint32_t info);
extern uint8_t hw_uart2_send(uint8_t *tx_buf, uint16_t tx_len);

#define AT_END_TOK_LEN 4
#define DEVICE_ID_LEN 15
#define SOS_MESSAGE_MAX_LEN 256
static const uint8_t ble_at_end_tok[AT_END_TOK_LEN] = {0x23,0x23,0x0d,0x0a};//##\r\n
static const uint8_t ble_at_end[2] = {0x0d,0x0a};//##\r\n
static uint8_t isAlerting=0;
uint8_t bind_ok_flag =0;
uint8_t rd_buf[16] = {0x24,0x43,0x43,0x49,0x43,0x41,0x2C,0x30,0x2C,0x30,0x30,0x2A,0x37,0x42,0x0D,0x0A};
struct zdevice_setting{
					uint8_t task_id;
					uint8_t bdev_num;
					uint8_t zdev_is_set;
					uint8_t zdev_num;
						
					//struct zdevice_bbind_bdev bbinddev;
};
struct zdevice_setting zdev;
struct bdevice_status ft_dev;
static uint8_t rd_test_flag =0;

uint8_t ble_uart_at_handle_poweroff(uint8_t idx);
uint8_t ble_uart_at_handle_bpdismid(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_bpdisoid(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_rdsspower(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_rnsspower(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_airplanemode(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_bbroadcast(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_bcall(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_bbind(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_bunbind(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_bbeat(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_binfowrite(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_bbbeat(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_blight(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_binfoerase(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_centeraddress(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_bsosinfo(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_bsos(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_selfcheck(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_bdistance(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_blowbattery(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_breportinfotime(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_taskgoon(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_bboxunlock(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_bpalert(uint8_t *data, uint16_t len, uint8_t at_index);

uint8_t ble_uart_at_handle_lbbind(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_lubind(uint8_t *data, uint16_t len, uint8_t idx);

uint8_t get_zdevice_num(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t get_report_message_id(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t get_report_timer(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t get_report_port(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t get_rn_num(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t get_rd_num(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t get_other_zdev_id(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t get_zdev_id(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t get_report_ipaddr(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t get_report_apn(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t get_report_usr(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t get_report_pws(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t get_report_bid(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_uart_at_handle_olock(uint8_t *data, uint16_t len, uint8_t idx);


/***********************test mode*************************/
uint8_t ble_uart_at_handle_rd(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_rn(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_sd(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_gprs(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_power_key(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_sos_key(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_charger_key(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_light(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_nfc(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_battery(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_sensor_ps(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_hall(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_uart_at_handle_lock(uint8_t *data, uint16_t len, uint8_t at_index);

uint8_t ble_uart_at_handle_nfc(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_moto_s(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_moto_z(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_moto_c(uint8_t *data, uint16_t len, uint8_t idx);
uint8_t ble_uart_at_handle_lcd(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_uart_at_handle_lcd_key(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_uart_at_handle_write_sn(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_uart_at_handle_reset_back_data(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_uart_at_handle_ble_getmid(uint8_t *data, uint16_t len, uint8_t at_index);
uint8_t ble_uart_at_handle_ble_connect(uint8_t *data, uint16_t len, uint8_t at_index);
extern void set_moto_work(uint8_t enable);
	
	uint8_t ble_uart_at_handle_batt(uint8_t *data, uint16_t len, uint8_t at_index);
static const ble_at_symbol ble_uart_at_table[13] = {
	{ble_uart_at_handle_bpdisoid, (uint8_t *)"BPDISOID:"},
	{ble_uart_at_handle_bpdismid, (uint8_t *)"BPDISMID:"},
	{ble_uart_at_handle_poweroff, (uint8_t *)"POWEROFF"},
	{ble_uart_at_handle_bbind, (uint8_t *)"bbind="},
	{ble_uart_at_handle_olock, (uint8_t *)"olock:"},
	{ble_uart_at_handle_bunbind, (uint8_t *)"AT+BUNBIND="},
	{ble_uart_at_handle_lbbind, (uint8_t *)"lbbind:"},
	{ble_uart_at_handle_lubind, (uint8_t *)"lubind:"},
	{ble_uart_at_handle_taskgoon, (uint8_t *)"TASKGOON"},
	{ble_uart_at_handle_ble_connect, (uint8_t *)"FTCN##"},
	{ble_uart_at_handle_ble_getmid, (uint8_t *)"GMID##"},
	{ble_uart_at_handle_write_sn, (uint8_t *)"FTSN"},
	{ble_uart_at_handle_reset_back_data, (uint8_t *)"FTXX##"},
};






uint32_t ti_is_work_flag =0;

uint8_t ble_uart_at_handle_batt(uint8_t *data, uint16_t len, uint8_t at_index)
{
	
	//if(0==memcpy(data,"batt:ok",7))
		//	ti_is_work_flag++;
}


extern osStatus task_test_mail_put(uint8_t *data, uint16_t len);
uint8_t rn_send_flag =0xff;
uint8_t rd_send_flag =0xff;
uint8_t testtimercount = 0;
void testTimerCallback(void const * argument)
{
	
}

void test(uint8_t *buf,uint8_t len)
{
		
			
}

uint8_t batt_soc[10];
extern uint8_t bat_rsoc;
extern uint8_t lte_network_flag;
extern uint8_t lte_init_flag;
extern void read_ps_data(uint8_t *val);
uint8_t ti_reset_flag =0;
uint8_t data_lock[2];
extern uint8_t response_location[128];
extern uint8_t init_lte_flag;
uint32_t rd_send_count = 0;
uint32_t bd_num = 220215;
extern uint8_t bd_totlelen;
extern uint8_t send_failed_count;
extern uint8_t phone_num_buf[11];
void LedTimerCallback(void const * argument)
{
		printf("LedTimerCallback \r\n");
	
	
		
			
	      printf("lte response_location = %s \r\n",response_location);
			if(bind_ok_flag){
			//if(1) {
				if(init_lte_flag ==0){
				
					lte_init_network();
					init_lte_flag = 1;
				}else{
	
	     // if((response_location[0] == '+' && response_location[1] == 'B')){
					printf("init lte and connect\r\n");
				lte_open_connect_network();
				lte_tcp_connect_network();
				printf("lte send data before\r\n");
				lte_tcp_send_data();
				printf("lte send data after \r\n");
				lte_close_connect_network();
				//}
				}
				
				if(send_failed_count >2){
						rd_send_msg(bd_num,response_location,bd_totlelen);
					  send_failed_count =0;
					if(send_failed_count > 250)
					  send_failed_count =0;
				   }
				
				
			}
		
				memcpy(batt_soc,"batt:",5);
				set_val_to_report();
				//if(bat_rsoc ==4){
					batt_soc[5]= bat_rsoc + '0';			
				//}else if(bat_rsoc ==3){
				//	batt_soc[5]= bat_rsoc + '0';	
				//}else if(bat_rsoc ==2){
				//	batt_soc[5]= bat_rsoc + '0';	
				//}else if(bat_rsoc ==1){
					//batt_soc[5]= bat_rsoc + '0';	
			//	}else if(bat_rsoc ==0){
					//batt_soc[5]= bat_rsoc + '0';	
				//}
				batt_soc[6]=  ',';
				if(ft_dev.rn_flags == 1){
					batt_soc[7]=  '1';
				}else{
					batt_soc[7]=  '0';
				}
					batt_soc[8]=  ',';
				printf("read ps data\r\n");
				read_ps_data(data_lock);
					if(data_lock[0] >0 || data_lock[1] > 0){
					printf("lock error\r\n");
						batt_soc[9]=  '1';
					}else{
						batt_soc[9]=  '0';
					}
				//ti_reset_flag++;
				
				hw_uart4_send(batt_soc,10);
					printf("set local data ok\r\n");
}

void send_bd1_get_card_num(void)
{
		//hw_uart2_send(rd_buf,16);
	HAL_UART_Transmit(&huart2,rd_buf,16, 0xFFFF);
}
void rdTimerCallback(void const * argument)
{
	
	set_moto_work(2);
	printf("rdTimerCallback \r\n");
	
	
}
	
uint8_t ble_uart_at_handle_rd(uint8_t *data, uint16_t len, uint8_t idx)
{
				printf("rd test \r\n");
			ft_dev.rd_flags =0;
		

	
}
/* ************定位 rn××××××××××××××××××××××*/

uint8_t ble_uart_at_handle_rn(uint8_t *data, uint16_t len, uint8_t idx)
{	
		
}
uint8_t ble_uart_at_handle_sd(uint8_t *data, uint16_t len, uint8_t idx)
{
			
			
}
uint8_t ble_uart_at_handle_gprs(uint8_t *data, uint16_t len, uint8_t idx)
{
		

}
uint8_t ble_uart_at_handle_power_key(uint8_t *data, uint16_t len, uint8_t idx)
{
				
	
}
uint8_t ble_uart_at_handle_sos_key(uint8_t *data, uint16_t len, uint8_t idx)
{
			
									
}
uint8_t ble_uart_at_handle_charger_key(uint8_t *data, uint16_t len, uint8_t idx)
{

}
uint8_t ble_uart_at_handle_light(uint8_t *data, uint16_t len, uint8_t idx)
{
			

}
uint8_t ble_uart_at_handle_nfc(uint8_t *data, uint16_t len, uint8_t idx)
{
			

}


extern void zdev_read_battery_soc(void);
extern uint8_t bat_zdev_soc;
uint8_t ble_uart_at_handle_battery(uint8_t *data, uint16_t len, uint8_t idx)
{
		
		
}
extern void read_ps_data(uint8_t *val);
uint8_t ble_uart_at_handle_sensor_ps(uint8_t *data, uint16_t len, uint8_t idx)
{
		
}
uint8_t ble_uart_at_handle_hall(uint8_t *data, uint16_t len, uint8_t at_index)
{
			
}
uint8_t ble_uart_at_handle_lock(uint8_t *data, uint16_t len, uint8_t at_index)
{
			

}



uint8_t ble_uart_at_handle_moto_s(uint8_t *data, uint16_t len, uint8_t idx)
{
		uint8_t enable =0; // 0 or 1 ,2 close;
	
		 set_moto_work(0);
		
}

uint8_t ble_uart_at_handle_moto_z(uint8_t *data, uint16_t len, uint8_t idx)
{
		uint8_t enable =0; // 0 or 1 ,2 close;
	
		 set_moto_work(1);
		
}
uint8_t ble_uart_at_handle_moto_c(uint8_t *data, uint16_t len, uint8_t idx)
{
		uint8_t enable =0; // 0 or 1 ,2 close;
	
		 set_moto_work(2);
		
}
uint8_t ble_uart_at_handle_lcd(uint8_t *data, uint16_t len, uint8_t at_index)
{


}
uint8_t ble_uart_at_handle_lcd_key(uint8_t *data, uint16_t len, uint8_t at_index)
{

}
uint8_t gsn_buf[12] ={0};
uint8_t ble_uart_at_handle_ble_getmid(uint8_t *data, uint16_t len, uint8_t at_index)
{
		
		uint8_t send_buf[20];
		
		get_nvram_sn(gsn_buf);
		memcpy(send_buf,"GMID",4);
		memcpy(&send_buf[4],gsn_buf,12);
		memcpy(&send_buf[16],"##",2);
		ble_send_response(send_buf,18);

}
uint8_t ble_uart_at_handle_write_sn(uint8_t *data, uint16_t len, uint8_t at_index)
{
		uint8_t sn_buf[12] = "112233445566",buf[10];
		uint8_t i;
		
		memcpy(sn_buf,&data[4],12);
		set_nvram_sn_data(sn_buf);
		memset(sn_buf,0x00,12);
		osDelay(1000);
		get_nvram_sn(sn_buf);
	  for(i =0;i<3;i++){
		if(memcmp(sn_buf,&data[4],12)!= 0){
		
			memcpy(sn_buf,&data[4],12);
			set_nvram_sn_data(sn_buf);
			memset(sn_buf,0x00,12);
			osDelay(1000);
			get_nvram_sn(sn_buf);
		}else{
					
			memcpy(buf,"FTSNOK",6);
			memcpy(&buf[6],ble_at_end_tok,4);
			ble_send_response(buf,10);
			break;
		}
		printf("sn_buf = %s \r\n",sn_buf);
	}
			if((i ==2)&&(memcmp(sn_buf,&data[4],12)!= 0)){
				memcpy(buf,"FTSNERR",7);
			memcpy(&buf[7],ble_at_end_tok,4);
			ble_send_response(buf,11);
			}
		
}
	extern void aw9523_init(void);
uint8_t ble_uart_at_handle_ble_connect(uint8_t *data, uint16_t len, uint8_t at_index)
{
		printf("ble_conect \r\n");
		aw9523_init();
	//	osTimerStart (LedTimerHandle, 1000);

}
uint8_t ble_uart_at_handle_reset_back_data(uint8_t *data, uint16_t len, uint8_t at_index)
{
			uint8_t buf[10];
		
			 memcpy(buf,"FTXX",4);
			 memcpy(&buf[4],ble_at_end_tok,4);
			 ble_send_response(buf,8);


}

/*todo, 20181115*/
void get_bdevice_id(uint8_t *data)
{

}

void add_separator_comma(uint8_t *data)
{
		*data=',';
}

uint8_t ble_send_response(uint8_t *data, uint16_t len)
{
	return hw_uart4_send(data,len);
}


uint8_t make_bdevice_okerror_response(uint8_t is_id_need, uint8_t is_errno_need, 
											uint8_t result, uint8_t at_index)
{
	
}



uint8_t handle_bdevice_rdsspower(uint8_t onoff, uint8_t idx)
{

}


uint8_t ble_uart_at_handle_rdsspower(uint8_t *data, uint16_t len, uint8_t idx)
{
	


}

uint8_t handle_bdevice_rnsspower(uint8_t onoff, uint8_t idx)
{
	
}


uint8_t ble_uart_at_handle_rnsspower(uint8_t *data, uint16_t len, uint8_t idx)
{
	
}

/*todo, need to close lte, 20181115*/
uint8_t handle_bdevice_airplanemode(uint8_t idx)
{
	
}
extern  UART_HandleTypeDef huart1;
uint8_t ble_uart_at_handle_poweroff(uint8_t idx)
{
	
	HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CPOF\r\n", 11, 0xFFFF);
	set_ble_work(0);
	app_power_close();

}
uint8_t ble_uart_at_handle_airplanemode(uint8_t *data, uint16_t len, uint8_t idx)
{

}

/*todo, 20181115, i won't handle it.*/
uint8_t ble_uart_at_handle_bbroadcast(uint8_t *data, uint16_t len, uint8_t idx)
{
	return 0;
}

/*todo, 20181115, i won't handle it.*/
uint8_t ble_uart_at_handle_bcall(uint8_t *data, uint16_t len, uint8_t idx)
{
	return 0;
}

/*todo, 20181115, i won't handle it.*/


uint8_t get_zdevice_num(uint8_t *data, uint16_t len, uint8_t idx)
{
					
}
uint8_t get_report_message_id(uint8_t *data, uint16_t len, uint8_t idx)
{
				
	
}
uint8_t get_report_timer(uint8_t *data, uint16_t len, uint8_t idx)
{
			
}
uint8_t get_report_port(uint8_t *data, uint16_t len, uint8_t idx)
{
				
	
}
uint8_t get_rn_num(uint8_t *data, uint16_t len, uint8_t idx)
{
					
}
uint8_t get_rd_num(uint8_t *data, uint16_t len, uint8_t idx)
{
				
}
uint8_t get_other_zdev_id(uint8_t *data, uint16_t len, uint8_t idx)
{
				
}
uint8_t get_zdev_id(uint8_t *data, uint16_t len, uint8_t idx)
{
			
			
}
uint8_t get_report_ipaddr(uint8_t *data, uint16_t len, uint8_t idx)
{
				
}
uint8_t get_report_apn(uint8_t *data, uint16_t len, uint8_t idx)
{
				
}
uint8_t get_report_usr(uint8_t *data, uint16_t len, uint8_t idx)
{

					
}
uint8_t get_report_pws(uint8_t *data, uint16_t len, uint8_t at_index)
{
				
}
uint8_t get_report_bid(uint8_t *data, uint16_t len, uint8_t at_index)
{
					
}


uint8_t ble_uart_at_handle_bpdismid(uint8_t *data, uint16_t len, uint8_t idx)
{
		uint8_t lenth = 0;	
		lenth = strlen("BPDISMID:");
	printf("open buzzer \r\n");
	
	if(data[lenth] != 'C'){
			buzzer_timer_en(1);
	}else{	
		  buzzer_timer_en(0);
	}
	
}
uint8_t ble_uart_at_handle_bpdisoid(uint8_t *data, uint16_t len, uint8_t idx)
{
uint8_t lenth = 0;	
		lenth = strlen("BPDISOID:");
	printf("open buzzer \r\n");
	
	if(data[lenth] != 'C'){
			buzzer_timer_en(1);
	}else{	
		  buzzer_timer_en(0);
	}

}
uint8_t open_lock_flag1 =0;


uint8_t ble_uart_at_handle_olock(uint8_t *data, uint16_t len, uint8_t idx)
{
	
	printf("lock commnd :%s \r\n",data);
		if((data[19] == 'O')&& (data[20] == 'N')){
			printf(" open lock \r\n");
		 set_moto_work(0);
			osDelay(1000);
		 set_moto_work(2);
		open_lock_flag1 = 1;
		}else if ((data[19] == 'O')&& (data[20] == 'F')){
		printf("close lock \r\n");
		set_moto_work(1);
			osDelay(1000);
		 set_moto_work(2);
		open_lock_flag1 = 0;
		}
}
uint8_t ble_uart_at_handle_lbbind(uint8_t *data, uint16_t len, uint8_t idx)
{
	set_ble_led_red_close();
	 bind_ok_flag =1;
}
uint8_t ble_uart_at_handle_lubind(uint8_t *data, uint16_t len, uint8_t idx)
{
	set_ble_led_red();
	 bind_ok_flag =0;
}
//uint8_t apn_buf_ble[11];
extern uint8_t apn_buf[50];
uint8_t phone_num_buf[11];
uint8_t tempbuf[11]={0};


uint8_t ble_uart_at_handle_bbind(uint8_t *data, uint16_t len, uint8_t idx)
{
		uint8_t i,lenth =0,lenth1 =0,atlen =0;
		uint8_t count =0,temp_count = 0,commnd_count =0;
		lenth = strlen("bbind=");

		set_ble_led_red_close();
		memset(tempbuf,0x00,11);
		memcpy(&apn_buf[13],&data[lenth],11);
		//memcpy(phone_num_buf,&data[lenth],11);
		//set_nvram_center_address(tempbuf);
	//if(memcmp(tempbuf,phone_num_buf,11)!=0){
		//	set_nvram_id_data(phone_num_buf);
		 //  set_nvram_id_data(phone_num_buf);
	
	   // get_nvram_id(tempbuf);
	 //  printf("save data is : %s \r\n",tempbuf);
	//	memset(tempbuf,0x00,11);
	//}
	  bind_ok_flag =1;
//		task_test_mail_put("connect",7);
	 lte_init_network();
	init_lte_flag=1;
	return 0;
}	

/*todo, 20181115, i won't handle it.*/
uint8_t ble_uart_at_handle_bunbind(uint8_t *data, uint16_t len, uint8_t idx)
{
	return 0;
	 bind_ok_flag =0;
}

/*todo, 20181115, complete it*/
uint8_t handle_bdevice_bbeat(void)
{
	uint8_t res=0;
	return res;
}


uint8_t ble_uart_at_handle_bbeat(uint8_t *data, uint16_t len, uint8_t idx)
{
	
	
}


/**************land transport module****************/
uint8_t ble_uart_at_handle_bbbeat(uint8_t *data, uint16_t len, uint8_t idx)
{
	return 0;
}

uint8_t ble_uart_at_handle_binfowrite(uint8_t *data, uint16_t len, uint8_t idx)
{
	return 0;
}

uint8_t ble_uart_at_handle_blight(uint8_t *data, uint16_t len, uint8_t idx)
{
	return 0;
}

uint8_t ble_uart_at_handle_binfoerase(uint8_t *data, uint16_t len, uint8_t idx)
{
	return 0;
}

/**************land transport module****************/

/*todo, nvram for centeradderss*/
uint8_t handle_bdevice_centeraddress(ZbTokenizer*  tzer)
{
	
}


uint8_t ble_uart_at_handle_centeraddress(uint8_t *data, uint16_t len, uint8_t idx)
{
	
}

/*todo, nvram for sos message*/
uint8_t handle_bdevice_bsosinfo(ZbTokenizer*  tzer)
{
	

}

uint8_t ble_uart_at_handle_bsosinfo(uint8_t *data, uint16_t len, uint8_t idx)
{

}
/*todo, rd or lte??*/
uint8_t handle_bdevice_bsos(const uint8_t *data, uint16_t len, uint8_t idx)
{
	
}

uint8_t ble_uart_at_handle_bsos(uint8_t *data, uint16_t len, uint8_t idx)
{
	

	
}

/*todo*/
uint8_t handle_bdevice_selfcheck(void)
{
}

uint8_t make_bdevice_selfcheck_response(void)
{
}

uint8_t ble_uart_at_handle_selfcheck(uint8_t *data, uint16_t len, uint8_t idx)
{
	
}

/*handled by 2640, not me!*/
uint8_t ble_uart_at_handle_bdistance(uint8_t *data, uint16_t len, uint8_t idx)
{
	return 0;
}

/*todo, need nvram to save this value. 20181115*/
uint8_t handle_bdevice_blowbattery(uint8_t lowbat)
{
	
}


uint8_t ble_uart_at_handle_blowbattery(uint8_t *data, uint16_t len, uint8_t idx)
{
	
	
}

/*todo, need nvram to save this value. 20181114*/
uint8_t handle_bdevice_breportinfotime(uint32_t time_in_second)
{
	
}


uint8_t ble_uart_at_handle_breportinfotime(uint8_t *data, uint16_t len, uint8_t idx)
{
	

}

/*todo, 20181115, do what??*/
uint8_t handle_bdevice_taskover(void)
{
	uint8_t res=0;
	return res;
}

/*Do we need to append device id in response message? */
uint8_t ble_uart_at_handle_taskgoon(uint8_t *data, uint16_t len, uint8_t idx)
{
	uint8_t lenth =0;
	lenth = strlen("TASKGOON:");
	set_ble_led_red_close();
	memcpy(&apn_buf[13],&data[lenth],11);
	bind_ok_flag =1;
}

/*todo, unlock the box*/
uint8_t handle_bdevice_bboxunlock()
{
	
}

uint8_t ble_uart_at_handle_bboxunlock(uint8_t *data, uint16_t len, uint8_t idx)
{
	
	
}

/*only alert, no response*/
extern uint8_t dis_data[2];

uint8_t ble_uart_at_handle_bpalert(uint8_t *data, uint16_t len, uint8_t at_index)
{
		
	

	
	//hw_uart2_send(data, len);
}

uint8_t ble_uart_data_handle(uint8_t *data, uint16_t len)
{
	uint8_t i=0,atlen=0;
	
	//if(len<AT_END_TOK_LEN) return 1;
//printf("ble_uart_data_handle1 \r\n");
//	if(memcmp(&data[len-AT_END_TOK_LEN],ble_at_end_tok,AT_END_TOK_LEN)!=0) return 1;
//printf("ble_uart_data_handle 2\r\n");
	for(i=0;i<13;i++)
	{
	
		atlen=strlen(ble_uart_at_table[i].pData);
		if((0==memcmp(data, ble_uart_at_table[i].pData, atlen))&&ble_uart_at_table[i].handle!=NULL){
				
				return ble_uart_at_table[i].handle(data, len,i);
		}
	}
	return 1;
}








uint8_t ble_receive_and_handle(void)
{
	uint8_t ret=0;
	
	memset(ble_uart_data_loop,0x00,sizeof(ble_uart_data_loop));
	hw_uart4_receive(ble_uart_data_loop, &ble_uart_data_loop_len, 0xff);
	

	printf("ble uart is %s \r\n",ble_uart_data_loop);

//hw_uart4_send(ble_uart_data_loop,ble_uart_data_loop_len);
	if (ble_uart_data_loop_len>0)
	{
	//	hw_uart1_send(ble_uart_data_loop, ble_uart_data_loop_len);
		ret=ble_uart_data_handle(ble_uart_data_loop,ble_uart_data_loop_len);
		ble_uart_data_loop_len=0;
	}
	return ret;
}



void buzzer_timer_en(uint8_t en)
{
  if(en == 1)
	{
		if(isAlerting ==0){
			osTimerStart (BuzzerTimerHandle, 500);//开始定时
			isAlerting=1;
		}else{
			return ;
		}
	}else{
		if(isAlerting ==1){
			osTimerStop (BuzzerTimerHandle);//停止定时
			isAlerting=0;
			hw_buzzer_enable(0);
		}else{
			return ;
		}
	//buzzer_alert_counter=0;
	}
}
/*
static float GUA_CalcDistByRSSI(int rssi)  
{  
    uint8_t A = 59;
    float n = 3.0;
    
    int iRssi = abs(rssi);  
    float power = (iRssi-A)/(10*n); 
    return pow(10, power);  
} 

*/
	
void BuzzerTimerCallback(void const * argument)
{
	//	buzzer_alert_counter++;
	
	//if (hw_buzzer_get_status()==1)
	//	hw_buzzer_enable(0);
//	else
		//hw_buzzer_enable(1);
	isAlerting=1;
	if(buzzer_alert_counter % 2){
				hw_buzzer_enable(1);
	
		}else{
				hw_buzzer_enable(0);
		}
	
	if(buzzer_alert_counter ==10){
			osTimerStop (BuzzerTimerHandle);//停止定时
			isAlerting=0;
			hw_buzzer_enable(0);
			buzzer_alert_counter=0;
	
	}else{
	
			buzzer_alert_counter++;
	}
	//ret=GUA_CalcDistByRSSI(-68);
	//sprintf(buffer, "%f", ret);
	//hw_uart2_send(buffer, strlen(buffer));
}

/*****************debug code *************/


/*****************debug code *************/

