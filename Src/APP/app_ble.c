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
uint8_t rd_buf[16] = {0x24,0x43,0x43,0x49,0x43,0x41,0x2C,0x30,0x2C,0x30,0x30,0x2A,0x37,0x42,0x0D,0x0A};
static uint8_t ptr[6]={0x23,0x23,0x31,0x32,0x0d,0x0a};//test code
struct zdevice_setting{
					uint8_t task_id;
					uint8_t bdev_num;
					uint8_t zdev_is_set;
					uint8_t zdev_num;
					uint8_t report_mesage_id ;
					uint8_t report_port[8];
					uint8_t zdev_report_time ;
					uint8_t bdev_report_time;					
					uint8_t rn_num[8];
					uint8_t rd_num[8];
					uint8_t other_id[12];
					uint8_t mydev_id[12];
					uint8_t report_ip[36];			
					uint8_t report_apn[36];
					uint8_t user_name[36];
					uint8_t user_paswd[36];
					
	
					//struct zdevice_bbind_bdev bbinddev;
};
struct zdevice_setting zdev;
struct bdevice_status ft_dev;
static uint8_t rd_test_flag =0;
typedef enum{
	RDSSPOWER=0,
	BPDISOID,
	RNSSPOWER,
	POWEROFF,
	AIRPLANEMODE,
	BBROADCAST,
	BCALL,
	BBIND,
	olock,
	batt,
	BUNBIND,
	LBBIND,
	LUBIND,
	BBEAT,
	BINFOWRITE,
	BBBEAT,
	BLIGHT,
	BINFOERASE,
	CENTERADDRESS,
	BSOSINFO,
	BSOS,
	//TX_BSOSSEND,//unsolicited
	SELFCHECK,
	BDISTANCE,
	BLOWBATTERY,
	//TX_ZLOCATION,//unsolicited
	BREPORTINFOTIME,
	//TX_BINFOBEAT,//unsolicited
	//TX_BPALERT,//unsolicited
	//TX_BCALERT,//unsolicited
	TASKGOON,
	BBOXUNLOCK,
	BPALERT,
	FTCN,
	GMID,
	FTRD,
	FTRN,
	FTSD,
	FTGPRS,
  FTPWKEY,
	FTSOS,
	FTCHARGKEY,
	FTLIGHT,
  FTNFC,
	FTBAT,
	FTPS,
	FTHALL,
	FTLOCK,
	FTMOTO0,
	FTMOTO1,
	FTMOTO2,
	FTLCD,
	FTLCDKEY,
	FTWRSN,
	FTXX,
	//TX_BBOXLOCKED,//unsolicited
	SUPPORT_AT_NUM
}ble_at_request;
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
static const ble_at_symbol ble_uart_at_table[SUPPORT_AT_NUM] = {
	{ble_uart_at_handle_rdsspower, (uint8_t *)"AT+RDSSPOWER="},
	{ble_uart_at_handle_bpdisoid, (uint8_t *)"BPDISOID:"},
	{ble_uart_at_handle_bpdismid, (uint8_t *)"BPDISMID:"},
	{ble_uart_at_handle_rnsspower, (uint8_t *)"AT+RNSSPOWER="},
	{ble_uart_at_handle_poweroff, (uint8_t *)"POWEROFF"},
	{ble_uart_at_handle_airplanemode, (uint8_t *)"AT+AIRPLANEMODE="},
	{ble_uart_at_handle_bbroadcast, (uint8_t *)"AT+BBROADCAST?"},
	{ble_uart_at_handle_bcall, (uint8_t *)"AT+BCALL="},
	{ble_uart_at_handle_bbind, (uint8_t *)"bbind="},
	{ble_uart_at_handle_olock, (uint8_t *)"olock:"},
	{ble_uart_at_handle_batt, (uint8_t *)"batt:"},
	{ble_uart_at_handle_bunbind, (uint8_t *)"AT+BUNBIND="},
	{ble_uart_at_handle_lbbind, (uint8_t *)"lbbind:"},
	{ble_uart_at_handle_lubind, (uint8_t *)"lubind:"},
	{ble_uart_at_handle_bbeat, (uint8_t *)"AT+BBEAT="},
	{ble_uart_at_handle_binfowrite, (uint8_t *)"AT+BINFOWRITE="},
	{ble_uart_at_handle_bbbeat, (uint8_t *)"AT+BBBEAT="},
	{ble_uart_at_handle_blight, (uint8_t *)"AT+BLIGHT="},
	{ble_uart_at_handle_binfoerase, (uint8_t *)"AT+BINFOERASE="},
	{ble_uart_at_handle_centeraddress, (uint8_t *)"AT+CENTERADDRESS="},
	{ble_uart_at_handle_bsosinfo, (uint8_t *)"AT+BSOSINFO="},
	{ble_uart_at_handle_bsos, (uint8_t *)"AT+BSOS="},
	{ble_uart_at_handle_selfcheck, (uint8_t *)"AT+SELFCHECK?"},
	{ble_uart_at_handle_bdistance, (uint8_t *)"AT+BDISTANCE="},
	{ble_uart_at_handle_blowbattery, (uint8_t *)"AT+BLOWBATTERY="},
	{ble_uart_at_handle_breportinfotime, (uint8_t *)"AT+BREPORTINFOTIME="},
	{ble_uart_at_handle_taskgoon, (uint8_t *)"TASKGOON"},
	{ble_uart_at_handle_bboxunlock, (uint8_t *)"AT+BBOXUNLOCK="},
	{ble_uart_at_handle_ble_connect, (uint8_t *)"FTCN##"},
	{ble_uart_at_handle_ble_getmid, (uint8_t *)"GMID##"},
	{ble_uart_at_handle_rd, (uint8_t *)"FTRD##"},
	{ble_uart_at_handle_rn, (uint8_t *)"FTRN##"},
	{ble_uart_at_handle_sd, (uint8_t *)"FTSD##"},
	{ble_uart_at_handle_gprs, (uint8_t *)"FTGPRS##"},
	{ble_uart_at_handle_power_key, (uint8_t *)"FTPWKEY##"},
	{ble_uart_at_handle_sos_key, (uint8_t *)"FTSOS##"},
	{ble_uart_at_handle_charger_key, (uint8_t *)"FTCHARGKEY##"},
	{ble_uart_at_handle_light, (uint8_t *)"FTLIGHT##"},
	{ble_uart_at_handle_nfc, (uint8_t *)"FTNFC##"},
	{ble_uart_at_handle_battery, (uint8_t *)"FTBAT##"},
	{ble_uart_at_handle_sensor_ps, (uint8_t *)"FTPS##"},
	{ble_uart_at_handle_hall, (uint8_t *)"FTHALL##"},
	{ble_uart_at_handle_lock, (uint8_t *)"FTLOCK##"},
	{ble_uart_at_handle_moto_s, (uint8_t *)"FTMOTO0##"},
	{ble_uart_at_handle_moto_z, (uint8_t *)"FTMOTO1##"},
	{ble_uart_at_handle_moto_c, (uint8_t *)"FTMOTO2##"},
	{ble_uart_at_handle_lcd, (uint8_t *)"FTLCD##"},
	{ble_uart_at_handle_lcd_key, (uint8_t *)"FTLCDKEY##"},
	{ble_uart_at_handle_write_sn, (uint8_t *)"FTSN"},
	{ble_uart_at_handle_reset_back_data, (uint8_t *)"FTXX##"},
};


#if 0	
static const ble_at_symbol ble_uart_at_table1[13] = {
	{get_zdevice_num, (uint8_t *)"znu"},
	{get_report_bid, (uint8_t *)"bid"},
	{get_report_message_id, (uint8_t *)"med"},
	{get_report_timer, (uint8_t *)"brt"},
	{get_report_port, (uint8_t *)"rpt"},
	{get_rn_num, (uint8_t *)"rnu"},
	{get_rd_num, (uint8_t *)"rdu"},
	{get_zdev_id, (uint8_t *)"mid"},
	{get_other_zdev_id, (uint8_t *)"oid"},
	
	{get_report_ipaddr, (uint8_t *)"rip"},
	{get_report_apn, (uint8_t *)"apn"},
	{get_report_usr, (uint8_t *)"usr"},
	{get_report_pws, (uint8_t *)"pws"},
	
};
#endif

static const ble_at_symbol ble_at_respone_table[SUPPORT_AT_NUM] = {
	{0, (uint8_t *)"+RDSSPOWER:"},
	{0, (uint8_t *)"+RNSSPOWER:"},
	{0, (uint8_t *)"+AIRPLANEMODE:"},
	{0, (uint8_t *)"+BBROADCAST:"},
	{0, (uint8_t *)"+BCALL:"},
	{0, (uint8_t *)"+BBIND:"},
	{0, (uint8_t *)"+BUNBIND:"},
	{0, (uint8_t *)"+BBEAT:"},
	{0, (uint8_t *)"+BINFOWRITE:"},
	{0, (uint8_t *)"+BBBEAT:"},
	{0, (uint8_t *)"+BLIGHT:"},
	{0, (uint8_t *)"+BINFOERASE:"},
	{0, (uint8_t *)"+CENTERADDRESS:"},
	{0, (uint8_t *)"+BSOSINFO:"},
	{0, (uint8_t *)"+BSOS:"},
	//{0, (uint8_t *)""},//+BSOSSEND:
	{0, (uint8_t *)"+SELFCHECK:"},
	{0, (uint8_t *)"+BDISTANCE:"},
	{0, (uint8_t *)"+BLOWBATTERY:"},
	//{0, (uint8_t *)""},//TX_ZLOCATION,
	{0, (uint8_t *)"+BREPORTINFOTIME:"},
	//{0, (uint8_t *)""},//+BINFOBEAT:
	//{0, (uint8_t *)""},//+BPALERT:
	//{0, (uint8_t *)""},//+BCALERT:
	{0, (uint8_t *)"+TASKOVER:"},
	{0, (uint8_t *)"+BBOXUNLOCK:"},
	//{0, (uint8_t *)""},//+BBOXLOCKED: 
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
	uint8_t buf[20];
//	printf("testTimerCallback \r\n");
	task_gps_message_put(0x02);
	if(testtimercount >20)
			{
				hw_bd2_close();
				testtimercount =0;
				task_test_mail_put("stoptesttimer",13);
			}
	if(ft_dev.rn_flags){
					printf("ft_dev.rn_flags = %d \r\n",ft_dev.rn_flags);
						memcpy(buf,"FTRNOK",6);
						memcpy(&buf[6],ble_at_end_tok,4);
						ble_send_response(buf,10);
						rn_send_flag =0;
						//hw_bd2_close();
						hw_bd2_close();
					task_test_mail_put("stoptesttimer",13);
				}
	
				testtimercount++;
}
/*****************/
//$CCRMO,BSI,2,1*27 ÉèÖÃ¹¦ÂÊÊä³ö
//$CCTXA,0179773,1,2,A468656C6C6F*7A ·¢ËÍÖ¸Áî
//$BDBSI,01,03,3,2,2,0,3,0,0,3,0,0*5F ÐÅºÅÇ¿¶È
//$BDTXR,1,0179773,2,,A468656C6C6F*43 ½ÓÊÕÖ¸Áî
/**************************/
uint8_t rd_send_power[30];
uint8_t rd_send_data[50];
uint8_t rd_power_send_flag =0;
uint8_t commnd_lenth;
uint8_t back_rn_numbuf[20];
uint8_t rdtimercount =0;
void test(uint8_t *buf,uint8_t len)
{
		uint8_t count = 0,temp =0;
		uint8_t tempbuf[50];
		while(buf[count]){
			if(buf[count] == '$' || buf[count] == '!'){
						temp = count;
					
			}else if(buf[count] == '*'){
								memset(tempbuf,0x00,50);
								memcpy(tempbuf,&buf[temp+1],count-temp-1);
								break;
			}	
			count++;
		}
		count =0;
	//	printf("tempbuf = %s \r\n",tempbuf);
		while(tempbuf[count] != 0){
					
			//			printf("char is %c \r\n",tempbuf[count]);
			
				if(count ==0){
						temp = tempbuf[count] ^ tempbuf[count+1];
				
				}else if(count >0 && tempbuf[count+1] !=0){
						temp = temp ^ tempbuf[count+1];
				
				}else if(tempbuf[count +1] == 0){
				
						printf("temp = %x \r\n",temp);
								break;
				}
			
				count++;
		}
		printf("low is = %d\r\n",temp&0x0f);
		
		if( ((temp>>4)>= 0) && ((temp>>4)<=9) ){
				buf[len] = (temp >> 4) + '0';
		}
		if( (9 <(temp>>4)) && ((temp>>4)<17)){
				buf[len] = (temp >>4) + '7';
		}
		 if(((temp&0x0f) <= 9)&&((temp&0x0f) >=0)){
				buf[len+1] = (temp & 0x0f) + '0';
					
			}
		 if((9<(temp&0x0f))&&((temp&0x0f)<17)){
			buf[len+1] = (temp & 0x0f) + '7';
			printf("buf[33] = %c \r\n",buf[33]);
			}
			
}

uint8_t batt_soc[10];
extern uint8_t bat_rsoc;
extern uint8_t lte_network_flag;
extern uint8_t lte_init_flag;
extern void read_ps_data(uint8_t *val);
uint8_t ti_reset_flag =0;
uint8_t data_lock[2];
void LedTimerCallback(void const * argument)
{
		printf("LedTimerCallback \r\n");
	
			//	if(ti_reset_flag ==5){
					//	set_ble_work(0);
					//	ti_reset_flag =0;
					//osDelay(100);
					// set_ble_work(1);
			//	}
				//	set_local_data();
				#if 1
	      if(lte_init_flag ==1){
					printf("init lte and connect\r\n");
				lte_open_connect_network();
				lte_tcp_connect_network();
				printf("lte send data before\r\n");
				lte_tcp_send_data();
				printf("lte send data after \r\n");
				lte_close_connect_network();
				}
			#endif
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
	#if 0
	
			if(rdtimercount >20)
			{
				rdtimercount =0;
						hw_bd1_close();
				task_test_mail_put("stoprdtimer",11);
			}
		if((rd_num[0]==0)&& (rd_num[1] ==0)&& (rd_num[2] ==0)){
			HAL_UART_Transmit(&huart2,rd_buf,16, 0xFFFF);
			}
		else if((rd_num[0]!=0)&& (rd_num[1] !=0)&& (rd_num[2] !=0 )&&(rd_test_flag == 0)){
				commnd_lenth = strlen("$CCRMO,BSI,2,1*27\r\n");
				printf("commnd_lenth = %d \r\n",commnd_lenth);
				//memcpy(&rd_send_power[commnd_lenth],ble_at_end,2);
				memcpy(rd_send_power,"$CCRMO,BSI,2,1*27\r\n",commnd_lenth);			
				hw_uart2_send_nodma(rd_send_power,21);
				memcpy(back_rn_numbuf,"FTRD",4);
				memcpy(&back_rn_numbuf[4],rd_num,6);
				memcpy(&back_rn_numbuf[10],"##\r\n",6);
				ble_send_response(back_rn_numbuf,16);
				rd_test_flag =1;
				printf("timer rd_num = %s  \r\n",rd_num);
		 }else if( (0 ==rd_power_send_flag)&&rd_power_buf[0]!=0 &&(rd_power_buf[13]>2 || rd_power_buf[15]>2 || rd_power_buf[17]>2 || rd_power_buf[19]>2 || rd_power_buf[21]>2 || rd_power_buf[23]>2 || rd_power_buf[25]>2 ||rd_power_buf[27]>2 ||rd_power_buf[29]>2 )){
				memset(rd_send_data,0x00,50);
				memcpy(rd_send_data,"$CCTXA,0",8);
				memcpy(&rd_send_data[8],rd_num,6);
				memcpy(&rd_send_data[14],",1,2,A468656C6C6F*",18);
				test(rd_send_data,32);
				memcpy(&rd_send_data[34],"\r\n",4);
				HAL_UART_Transmit(&huart2,rd_send_data,38, 0xFFFF);
				rd_power_send_flag = 1;
	
		 }else if( 0 == memcmp(rd_rece_buf,"$BDTXR",6)){
			
				if (0 == memcmp(&rd_rece_buf[22],"68656C6C6F",10)){			
					ble_send_response("FTRDOK##\r\n",12);
					osDelay(500);
						task_test_mail_put("stoprdtimer",11);
						
					 printf("rd rece : %s \r\n",&rd_rece_buf[22]);
					memset(rd_rece_buf ,0x00,50);
				}
		 }
		 rdtimercount++;
		#endif 
	
}
	
uint8_t ble_uart_at_handle_rd(uint8_t *data, uint16_t len, uint8_t idx)
{
				printf("rd test \r\n");
			ft_dev.rd_flags =0;
		
		rd_power_send_flag = 0;
		hw_bd1_init();
		memset(rd_send_power,0x00,30);
		memset(rd_send_data,0x00,50);
		memset(back_rn_numbuf,0x00,50);
		memset(rd_num,0x00,6);
		memset(rd_power_buf,0x00,50);
		memset(rd_rece_buf,0x00,50);
		//hw_bd1_open();
		rd_test_flag =0;
		rdtimercount =0;
	osTimerStart (rdTimerHandle, 3000);//¿ªÊ¼¶¨Ê
	//ble_send_response("OKFTRD##",8);
			
	//ble_uart_at_handle_light
	
}
/* ************¶¨Î» rn¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á¡Á*/

uint8_t ble_uart_at_handle_rn(uint8_t *data, uint16_t len, uint8_t idx)
{	
		ft_dev.rn_flags =0;	
		printf("rn test \r\n");
		hw_bd2_init();
		osTimerStart (testTimerHandle, 3000);
		rn_send_flag = 1;
	//rd_power_send_flag =0;
}
uint8_t ble_uart_at_handle_sd(uint8_t *data, uint16_t len, uint8_t idx)
{
			
			
}
uint8_t ble_uart_at_handle_gprs(uint8_t *data, uint16_t len, uint8_t idx)
{
			uint8_t buf[20];
				if(ft_dev.gprs_flags ==1){
				
						memcpy(buf,"OKFTGPRS",8);
						memcpy(&buf[6],ble_at_end_tok,4);
						//ble_send_response(buf,12);
				
				}else{
				
						memcpy(buf,"ERRFTGPRS",9);
						memcpy(&buf[6],ble_at_end_tok,4);
					//	ble_send_response(buf,13);
				
				}

}
uint8_t ble_uart_at_handle_power_key(uint8_t *data, uint16_t len, uint8_t idx)
{
					uint8_t buf[15];
					uint8_t count =0;
	
					ft_dev.pwkey_flags = 0;
					while(1){
									printf("pwkey_flags = %d \r\n",ft_dev.pwkey_flags);
								if(ft_dev.pwkey_flags ==1){							
										memcpy(buf,"FTPWKEYOK",9);
										memcpy(&buf[9],ble_at_end_tok,4);
										ble_send_response(buf,13);
										break;
									
								}else{
										if(count >100){
										memcpy(buf,"FTPWKEYERR",10);
										memcpy(&buf[10],ble_at_end_tok,4);
										ble_send_response(buf,14);
											count =0;
											break;
										}
										osDelay(3000);
										count ++;
								}
					}
	
}
uint8_t ble_uart_at_handle_sos_key(uint8_t *data, uint16_t len, uint8_t idx)
{
				uint8_t buf[15];
					uint8_t count =0;
	
					ft_dev.soskey_flags = 0;
					while(1){
								if(ft_dev.soskey_flags ==1){							
										memcpy(buf,"FTSOSOK",7);
										memcpy(&buf[7],ble_at_end_tok,4);
										ble_send_response(buf,11);
										break;
									
								}else{
										if(count >100){
										memcpy(buf,"FTSOSERR",8);
										memcpy(&buf[8],ble_at_end_tok,4);
										ble_send_response(buf,12);
											break;
										}
										osDelay(1000);
										count ++;
								}
					}
}
uint8_t ble_uart_at_handle_charger_key(uint8_t *data, uint16_t len, uint8_t idx)
{

}
uint8_t ble_uart_at_handle_light(uint8_t *data, uint16_t len, uint8_t idx)
{
				uint8_t count =0;
				uint8_t led[6] = {0x29,0x2d,0x2c,0x2b,0x26,0x2a};
				
				aw9523_set_light(led[0],0x0);
					aw9523_set_light(led[1],0x0);
					aw9523_set_light(led[2],0x0);
					aw9523_set_light(led[3],0x0);
					aw9523_set_light(led[4],0x0);
					aw9523_set_light(led[5],0x0);
			while(1){
				if(count ==6){
				aw9523_set_light(led[0],0x0);
					aw9523_set_light(led[1],0x0);
					aw9523_set_light(led[2],0x0);
					aw9523_set_light(led[3],0x0);
					aw9523_set_light(led[4],0x0);
					aw9523_set_light(led[5],0x0);
						break;
				}
					aw9523_set_light(led[count],0x30);
				osDelay(1000);
				//aw9523_set_light(led[count],0x00);
				count ++;
			}

}
uint8_t ble_uart_at_handle_nfc(uint8_t *data, uint16_t len, uint8_t idx)
{
				uint8_t buf[15];
				if(ft_dev.nfc_flags ==1){
				
									memcpy(buf,"FTNFCOK",7);
									memcpy(&buf[7],ble_at_end_tok,4);
									ble_send_response(buf,11);
				}else{
									memcpy(buf,"FTNFCERR",8);
									memcpy(&buf[8],ble_at_end_tok,4);
									ble_send_response(buf,12);
				
				}

}


extern void zdev_read_battery_soc(void);
extern uint8_t bat_zdev_soc;
uint8_t ble_uart_at_handle_battery(uint8_t *data, uint16_t len, uint8_t idx)
{
		uint16_t battery;
		uint8_t val[2],buf[20],temp,temp1;
	
		zdev_read_battery_soc();
		 while(1){
					
					if(bat_zdev_soc == 0xff ){
										memcpy(buf,"FTBATERR",8);
										memcpy(&buf[8],ble_at_end_tok,4);
										ble_send_response(buf,12);	
										break;
					}else{
										if(bat_zdev_soc > 100){
												battery = 100;
										}else{
													if(bat_zdev_soc ==100){
													memcpy(buf,"FTBAT100",8);
													memcpy(&buf[8],ble_at_end_tok,4);
													ble_send_response(buf,12);	
													}else{
															temp = (bat_zdev_soc/10) +0x30;
															temp1= (bat_zdev_soc%10) +0x30;
															memcpy(buf,"FTBAT",5);
															memcpy(&buf[5],&temp,1);
															memcpy(&buf[6],&temp1,1);
															
															memcpy(&buf[7],ble_at_end_tok,4);
															ble_send_response(buf,11);
															break;
													}
											
										}
												
					}
		 }
}
extern void read_ps_data(uint8_t *val);
uint8_t ble_uart_at_handle_sensor_ps(uint8_t *data, uint16_t len, uint8_t idx)
{
		uint8_t ps_data[2],buf[10];
		uint16_t count = 0;
	
		 while(1){
					osDelay(1000);
					if(count>20)
					{
								count = 0;
							break;
					}
					read_ps_data(ps_data);
					printf("ps_data = %x %x\r\n",ps_data[0],ps_data[1]);
			 
					if(ps_data[0] <5 && ps_data[1] <5){
								memcpy(buf,"FTPS1",5);
								memcpy(&buf[5],ble_at_end_tok,4);
								ble_send_response(buf,9);
							//	break;
					}else{
								memcpy(buf,"FTPS0",5);
								memcpy(&buf[5],ble_at_end_tok,4);
								ble_send_response(buf,9);
							//	break;
					
					}
						count ++;
					
					
		 }
}
uint8_t ble_uart_at_handle_hall(uint8_t *data, uint16_t len, uint8_t at_index)
{
				uint8_t buf[15];
					uint8_t count;
	
					ft_dev.hall_flags = 0;
					while(1){
									printf("ft_dev.hall_flags = %d \r\n",ft_dev.hall_flags);
								if(ft_dev.hall_flags ==1){							
										memcpy(buf,"FTHALLOK",8);
										memcpy(&buf[8],ble_at_end_tok,4);
										ble_send_response(buf,12);
										break;
									
								}else{
										if(count >100){
										memcpy(buf,"FTHALLERR",9);
										memcpy(&buf[9],ble_at_end_tok,4);
										ble_send_response(buf,13);
											break;
										}
										osDelay(5000);
										count ++;
								}
					}
	

}
uint8_t ble_uart_at_handle_lock(uint8_t *data, uint16_t len, uint8_t at_index)
{
				uint8_t buf[15];
					uint8_t count =0;
	
					ft_dev.lock_flags = 0;
					while(1){
									printf("lock_flags = %d \r\n",ft_dev.lock_flags);
								if(ft_dev.lock_flags ==1){							
										memcpy(buf,"FTLOCKOK",8);
										memcpy(&buf[8],ble_at_end_tok,4);
										ble_send_response(buf,12);
										break;
									
								}else{
										if(count >100){
										memcpy(buf,"FTLOCKERR",9);
										memcpy(&buf[9],ble_at_end_tok,4);
										ble_send_response(buf,13);
											count =0;
											break;
										}
										osDelay(2000);
										count ++;
								}
					}

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
	uint8_t* p="110123456789999";
	memcpy(data,p,DEVICE_ID_LEN);
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
	uint16_t totallen=0,len=0;
	uint8_t   deviceid[DEVICE_ID_LEN];
	uint8_t response[64];

	len=strlen(ble_at_respone_table[at_index].pData);
	memcpy(&response[totallen],ble_at_respone_table[at_index].pData,len);
	totallen+=len;

	if(is_id_need)
	{
		get_bdevice_id(deviceid);
		len=DEVICE_ID_LEN;
		memcpy(&response[totallen],deviceid,len);
		totallen+=len;

		len=1;
		add_separator_comma(&response[totallen]);
		totallen+=len;
	}


	if(result==0)
	{
		len=2;
		memcpy(&response[totallen],"OK",len);
		totallen+=len;
	}
	else/*to do, error number needed? 20181113*/
	{
		uint8_t buf[10];
		len=5;
		memcpy(&response[totallen],"ERROR",len);
		totallen+=len;

		if(is_errno_need)
		{
			len=1;
			add_separator_comma(&response[totallen]);
			totallen+=len;
			
			int2str(result, buf);
			len=strlen(buf);
			memcpy(&response[totallen],buf,len);
			totallen+=len;
		}
	}

	len=AT_END_TOK_LEN;
	memcpy(&response[totallen],ble_at_end_tok,len);
	totallen+=AT_END_TOK_LEN;

	return ble_send_response(response,totallen);
}



uint8_t handle_bdevice_rdsspower(uint8_t onoff, uint8_t idx)
{
	uint8_t result=0,res=0;
	
	if(onoff==0)
		hw_bd1_close();
	else if(onoff==1)
		hw_bd1_open();
	res=make_bdevice_okerror_response(1,1,result,idx);
	return res;
}


uint8_t ble_uart_at_handle_rdsspower(uint8_t *data, uint16_t len, uint8_t idx)
{
	ZbTokenizer  tzer[1];
	ZbToken  tok_device_id;
	uint16_t headerlen=0;
	uint8_t   deviceid[DEVICE_ID_LEN],res;

	headerlen=strlen(ble_uart_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);
	get_bdevice_id(deviceid);

	tok_device_id = zb_tokenizer_get(tzer,0);
	if(0==memcmp(deviceid,tok_device_id.p,DEVICE_ID_LEN))
	{
		ZbToken tok_power=zb_tokenizer_get(tzer,1);
		if(tok_power.end-tok_power.p==2)//ON
			res=handle_bdevice_rdsspower(1,idx);
		else if(tok_power.end-tok_power.p==3)//OFF
			res=handle_bdevice_rdsspower(0,idx);
	}
	else
	{
		res=make_bdevice_okerror_response(1, 1, 1, idx);
	}
	return res;
}

uint8_t handle_bdevice_rnsspower(uint8_t onoff, uint8_t idx)
{
	uint8_t result=0,res=0;
	if(onoff==0)
		hw_bd2_close();
	else if(onoff==1)
		hw_bd2_open();
	res=make_bdevice_okerror_response(1,1,result,idx);
	return res;
}


uint8_t ble_uart_at_handle_rnsspower(uint8_t *data, uint16_t len, uint8_t idx)
{
	ZbTokenizer  tzer[1];
	ZbToken  tok_device_id;
	uint16_t headerlen=0;
	uint8_t   deviceid[DEVICE_ID_LEN],res;

	headerlen=strlen(ble_uart_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);
	get_bdevice_id(deviceid);

	tok_device_id = zb_tokenizer_get(tzer,0);
	if(0==memcmp(deviceid,tok_device_id.p,DEVICE_ID_LEN))
	{
		ZbToken tok_power=zb_tokenizer_get(tzer,1);
		if(tok_power.end-tok_power.p==2)//on
			res=handle_bdevice_rnsspower(1,idx);
		else if(tok_power.end-tok_power.p==3)//off
			res=handle_bdevice_rnsspower(0,idx);
	}
	else
	{
		res=make_bdevice_okerror_response(1, 1, 1, idx);
	}
	return res;
}

/*todo, need to close lte, 20181115*/
uint8_t handle_bdevice_airplanemode(uint8_t idx)
{
	uint8_t result=0,res=0;
	hw_bd1_close();
	hw_bd2_close();
	res=make_bdevice_okerror_response(1,1,result, idx);
	return res;
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
	ZbTokenizer  tzer[1];
	ZbToken  tok_device_id;
	uint16_t headerlen=0;
	uint8_t   deviceid[DEVICE_ID_LEN],res;

	headerlen=strlen(ble_uart_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);
	get_bdevice_id(deviceid);

	tok_device_id = zb_tokenizer_get(tzer,0);
	if(0==memcmp(deviceid,tok_device_id.p,DEVICE_ID_LEN))
	{
		res=handle_bdevice_airplanemode(idx);
	}
	else
	{
		res=make_bdevice_okerror_response(1, 1, 1, idx);
	}
	return res;
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
					uint8_t lenth,znum = 0;
					lenth = strlen(data);
					zdev.zdev_num = data[3] - '0';
	
						ble_send_response(data,lenth);	
					printf("zdev.zdev_num = %d\r\n",zdev.zdev_num);
}
uint8_t get_report_message_id(uint8_t *data, uint16_t len, uint8_t idx)
{
					zdev.report_mesage_id = data[3] - '0';
					printf("report_mesage_id = %d\r\n",zdev.report_mesage_id);
	
}
uint8_t get_report_timer(uint8_t *data, uint16_t len, uint8_t idx)
{
				uint8_t lenth;
				
				lenth = strlen(data);
				if((lenth -3) ==3){
						zdev.bdev_report_time = (data[3] - '0')*100 + (data[4]-'0') *10 +(data[5] -'0');
				}else if( (lenth -3) ==2){
				
						zdev.bdev_report_time = (data[3]- '0')*10 + (data[4] -'0');
				}else if(( lenth -3) ==1){
				
						zdev.bdev_report_time = (data[3]- '0');
				}
				printf("zdev.bdev_report_time = %d\r\n",zdev.bdev_report_time);
}
uint8_t get_report_port(uint8_t *data, uint16_t len, uint8_t idx)
{
					uint8_t lenth = 0;
		
					lenth = strlen(data);
					
					memcpy(zdev.report_port,&data[3],4);
	printf("zdev.report_port = %s\r\n",zdev.report_port);
	
}
uint8_t get_rn_num(uint8_t *data, uint16_t len, uint8_t idx)
{
					uint8_t lenth = 0;
		
					lenth = strlen(data);
					
					memcpy(zdev.rn_num,&data[3],6);	
printf("zdev.rn_num = %s\r\n",zdev.rn_num);	
}
uint8_t get_rd_num(uint8_t *data, uint16_t len, uint8_t idx)
{
					uint8_t lenth = 0;
		
					lenth = strlen(data);
					
					memcpy(zdev.rd_num,&data[3],6);	
	printf("zdev.rd_num = %s\r\n",zdev.rd_num);
}
uint8_t get_other_zdev_id(uint8_t *data, uint16_t len, uint8_t idx)
{
				uint8_t lenth = 0;
		
					lenth = strlen(data);
				//	printf("zdev data %s \r\n",data);
					memcpy(zdev.other_id,&data[3],13);	
					if(data[4] == '0' && data[3] == '1')
						ble_send_response(data,lenth);
	printf("zdev.other_id = %s\r\n",zdev.other_id);
			//	ble_send_response(data,lenth);	
}
uint8_t get_zdev_id(uint8_t *data, uint16_t len, uint8_t idx)
{
			uint8_t lenth = 0;
		
					lenth = strlen(data);
					//printf("zdev mdata %s \r\n",data);
					memcpy(zdev.mydev_id,&data[3],13);
					osDelay(1000);
					if(data[4] == '0' && data[3] == '1')
						ble_send_response(data,lenth);
	printf("zdev.mydev_id = %s\r\n",zdev.mydev_id);
			
}
uint8_t get_report_ipaddr(uint8_t *data, uint16_t len, uint8_t idx)
{
					uint8_t lenth = 0;
		
					lenth = strlen(data);
					
					memcpy(zdev.report_ip,&data[3],lenth -3);	
printf("zdev.report_ip = %s\r\n",zdev.report_ip);	
}
uint8_t get_report_apn(uint8_t *data, uint16_t len, uint8_t idx)
{
					uint8_t lenth = 0;
		
					lenth = strlen(data);
					
					memcpy(zdev.report_apn,&data[3],lenth -3);	
printf("zdev.report_apn = %s\r\n",zdev.report_apn);	
}
uint8_t get_report_usr(uint8_t *data, uint16_t len, uint8_t idx)
{

						uint8_t lenth = 0;
		
					lenth = strlen(data);
					
					memcpy(zdev.user_name,&data[3],lenth -3);	
	printf("zdev.user_name = %s\r\n",zdev.user_name);
}
uint8_t get_report_pws(uint8_t *data, uint16_t len, uint8_t at_index)
{
					uint8_t lenth = 0;
		
					lenth = strlen(data);
					
					memcpy(zdev.user_paswd,&data[3],lenth -3);	
	printf("zdev.user_paswd = %s\r\n",zdev.user_paswd);
}
uint8_t get_report_bid(uint8_t *data, uint16_t len, uint8_t at_index)
{
					uint8_t lenth = 0;
		
					lenth = strlen("bid");
					
				//	memcpy(zde,&data[3],lenth -3);	
					zdev.task_id = data[lenth] - '0';
	
					ble_send_response(data,len);
				printf("zdev.task_id = %d\r\n",zdev.task_id);
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
}
uint8_t ble_uart_at_handle_lubind(uint8_t *data, uint16_t len, uint8_t idx)
{
	set_ble_led_red();
}
//uint8_t apn_buf_ble[11];
extern uint8_t apn_buf[50];
uint8_t ble_uart_at_handle_bbind(uint8_t *data, uint16_t len, uint8_t idx)
{
		uint8_t i,lenth =0,lenth1 =0,atlen =0;
		uint8_t count =0,temp_count = 0,commnd_count =0;
		lenth = strlen("bbind=");
	set_ble_led_red_close();
		memcpy(&apn_buf[13],&data[lenth],11);
	   printf("bbind data is : %s \r\n",data);
		task_test_mail_put("connect",7);
	 lte_init_network();
	return 0;
}	

/*todo, 20181115, i won't handle it.*/
uint8_t ble_uart_at_handle_bunbind(uint8_t *data, uint16_t len, uint8_t idx)
{
	return 0;
}

/*todo, 20181115, complete it*/
uint8_t handle_bdevice_bbeat(void)
{
	uint8_t res=0;
	return res;
}


uint8_t ble_uart_at_handle_bbeat(uint8_t *data, uint16_t len, uint8_t idx)
{
	ZbTokenizer  tzer[1];
	ZbToken  tok_device_id;
	uint16_t headerlen=0;
	uint8_t deviceid[DEVICE_ID_LEN],res;

	headerlen=strlen(ble_uart_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);
	get_bdevice_id(deviceid);

	tok_device_id=zb_tokenizer_get(tzer,0);

	if(0==memcmp(deviceid,tok_device_id.p,DEVICE_ID_LEN))
	{
		res=handle_bdevice_bbeat();
		make_bdevice_okerror_response(1,0,0,idx);
	}
	else
	{
		res=make_bdevice_okerror_response(1,0,1,idx);
	}
	return res;
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
	uint8_t address[NV_CENTER_ADDRESS_SIZE_BYTE]={0};
	ZbToken  tok_centeraddress;
	flash_status_t res;
	
	tok_centeraddress=zb_tokenizer_get(tzer,0);
	memcpy(address,tok_centeraddress.p,(tok_centeraddress.end-tok_centeraddress.p));

	res=set_nvram_center_address(address);
	
	return (uint8_t)res;
}


uint8_t ble_uart_at_handle_centeraddress(uint8_t *data, uint16_t len, uint8_t idx)
{
	ZbTokenizer  tzer[1];
	ZbToken  tok_centeraddress;
	uint16_t headerlen=0;
	uint8_t res;

	headerlen=strlen(ble_uart_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);

	handle_bdevice_centeraddress(tzer);

	make_bdevice_okerror_response(1,0,0,idx);
	return res;
}

/*todo, nvram for sos message*/
uint8_t handle_bdevice_bsosinfo(ZbTokenizer*  tzer)
{
	uint8_t ltedest[NV_LTE_DEST_SIZE_BYTE]={0},msg[NV_SOS_DEFAULT_MSG_SIZE_BYTE]={0};
	ZbToken  tok_rddest,tok_ltedest,tok_msg;
	uint32_t cardno=0;
	flash_status_t res;
	tok_ltedest=zb_tokenizer_get(tzer,0);
	tok_rddest=zb_tokenizer_get(tzer,1);
	tok_msg=zb_tokenizer_get(tzer,2);

	cardno=(uint32_t)str2int(tok_rddest.p, tok_rddest.end);
	memcpy(ltedest,tok_ltedest.p,(tok_ltedest.end-tok_ltedest.p));
	memcpy(msg,tok_msg.p,(tok_msg.end-tok_msg.p));

	res=set_nvram_sos_info(cardno, ltedest, msg);

	return (uint8_t)res;
}

uint8_t ble_uart_at_handle_bsosinfo(uint8_t *data, uint16_t len, uint8_t idx)
{
	ZbTokenizer  tzer[1];
	uint16_t headerlen=0;
	uint8_t res;

	headerlen=strlen(ble_uart_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);

	res=handle_bdevice_bsosinfo(tzer);

	if(res==0)
		make_bdevice_okerror_response(1,0,0,idx);
	else
		make_bdevice_okerror_response(1,0,1,idx);
	return res;
}

/*todo, rd or lte??*/
uint8_t handle_bdevice_bsos(const uint8_t *data, uint16_t len, uint8_t idx)
{
	uint8_t res;
	return res;
}

uint8_t ble_uart_at_handle_bsos(uint8_t *data, uint16_t len, uint8_t idx)
{
	ZbTokenizer  tzer[1];
	ZbToken  tok_device_id,tok_sos;
	uint16_t headerlen=0;
	uint8_t deviceid[DEVICE_ID_LEN],res;

	headerlen=strlen(ble_uart_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);
	get_bdevice_id(deviceid);

	tok_device_id=zb_tokenizer_get(tzer,0);
	tok_sos=zb_tokenizer_get(tzer,1);

	if(0==memcmp(deviceid,tok_device_id.p,DEVICE_ID_LEN))
	{
		uint16_t sos_buf_len=(tok_sos.end-tok_sos.p>SOS_MESSAGE_MAX_LEN)?SOS_MESSAGE_MAX_LEN:(tok_sos.end-tok_sos.p);
		
		res=handle_bdevice_bsos(tok_sos.p,sos_buf_len,idx);
		make_bdevice_okerror_response(1,0,0,idx);
	}
	else
	{
		res=make_bdevice_okerror_response(1,0,1,idx);
	}
	return res;
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
	ZbTokenizer  tzer[1];
	uint16_t headerlen=0;
	uint8_t deviceid[DEVICE_ID_LEN],res;

	headerlen=strlen(ble_uart_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);
	get_bdevice_id(deviceid);

	res=handle_bdevice_selfcheck();
	make_bdevice_selfcheck_response();
	return res;
}

/*handled by 2640, not me!*/
uint8_t ble_uart_at_handle_bdistance(uint8_t *data, uint16_t len, uint8_t idx)
{
	return 0;
}

/*todo, need nvram to save this value. 20181115*/
uint8_t handle_bdevice_blowbattery(uint8_t lowbat)
{
	flash_status_t ret=0;
	uint8_t ori_bat=0;

	get_nvram_low_battery(&ori_bat);
	if(ori_bat^lowbat)
	{
		ret=set_nvram_low_battery(lowbat);
	}
	return (uint8_t)ret;
}


uint8_t ble_uart_at_handle_blowbattery(uint8_t *data, uint16_t len, uint8_t idx)
{
	ZbTokenizer  tzer[1];
	ZbToken battery;
	uint16_t headerlen=0;
	uint8_t   res,result,lowbat;

	headerlen=strlen(ble_uart_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);

	battery=zb_tokenizer_get(tzer, 0);
	lowbat=(uint8_t)str2int(battery.p, battery.end);

	if(lowbat>=0 && lowbat<=4)
	{
		handle_bdevice_blowbattery(lowbat);
		make_bdevice_okerror_response(0,0,0,idx);
	}
	else
	{
		make_bdevice_okerror_response(0,0,1,idx);
	}
	return res;
}

/*todo, need nvram to save this value. 20181114*/
uint8_t handle_bdevice_breportinfotime(uint32_t time_in_second)
{
	flash_status_t ret=0;
	uint32_t ori_data=0;

	ret=get_nvram_report_period(&ori_data);
	if(ori_data^time_in_second)
	{
		ret=set_nvram_report_period(time_in_second);
	}
	return (uint8_t)ret;
}


uint8_t ble_uart_at_handle_breportinfotime(uint8_t *data, uint16_t len, uint8_t idx)
{
	ZbTokenizer  tzer[1];
	ZbToken  tok_device_id,tok_time;
	uint32_t time_in_second;
	uint16_t headerlen=0;
	uint8_t deviceid[DEVICE_ID_LEN],res;

	headerlen=strlen(ble_uart_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);
	get_bdevice_id(deviceid);

	tok_device_id=zb_tokenizer_get(tzer,0);
	tok_time=zb_tokenizer_get(tzer,1);

	if(0==memcmp(deviceid,tok_device_id.p,DEVICE_ID_LEN))
	{
		time_in_second=(uint32_t)str2int(tok_time.p, tok_time.end);
		res=handle_bdevice_breportinfotime(time_in_second);
		make_bdevice_okerror_response(0,0,0,idx);
	}
	else
	{
		res=make_bdevice_okerror_response(0,0,1,idx);
	}
	return res;
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
	set_ble_led_red_close();

}

/*todo, unlock the box*/
uint8_t handle_bdevice_bboxunlock()
{
	uint8_t res=0;
	return res;
}

uint8_t ble_uart_at_handle_bboxunlock(uint8_t *data, uint16_t len, uint8_t idx)
{
	ZbTokenizer  tzer[1];
	ZbToken  tok_device_id;
	uint16_t headerlen=0;
	uint8_t deviceid[DEVICE_ID_LEN],res;

	headerlen=strlen(ble_uart_at_table[idx].pData);
	zb_tokenizer_init(tzer, data+headerlen, data+len);
	get_bdevice_id(deviceid);

	tok_device_id=zb_tokenizer_get(tzer,0);

	if(0==memcmp(deviceid,tok_device_id.p,DEVICE_ID_LEN))
	{
		res=handle_bdevice_bboxunlock();
		make_bdevice_okerror_response(1,0,0,idx);
	}
	else
	{
		res=make_bdevice_okerror_response(1,0,1,idx);
	}
	return res;
}

/*only alert, no response*/
extern uint8_t dis_data[2];

uint8_t ble_uart_at_handle_bpalert(uint8_t *data, uint16_t len, uint8_t at_index)
{
		
	//printf("ble_uart_at_handle_bpalert \r\n");
	ZbTokenizer  tzer[1];
	ZbToken  tok_device_id,tok_alert,tok_distance;
	uint16_t headerlen=0;
	uint8_t dis_len;
	
	headerlen=strlen(ble_uart_at_table[at_index].pData);
	
	zb_tokenizer_init(tzer, data+headerlen, data+len);
	tok_device_id = zb_tokenizer_get(tzer,0);
	tok_alert = zb_tokenizer_get(tzer,1);
	tok_distance = zb_tokenizer_get(tzer,2);
	memcpy(dis_data,tok_distance.p,2);
	printf("tok_dis is %s ",dis_data);
	
	if(tok_alert.p[0]=='1' && isAlerting==0)
		buzzer_timer_en(1);
	else if(tok_alert.p[0]=='0' && isAlerting==1)
		buzzer_timer_en(0);
	
	//hw_uart2_send(data, len);
}

uint8_t ble_uart_data_handle(uint8_t *data, uint16_t len)
{
	uint8_t i=0,atlen=0;
	
	//if(len<AT_END_TOK_LEN) return 1;
//printf("ble_uart_data_handle1 \r\n");
//	if(memcmp(&data[len-AT_END_TOK_LEN],ble_at_end_tok,AT_END_TOK_LEN)!=0) return 1;
//printf("ble_uart_data_handle 2\r\n");
	for(i=0;i<SUPPORT_AT_NUM;i++)
	{
	
		atlen=strlen(ble_uart_at_table[i].pData);
		if((0==memcmp(data, ble_uart_at_table[i].pData, atlen))&&ble_uart_at_table[i].handle!=NULL){
				
				return ble_uart_at_table[i].handle(data, len,i);
		}
	}
	return 1;
}


uint8_t ble_mail_data_handle(uint8_t *data, uint16_t len)
{
	uint8_t ret;
	return ble_send_response(data,len);
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

uint8_t ble_mail_receive_and_handle(blemail *pmail)
{
	ble_mail_data_handle(pmail->data, pmail->len);
}


void buzzer_timer_en(uint8_t en)
{
  if(en == 1)
	{
		if(isAlerting ==0){
			osTimerStart (BuzzerTimerHandle, 500);//¿ªÊ¼¶¨Ê±
			isAlerting=1;
		}else{
			return ;
		}
	}else{
		if(isAlerting ==1){
			osTimerStop (BuzzerTimerHandle);//Í£Ö¹¶¨Ê±
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
			osTimerStop (BuzzerTimerHandle);//Í£Ö¹¶¨Ê±
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
void ble_test_send_data(void)
{
	hw_uart4_send(ptr, 6);
}

void ble_data_received(void)
{
	uint8_t data =0x31;
	//task_ble_mail_put(&data, 1);
	task_ble_message_put(0x31);
}
/*****************debug code *************/

