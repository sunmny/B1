#include "hw_uart.h" 
#include "stm32l4xx_hal.h"
//#include "cmsis_os.h"
#include "string.h"
//#include "delay.h"
#include "app_lte.h"
#include "hw_config.h"
#include "app_ble.h"
#define SUPPORT_AT_NUM    5

uint8_t lte_uart_data_loop[LTE_DATA_LOOP_SIZE+1];
extern  UART_HandleTypeDef huart1;
uint16_t lte_uart_data_loop_len;
extern  UART_HandleTypeDef huart1;
extern  uint8_t report_data[128];
#define AT_END_TOK_LEN 4
static const uint8_t lte_at_end_tok[AT_END_TOK_LEN] = {0x23,0x23,0x0d,0x0a};//##\r\n
static void delay_ms(uint32_t ms)
{	
		uint32_t i ,j;
		
		for(i =0;i<ms ;i++){
					for(j =0;j<10000;j++);
		
		}


}
extern uint8_t tcp_buf[39];
extern uint8_t ping_buf[50];
extern void set_moto_work(uint8_t enable);
uint8_t lte_count =1;
extern uint8_t lte_network_flag;
extern uint8_t tcp_connect_flag;
extern uint8_t phone_num_buf[11];
void LteTimerCallback(void const * argument)
{
	
	
			//	if((lte_count %10) == 0){
						//	if(tcp_connect_flag){
						//	HAL_UART_Transmit(&huart1,(uint8_t *)"AT+CIPSEND=0,76\r\n", 19,0xffff);
								//	osDelay(2000);
							//	HAL_UART_Transmit(&huart1,report_data,76,0xffff);
							//	printf("report_data %s \r\n",report_data);
							//}
					
				//}
		//		set_nvram_id_data(phone_num_buf);
			//set_nvram_id_data(phone_num_buf);
			if(lte_count ==15){
					send_bd1_get_card_num();
				}
		
				if(lte_network_flag ==1){
								set_rgb_lte_led_red_close();
							if(lte_count%2 ==1){
									set_rgb_lte_led_green();
							}else{
									set_rgb_lte_led_green_close();
							}
				}else{
						if(lte_count%2 ==1){
									set_rgb_lte_led_red();
							}else{
									set_rgb_lte_led_red_close();
							}
				}
		
	
	
		lte_count++;
		if(lte_count >100){
				lte_count =1;
		}

	#if 0
	HAL_UART_Transmit(&huart1, (uint8_t *)"ATS0=3\r\n", 13, 0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", 4, 0xFFFF);
	delay_ms(2000);
	HAL_UART_Transmit(&huart1,(uint8_t *)"AT+CIPSEND=0,83\r\n", 19,0xffff);
		delay_ms(1000);
	HAL_UART_Transmit(&huart1,report_data,83,0xffff);
	delay_ms(500);
	HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CIPCLOSE=0\r\n", 17, 0xFFFF);
	#endif
}
static const lte_at_symbol lte_at_table[SUPPORT_AT_NUM] = {
	{(uint8_t *)"AT+ZBEAT?"},
	{(uint8_t *)"AT+BBROADCAST?"},
	{(uint8_t *)"AT+BBIND="},
	{(uint8_t *)"AT+BBEAT="},
	{(uint8_t *)"AT+BDISTANCE="},
};

int16_t lte_at_handle(uint8_t *data, uint16_t len, uint8_t at_index)
{
	switch (at_index)
	{
		case 0:
			break;
		case 1:
			break;
		default:
			break;
	}
}

int16_t lte_data_handle(uint8_t *data, uint16_t len)
{
	uint8_t i=0,atlen=0;
	
	if(len<AT_END_TOK_LEN) return -1;

	if(memcmp(&data[len-AT_END_TOK_LEN],lte_at_end_tok,AT_END_TOK_LEN)!=0) return -1;

	for(i=0;i<SUPPORT_AT_NUM;i++)
	{
		atlen=strlen(lte_at_table[i].pData);
		if(0==memcmp(data, lte_at_table[i].pData, atlen))
			return lte_at_handle(data, len, i);
	}
	return -1;
}



uint8_t send_failed_count;

void lte_receive_and_handle(void)
{
	uint8_t i =0,len = 0,lenth =0,lenth1,lenth2 ,lenth3,lenth5,lenth6,lenth7,lenth8,lenth9,temp[30]={0};
	
	memset(lte_uart_data_loop,0x00,sizeof(lte_uart_data_loop));
	hw_uart1_receive(lte_uart_data_loop, &lte_uart_data_loop_len, 0xff);
	printf("lte rece :%d %s \r\n",lte_uart_data_loop_len,lte_uart_data_loop);
	
	if(lte_uart_data_loop_len ==68){
		send_failed_count ++;
	}else if(lte_uart_data_loop_len ==95){
		send_failed_count =0;
	
	}
	if (lte_uart_data_loop_len>0)
	{
		
		len = strlen("+CME ERROR: SIM not inserted");
		lenth = strlen("+CPIN: READY");
		lenth1=strlen("PB DONE");
		
		memcpy(temp,&lte_uart_data_loop[2],len);
		printf("temp %s \r\n",temp);
		if(0 == memcmp("+CME ERROR: SIM not inserted",temp,len)){
				//task_test_mail_put("nosim",5);
					//printf("sim is no insterted\r\n");
					memset(temp,0x00,30);
		}else if(0 == memcmp("+CPIN: READY",temp,lenth)){
				//task_test_mail_put("insim",5);
			lte_network_flag =1;
					//printf("sim is insterted\r\n");
			memset(temp,0x00,30);
		}else if(0 == memcmp("PB DONE",temp,lenth1)){
				//task_test_mail_put("connect",7);
			lte_network_flag =1;
				printf("connect\r\n");
			memset(temp,0x00,30);
		}else if(0== memcmp("+CIPERROR: 4",temp,12)){
		
			send_failed_count ++;
			printf("lte send failed %d \r\n",send_failed_count);
		}else if(0== memcmp("+CIPSEND: 0,68,68",temp,17)){
			send_failed_count =0;
		}

	
}
}
void lte_send_data(void)
{
	uint8_t *ptr="abcdefgh";
	hw_uart1_send(ptr, 8);
}

