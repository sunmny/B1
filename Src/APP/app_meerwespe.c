#include "myconfig.h"
#include "gui_meerwespe.h"
#include "app_meerwespe.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "hw_meerwespe.h"
#include "hw_config.h"

#include "gui_message.h"
#include "hw_charge.h"
#include "hw_pwm.h"
#include "app_power.h"
#include "delay.h"
#include "app_lcd.h"
#include "task_lora.h"
extern  UART_HandleTypeDef huart1;
extern  UART_HandleTypeDef huart2;
extern  UART_HandleTypeDef huart4;
WORK_MODE       work_mode       = main_page;
WORK_SEND_MODE  work_send_mode  = no_send;
GPS_MODE        gps_mode        = gps_close;
LORA_MODE       lora_mode       = lora_close;
static void delay_ms(uint32_t ms)
{	
		uint32_t i ,j;
		
		for(i =0;i<ms ;i++){
					for(j =0;j<10000;j++);
		
		}


}

uint8_t beddou1[5] = {0x24,0x47,0x4c,0x4a,0x43};

uint8_t tcp_buf[39];
uint8_t apn_buf[50];
uint8_t apn_buf1[81];
uint8_t ping_buf[50];
//extern  uint8_t report_data[128];
//extern  uint8_t report_data1[200];
//uint8_t apn_buf[70] ={"hellrelelelelelel"};

void lte_close_tcp_network(void)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CIPCLOSE=0\r\n", 17, 0xFFFF);
	osDelay(2000);

}
void lte_close_connect_network(void)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)"AT+NETCLOSE\r\n", 15, 0xFFFF);
	osDelay(2000);

}

void lte_open_connect_network(void)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)"AT+NETOPEN\r\n", 14, 0xFFFF);
	osDelay(2000);

}
void lte_tcp_connect_network(void)
{
			HAL_UART_Transmit(&huart1, tcp_buf, 38, 0xFFFF);
	    HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", 4, 0xFFFF);
		osDelay(2000);
	//	HAL_UART_Transmit(&huart1,(uint8_t *)"AT+CIPSEND=0,80\r\n", 19,0xffff);
		//osDelay(2000);
		//HAL_UART_Transmit(&huart1,report_data,80,0xffff);
}
extern uint8_t response_location[128];
void lte_tcp_send_data(void)
{
		//printf("report_data1 = %s \r\n",report_data1);
		HAL_UART_Transmit(&huart1,(uint8_t *)"AT+CIPSEND=0,68\r\n",19,0xffff);
		osDelay(2000);
		HAL_UART_Transmit(&huart1,response_location,68,0xffff);
		osDelay(2000);
	//HAL_UART_Transmit(&huart1,(uint8_t *)"AT+CIPCLOSE=0\r\n", 17,0xffff);
	 
}
extern uint8_t phone_num_buf[11];
void apn_init(void)
{

		apn_buf[0] = 'A';
			apn_buf[1] = 'T';
			apn_buf[2] = '+';
			apn_buf[3] = 'C';
			apn_buf[4] = 'G';
			apn_buf[5] = 'A';
			apn_buf[6] = 'U';
			apn_buf[7] = 'T';
			apn_buf[8] = 'H';
			apn_buf[9] = '=';
			apn_buf[10] = ',';
			apn_buf[11] = ',';
			apn_buf[12] = '"';
			apn_buf[13] = '1';
			apn_buf[14] = '8';
			apn_buf[15] = '0';
			apn_buf[16] = '0';
			apn_buf[17] = '1';
			apn_buf[18] = '3';
			apn_buf[19] = '1';
			
			apn_buf[20] = '6';
			apn_buf[21] = '6';
			apn_buf[22] = '6';
			apn_buf[23] = '6';
			
			apn_buf[24] = '@';
			
			apn_buf[25] = 'z';
			apn_buf[26] = 'b';
			apn_buf[27] = 'j';
			apn_buf[28] = 't';
			apn_buf[29] = 'j';
			apn_buf[30] = '.';
			apn_buf[31] = 'v';
			apn_buf[32] = 'p';
			apn_buf[33] = 'd';
			apn_buf[34] = 'n';
			apn_buf[35] = '.';
			apn_buf[36] = 'b';
			apn_buf[37] = 'j';
			apn_buf[38] = '"';
			apn_buf[39] = ',';
			apn_buf[40] = '"';
			apn_buf[41] = 'z';
			apn_buf[42] = 'b';
			apn_buf[43] = 'j';
			apn_buf[44] = 't';
			apn_buf[45] = 'j';
			apn_buf[46] = '1';
			apn_buf[47] = '2';
			apn_buf[48] = '3';
			apn_buf[49] = '"';

		if(phone_num_buf[0] == '1')
			 memcpy(&apn_buf[13],phone_num_buf,11);
}
void lte_init_network(void)
{
			



			apn_buf1[0] = 'A';
			apn_buf1[1] = 'T';
			apn_buf1[2] = '+';
			apn_buf1[3] = 'C';
			apn_buf1[4] = 'G';
			apn_buf1[5] = 'D';
			apn_buf1[6] = 'C';
			apn_buf1[7] = 'O';
			apn_buf1[8] = 'N';
			apn_buf1[9] = 'T';
			apn_buf1[10] = '=';
			apn_buf1[11] = '1';
			apn_buf1[12] = ',';
			apn_buf1[13] = '"';
			apn_buf1[14] = 'I';
			apn_buf1[15] = 'P';
			apn_buf1[16] = 'V';
			apn_buf1[17] = '4';
			apn_buf1[18] = 'V';
			apn_buf1[19] = '6';
			apn_buf1[20] = '"';
			apn_buf1[21] = ',';
			apn_buf1[22] = '"';
			apn_buf1[23] = 'p';
			apn_buf1[24] = 'r';
			apn_buf1[25] = 'i';
			apn_buf1[26] = 'v';
			apn_buf1[27] = 'a';
			apn_buf1[28] = 't';
			apn_buf1[29] = 'e';
			apn_buf1[30] = '.';
			apn_buf1[31] = 'v';
			apn_buf1[32] = 'p';
			apn_buf1[33] = 'd';
			apn_buf1[34] = 'n';
			apn_buf1[35] = '.';
			apn_buf1[36] = 'b';
			apn_buf1[37] = 'j';
			apn_buf1[38] = '"';
			apn_buf1[39] = ',';
			apn_buf1[40] = '"';
			apn_buf1[41] = '0';
			apn_buf1[42] = '.';
			apn_buf1[43] = '0';
			apn_buf1[44] = '.';
			apn_buf1[45] = '0';
			apn_buf1[46] = '.';
			apn_buf1[47] = '0';
			apn_buf1[48] = '.';
			apn_buf1[49] = '0';
			apn_buf1[50] = ',';
			apn_buf1[51] = '0';
			apn_buf1[52] = '.';
			apn_buf1[53] = '0';
			apn_buf1[54] = '.';
			
			apn_buf1[55] = '0';
			apn_buf1[56] = '.';
			apn_buf1[57] = '0';
			apn_buf1[58] = '.';
			apn_buf1[59] = '0';
			apn_buf1[60] = '.';
			apn_buf1[61] = '0';
			apn_buf1[62] = '.';
			apn_buf1[63] = '0';
			apn_buf1[64] = '.';
			apn_buf1[65] = '0';
			apn_buf1[66] = '.';
			apn_buf1[67] = '0';
			apn_buf1[68] = '.';
			apn_buf1[69] = '0';
			
			apn_buf1[70] = '.';
			apn_buf1[71] = '0';

				apn_buf1[72] = '"';
			apn_buf1[73] = ',';
				apn_buf1[74] = '0';
			apn_buf1[75] = ',';
				apn_buf1[76] = '0';
			apn_buf1[77] = ',';
				apn_buf1[78] = '0';
			apn_buf1[79] = ',';
			
				apn_buf1[80] = '0';
			
			
			tcp_buf[0]='A';
			tcp_buf[1]='T';
			tcp_buf[2]='+';
			tcp_buf[3]='C';
			tcp_buf[4]='I';
			tcp_buf[5]='P';
			tcp_buf[6]='O';
			tcp_buf[7]='P';
			tcp_buf[8]='E';
			tcp_buf[9]='N';
			tcp_buf[10]='=';
			tcp_buf[11]='0';
			tcp_buf[12]=',';
			tcp_buf[13]='"';
			tcp_buf[14]='T';
			tcp_buf[15]='C';
			tcp_buf[16]='P';
			tcp_buf[17]='"';
			tcp_buf[18]=',';
			tcp_buf[19]='"';
			tcp_buf[20]='5';
			tcp_buf[21]='9';
			tcp_buf[22]='.';
			tcp_buf[23]='2';
			tcp_buf[24]='5';
			tcp_buf[25]='5';
			tcp_buf[26]='.';
			tcp_buf[27]='1';
			tcp_buf[28]='0';
			tcp_buf[29]='2';
			tcp_buf[30]='.';
			tcp_buf[31]='5';
			tcp_buf[32]='"';
			tcp_buf[33]=',';
			tcp_buf[34]='9';
			tcp_buf[35]='1';
			tcp_buf[36]='3';
			tcp_buf[37]='5';

			ping_buf[0] = 'A';
			ping_buf[1] = 'T';
			ping_buf[2] = '+';
			ping_buf[3] = 'C';
			ping_buf[4] = 'P';
			ping_buf[5] = 'I';
			
			ping_buf[6] = 'N';
			ping_buf[7] = 'G';
			ping_buf[8] = '=';
			ping_buf[9] = '"';
			ping_buf[10] = '5';
			ping_buf[11] = '9';
			ping_buf[12] = '.';
			ping_buf[13] = '2';
			ping_buf[14] = '5';
			ping_buf[15] = '5';
			ping_buf[16] = '.';
			ping_buf[17] = '1';
			ping_buf[18] = '0';
			ping_buf[19] = '2';
			ping_buf[20] = '.';
			ping_buf[21] = '5';

			ping_buf[22] = '"';
			ping_buf[23] = ',';
			ping_buf[24] = '1';
			ping_buf[25] = ',';
			ping_buf[26] = '4';
			ping_buf[27] = ',';
			ping_buf[28] = '6';
			ping_buf[29] = '4';
			ping_buf[30] = ',';
			ping_buf[31] = '1';
			ping_buf[32] = '0';
			ping_buf[33] = '0';
			ping_buf[34] = '0';
			ping_buf[35] = ',';
			ping_buf[36] = '1';
			ping_buf[37] = '0';
			ping_buf[38] = '0';
			ping_buf[39] = '0';
			ping_buf[40] = '0';
			ping_buf[41] = ',';
			ping_buf[42] = '2';
			ping_buf[43] = '5';
			ping_buf[44] = '5';
	
	
		HAL_UART_Transmit(&huart1, apn_buf1, 81, 0xFFFF);
		HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", 4, 0xFFFF);
		osDelay(2000);
	
	printf("apn_buf :%s \r\n",apn_buf);
		HAL_UART_Transmit(&huart1, apn_buf, 50, 0xFFFF);
		HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", 4, 0xFFFF);
	
		osDelay(2000);
		HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CGATT?\r\n", 13, 0xFFFF);
		osDelay(2000);
		HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CSOCKSETPN=1\r\n", 19, 0xFFFF);
		osDelay(2000);
		HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CIPMODE=0\r\n", 16, 0xFFFF);
		osDelay(2000);
		HAL_UART_Transmit(&huart1, (uint8_t *)"AT+NETOPEN\r\n", 14, 0xFFFF);
		osDelay(5000);
		HAL_UART_Transmit(&huart1, ping_buf, 45, 0xFFFF);
		HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", 4, 0xFFFF);
		osDelay(5000);
		
		HAL_UART_Transmit(&huart1, (uint8_t *)"AT+NETOPEN\r\n", 14, 0xFFFF);
			
	
}
void hw_lte_init(void)
{

	
	HAL_GPIO_WritePin(LTE_POWERON_IND_GPIO_Port, LTE_POWERON_IND_Pin, GPIO_PIN_SET);
	osDelay(500);
	
	HAL_GPIO_WritePin(LTE_POWERON_IND_GPIO_Port, LTE_POWERON_IND_Pin, GPIO_PIN_RESET);
	printf("hw_lte_init\r\n");


	printf("hw_lte_init ok\r\n");
		osDelay(500);
	//HAL_UART_Transmit(&huart1, (uint8_t *)"ATS0=3\r\n", 13, 0xFFFF);
	
	apn_init();

}

uint8_t bat_val1,bat_val2;
extern uint8_t response_location[128];
extern uint8_t bat_zdev_soc;
uint8_t app_init(void)
{


	hw_gpio_init();
	hw_dma_init();
	hw_uart_init();
	hw_i2c_init();
	tim_init();
	delay_ms(100);
  cw_init();
	app_power_open();
	#if 0
	while(1){
   if(bat_val1 >= 0x2b ){
		app_power_open();
		 break;
		}else{
				zdev_read_battery_adc();
		}
	 //else if(bat_val1 <0x2b || bat_val1 == 0){
	//	hw_buzzer_enable(0);
			//app_power_close() ;
			//return 1;
			
	//	}
	}
	#endif
	hw_buzzer_enable(1);
	exti_init();
  hw_bd2_init();
	printf("bd2_init ok\r\n");

	
  hw_bd1_init();
	set_rd_led_red();
	printf("bd1_init ok\r\n");
	delay_ms(100);
	hw_buzzer_enable(0);
		
	//memset(report_data,0x00,128);
	memset(response_location,0x00,128);
//HAL_UART_Transmit(&huart4,"hello\r\n",9, 0xFFFF);
	return 0;
		
}


char *strchr2(char *s,char c,uint16_t count)
  {
		    uint16 i =0;
//     char*p=s;
    //  while(*s && *s!=c)
		while(*s)
		{
		//	if(*s!=c)
     //     s++;
     if(*s==c)
		        i++;
	      if(i==count)
         return s;
				s++;
			}
				return NULL; 
 }
//s2是否在s1  
 char strstr3(char *s1,char *s2)  
{  
    int n;  
    if (*s2)                      //??????  
    {  
        while(*s1)                 
        {  
            for (n=0;*(s1+n)==*(s2+n);n++)  
            {  
                if (!*(s2+n+1))            
                {  
                    return 0;  
                }  
            }  
            s1++;  
        }  
        return NULL;  
    }  
    else  
    {  
        return 1;  
    }  
} 
 
 
//s2在s1中第一次出现的我位置  空格作为结束

char *strstr2(char *s1,char *s2)  
{  
    int n;  
    if (*s2)                      //??????  
    {  
        while(*s1)                 
        {  
            for (n=0;*(s1+n)==*(s2+n);n++)  
            {  
                if (!*(s2+n+1))            //???????????'\0'  
                {  
                    return (char*)s1;  
                }  
            }  
            s1++;  
        }  
        return NULL;  
    }  
    else  
    {  
        return (char*)s1;  
    }  
} 


void * memcpy2(char *strDest, const char *strSrc, int Count)
{
	    unsigned char *tempDest;
    unsigned char *tempSrc;
    if(strDest == NULL || strSrc == NULL || Count <= 0) return NULL;

    tempDest = (unsigned char *)tempDest;
    tempSrc = (unsigned char *)tempSrc;
    while(Count--)
    {
        *tempDest = *tempSrc;
        tempDest++;
        tempSrc++;
    }
    return strDest;
}

uint16_t strlen2(const char *str) {

	uint8 len =0;
    //assert(str != NULL);
 if(str ==NULL)
	 return 0;
	
   

    while (*str++ != '\0') {

        ++len;

    }

    return len;

}




const char *my_strstr(const char *str, const char *sub_str)
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        int tem = i; ////tem±￡á??÷′??Dμ??eê??D????±ê???? 
        int j = 0;
        while(str[i++] == sub_str[j++])
        {
            if(sub_str[j] == '\0')
            {
                return &str[tem];
            }
        }
        i = tem;
    }

    return NULL;
}

