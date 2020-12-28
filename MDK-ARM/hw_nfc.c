#include "hw_i2c.h" 
#include "hw_config.h"
#include "stm32l4xx_hal.h"
#include "app_ble.h"

uint8_t pn512_write(uint8_t *sdata,uint8_t len)
{
	//uint8_t data[2] ;
	uint8_t ret ;
	//	data[0] = point_reg;
	//	data[1] =  *w_pdata;
	
	ret = hw_i2c_write( 0x00, 0x50, sdata, len);
	if(ret)
			return 1;
		
		return 0;
	
}


uint8_t pn512_read(uint8_t reg,uint8_t *rdata,uint8_t len)
{

	uint8_t data ,ret;
	
		data =reg;

	
 ret = hw_i2c_write( 0x00, 0x50, &data,  1);
	if(ret){
			return 1;
		}
 ret = hw_i2c_read(0x00,0x51,rdata,len);
	if(ret){
			return 1;
	}
	//printf("w_pdata = %x \r\n",*w_pdata);
	return 0;
}
static void delay_ms(uint16_t ms)
{
			int i,j;
			for(i = 0;i<1000;i++){
					for(j = 0;j<ms;j++);
			}
}

void pn512_init(void )
{
		int ret;
		uint8_t reg =0;
		uint8_t send_data[2]= {0x00,0x81};
	//PCD_Open();
	//ret=  PCD_Detect();
		ft_dev.nfc_flags = 0;
	pn512_write(send_data,0x02);
			delay_ms(1000);
	pn512_read(0x00,&reg,1);
			delay_ms(1000);
	//	pn512_read(0x1f,&reg);
		
		if(reg == 0x81){
		
			ft_dev.nfc_flags = 1;
		}
		printf("512ret = %d reg = %x\r\n",ret,reg);
	
}