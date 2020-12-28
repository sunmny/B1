//#include "includes.h"
//#include "demo.h"
//#include "HW_Gsm.h"
#//include "app_nfc.h"
//#include "app_card.h"
//#include "HW_Pcd.h"
#include "cmsis_os.h"
#include "main.h"
#include "fm175xx.h"
#include "hw_uart.h"
#include "type_a.h"
#include "hw_nfc.h"
#include "mifare_card.h"
#include "hw_config.h"

#define ROW(y)		(16*y)
#define COL(x)		(8*x)
uint16_t PCD_GetCardno(uint8_t *cardno);
static uint8_t u8Uid[10];
static uint8_t u8Ats[20];
//static uint8 u8Capdu[30];
//static uint8 u8Rapdu[300];

#define MONEY_LEN

#define STX 0x02
#define ETX 0x03
#define ROW(y)		(16*y)
#define COL(x)		(8*x)
#define  BUFCLR(szBuf) memset(szBuf, 0, sizeof(szBuf))

uint8_t ucSendBuffer[512];
uint32_t uiSendLen;
uint8_t ucRecvBuffer[512];
uint32_t uiRecvLen;
uint16_t guiSN = 0;
uint8_t guiCMD = 0;
 uint8_t cardno_tmp[20];
 uint8_t yuer[200];
	uint8_t money_t[21]={'\0'};


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
//校准过程中，判断电机过载，用来停止电机
//校准定时器
#define SBP_lock_EVT           0x0080

// How often to check battery (milliseconds)
#define NFC_PERIOD         800




void app_nfc_power(uint8_t en)
{
   if(en)
	  HAL_GPIO_WritePin(NFC_POWER_EN_GPIO_Port, NFC_POWER_EN_Pin, GPIO_PIN_SET);
		else 
		 HAL_GPIO_WritePin(NFC_POWER_EN_GPIO_Port, NFC_POWER_EN_Pin, GPIO_PIN_RESET);
}
void app_nfc_init(void)
{
		app_nfc_power(0);
		osDelay(100);
		app_nfc_power(1);
		osDelay(200);
}
static uint8_t send_buff[200]={0x00};


void app_test(void)
{

uint8_t i = 0;
char ret = 0;

printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n");
//软重启
//printf("SoftReset = 0x%x\r\n",FM175X_SoftReset());
osDelay(500);

//读取版本
// printf("pn512 version = 0x%x\r\n",Read_Reg(VersionReg));
osDelay(10);

//读取FM17XX所有地址
ret = Read_Reg_All(send_buff);
//printf("Read_Reg_All = 0x%x\r\n",ret);
//printf("data =");
for(i = 0;i<65;i++)
{
 // printf(" 0x%x",send_buff[i]);
}
//printf("\r\n");
osDelay(10);



/*
//寻卡 Standard	 send request command Standard mode
Set_Rf(3);   //turn on radio
Pcd_ConfigISOType(0);//a卡
ret = TypeA_Request(PICC_A.ATQA);//寻卡 Standard	 send request command Standard mode
printf("TypeA_Request = 0x%x\r\n",ret);
if(ret == OK)
{
  printf("PICC_A.ATQA = 0x%x 0x%x\r\n",PICC_A.ATQA[0],PICC_A.ATQA[1]);
}
printf("\r\n");
osDelay(50);






ret = TypeA_Halt();
printf("TypeA_Halt = 0x%x\r\n",ret);



//WAKEUP
Set_Rf(3);   //turn on radio
Pcd_ConfigISOType(0);//a卡
ret = TypeA_WakeUp(PICC_A.ATQA);
printf("TypeA_WakeUp = 0x%x\r\n",ret);
if(ret == OK)
{
  printf("PICC_A.ATQA = 0x%x 0x%x\r\n",PICC_A.ATQA[0],PICC_A.ATQA[1]);
}
printf("\r\n");
osDelay(50);
*/


//寻卡，防冲突，选卡
Set_Rf(3);   //turn on radio
printf("Set_Rf \r\n");
Pcd_ConfigISOType(0);//a卡
printf("Pcd_ConfigISOType \r\n");
memset(&PICC_A,0x00,sizeof(struct PICC_A_STR));
ret = TypeA_CardActivate(&PICC_A);
printf("TypeA_CardActivate = 0x%x\r\n",ret);
if(ret == OK)
{
  printf("PICC_A.ATQA = 0x%x 0x%x\r\n",PICC_A.ATQA[0],PICC_A.ATQA[1]);
	
	if(PICC_A.ATQA[0] !=0)
	{
			hw_buzzer_enable(1);
		osDelay(200);
		hw_buzzer_enable(0);
	
	}
	//printf("PICC_A.UID_len = 0x%x\r\n",PICC_A.UID_len);
	//printf("PICC_A.UID = ");
	//for(i = 0;i<PICC_A.UID_len;i++)
  //{
   // printf(" 0x%x",PICC_A.UID[i]);
  //}
  //printf("\r\n");
	//printf("PICC_A.SAK = 0x%x\r\n",PICC_A.SAK);
}

#if 0
osDelay(10);



//Mifare 认证 (非Mifare 不用认证)
send_buff[0]=0;//key a
send_buff[1]=0x00;//扇区1
memset(send_buff+2,0xff,6); //新卡密码要么全0xff要么A0 A1 A2 A3 A4 A5
/*
*(send_buff+2) = 0xA0;
*(send_buff+3) = 0xA1;
*(send_buff+4) = 0xA2;
*(send_buff+5) = 0xA3;
*(send_buff+6) = 0xA4;
*(send_buff+7) = 0xA5;
*/

ret = Mifare_Auth(send_buff[0],send_buff[1],send_buff+2,PICC_A.UID);
printf("Mifare_Auth = 0x%x\r\n",ret);
osDelay(10);




//Mifare 读块 0
memset(send_buff,0x00,16);
ret = Mifare_Blockread(0x00,send_buff);
printf("Mifare_Blockread = 0x%x\r\n",ret);
if(ret == OK)
{
	printf("data = ");
	for(i = 0;i<16;i++)
  {
    printf(" 0x%x",send_buff[i]);
  }
  printf("\r\n");
}
osDelay(10);



//Mifare 写块 2
memset(send_buff,0xaa,16);
ret = Mifare_Blockwrite(0x02,send_buff);
printf("Mifare_Blockwrite = 0x%x\r\n",ret);
if(ret == OK)
{
	printf("data = ");
	for(i = 0;i<16;i++)
  {
    printf(" 0x%x",send_buff[i]);
  }
  printf("\r\n");
}
osDelay(10);

//Mifare 读块 2
memset(send_buff,0x00,16);
ret = Mifare_Blockread(0x02,send_buff);
printf("Mifare_Blockread = 0x%x\r\n",ret);
if(ret == OK)
{
	printf("data = ");
	for(i = 0;i<16;i++)
  {
    printf(" 0x%x",send_buff[i]);
  }
  printf("\r\n");
}
osDelay(10);


printf("===============================================================\r\n");
	
osDelay(500);
#endif
/*
	case 0x0://版本确认
		send_buff.buff[0]=strlen(system_info);
		memcpy(send_buff.buff+1,system_info,send_buff.buff[0]);
		send_buff.length=send_buff.buff[0]+1;
		break;
		
	osDelay(500);

	case 0x1://FM17XX 软复位
		
		send_buff.buff[0]=FM175X_SoftReset();
		send_buff.length=1;
		break;

   	case 0x2://FM17XX 硬复位
		
		send_buff.buff[0]=FM175X_HardReset();
		send_buff.length=1;
		break;

	case 0x3://PC读取FM17XX所有地址
		send_buff.buff[0]=Read_Reg_All(send_buff.buff+1);
		send_buff.length=65;
		break;

	case 0x4://PC读取FM17XX地址
		send_buff.buff[0]=OK;
		send_buff.buff[1]=rece_buff.buff[1];
		send_buff.buff[2]=Read_Reg(rece_buff.buff[1]);
		send_buff.length=3;
		break;

	case 0x5://PC写入FM17XX地址
				
		Write_Reg(rece_buff.buff[1],rece_buff.buff[2]);
		send_buff.buff[0]=0;
		send_buff.length=1;
		break;
	
	case 0x6://读取扩展寄存器	
	 	send_buff.buff[0]=OK;
		send_buff.buff[1]=rece_buff.buff[1];
		send_buff.buff[2]=Read_Ext_Reg(rece_buff.buff[1]);
		send_buff.length=3;
	 	break;

	case 0x7://写入扩展寄存器
			
		send_buff.buff[0]= Write_Ext_Reg(rece_buff.buff[1],rece_buff.buff[2]);
		send_buff.buff[1]=rece_buff.buff[1];
		send_buff.length=2;
		break;	
	
	case 0x8://FM175X_HardPowerdown
		send_buff.buff[0]=FM175X_HardPowerdown();
		send_buff.length=1;
		break;

   	case 0x9://FM175X_SoftPowerdown
		send_buff.buff[0]=FM175X_SoftPowerdown();
		send_buff.length=1;
		break;

	case 0xA://REQUEST
			
		Set_Rf(3);   //turn on radio
		Pcd_ConfigISOType(0);
		send_buff.buff[0]=TypeA_Request(PICC_A.ATQA);//寻卡 Standard	 send request command Standard mode
		send_buff.buff[1]=PICC_A.ATQA[0];
		send_buff.buff[2]=PICC_A.ATQA[1];
		send_buff.length=3;
		break;

	case 0xB://WAKEUP
		Set_Rf(3);   //turn on radio
		Pcd_ConfigISOType(0);
		send_buff.buff[0]=TypeA_WakeUp(PICC_A.ATQA);
		send_buff.buff[1]=PICC_A.ATQA[0];
		send_buff.buff[2]=PICC_A.ATQA[1];
		send_buff.length=3;
		break;

 	case 0xC:// Halt
		send_buff.buff[0]= TypeA_Halt();
		send_buff.length=1;
		break;

	case 0x10://寻卡，防冲突，选卡
		Set_Rf(3);   //turn on radio
		Pcd_ConfigISOType(0);
		send_buff.buff[0]=TypeA_CardActivate(&PICC_A);

		memcpy(send_buff.buff+1,PICC_A.ATQA,2);
		memcpy(send_buff.buff+3,&PICC_A.UID_len,1);
		memcpy(send_buff.buff+4,PICC_A.UID,12);
		memcpy(send_buff.buff+16,&PICC_A.SAK,1);
	   		
		send_buff.length=17;	
		break;
	
	case 0x11://Mifare 认证
		
		send_buff.buff[0]=Mifare_Auth(rece_buff.buff[1],rece_buff.buff[2],rece_buff.buff+3,PICC_A.UID);	//card authentication
		send_buff.length=1;
		break;

	case 0x12://Mifare 读块
		send_buff.buff[0]=Mifare_Blockread(rece_buff.buff[1]+rece_buff.buff[2]*4,send_buff.buff+1);	//bolck read function
		send_buff.length=17;
		break;

	case 0x13://Mifare 写块

		send_buff.buff[0]=Mifare_Blockwrite((rece_buff.buff[1]+rece_buff.buff[2]*4),rece_buff.buff+3);	 //block write function
		send_buff.length=1;
		break;

	case 0x14://Mifare 增值

		send_buff.buff[0]=	Mifare_Blockinc(rece_buff.buff[1],rece_buff.buff+2);
		send_buff.length=1;
		break;
																								    
	case 0x15://Mifare 减值
		send_buff.buff[0]=	Mifare_Blockdec((rece_buff.buff[1]+rece_buff.buff[2]*4),rece_buff.buff+3);
		send_buff.length=1;
		break;

	case 0x16://Mifare transfer
		send_buff.buff[0]=	Mifare_Transfer(rece_buff.buff[1]);
		send_buff.length=1;
		break;

   	case 0x17://Mifare restore
		send_buff.buff[0]=	Mifare_Restore((rece_buff.buff[1]+rece_buff.buff[2]*4));
		send_buff.length=1;
		break;
	 
	case 0x18://Mifare 设置数据块
		send_buff.buff[0]= Mifare_Blockset(rece_buff.buff[1],rece_buff.buff+2);
		send_buff.length=1;
		break;
   					
	case 0x20://RATS
		send_buff.buff[0]=CPU_Rats(rece_buff.buff[1],rece_buff.buff[2],&send_buff.buff[1],send_buff.buff+2);
		send_buff.length=send_buff.buff[1]+2;
		break;
	
	case 0x21://PPS
		send_buff.buff[0]=CPU_PPS(rece_buff.buff[1],rece_buff.buff[2],&send_buff.buff[1],send_buff.buff+2);
		send_buff.length=send_buff.buff[1]+2;
		break;

   	case 0x22://高波特率
		send_buff.buff[0]=CPU_BaudRate_Set(rece_buff.buff[1]);
		send_buff.length=1;
		break;

	case 0x23://TPDU指令

		send_buff.buff[0]=CPU_TPDU(rece_buff.buff[1],rece_buff.buff+2,&send_buff.buff[1],send_buff.buff+2);
		send_buff.length=send_buff.buff[1]+2;
		break;
  
	case 0x24://R_block
		send_buff.buff[0]=CPU_R_Block(&send_buff.buff[1],send_buff.buff+2);
		send_buff.length=send_buff.buff[1]+2;
		break;

	case 0x25://S_block
		send_buff.buff[0]=CPU_S_Block(rece_buff.buff[1],&send_buff.buff[1],send_buff.buff+2);
		send_buff.length=send_buff.buff[1]+2;
		break;

   	case 0x26://I_block
		send_buff.buff[0]=CPU_I_Block(rece_buff.buff[1],rece_buff.buff+2,&send_buff.buff[1],send_buff.buff+2);
		send_buff.length=send_buff.buff[1]+2;
		break;
   	
	case 0x27://随机数
		send_buff.buff[0]=CPU_Random(&send_buff.buff[1],send_buff.buff+2);
		send_buff.length=send_buff.buff[1]+2;
		break;
   
	case 0x31://FM17550 TYPE B request
		Set_Rf(3);   //turn on radio
		Pcd_ConfigISOType(1);
		send_buff.buff[0]=TypeB_Request(&send_buff.buff[1],send_buff.buff+2,&PICC_B);
		send_buff.length=send_buff.buff[1]+2;
		break;

	case 0x32://FM17550 TYPE B attrib
		send_buff.buff[0]=TypeB_Select(PICC_B.PUPI,&send_buff.buff[1],send_buff.buff+2);
		send_buff.length=send_buff.buff[1]+2;
		break;

	case 0x33://FM17550 TYPE B halt
		send_buff.buff[0]=TypeB_Halt(PICC_B.PUPI,&send_buff.buff[1],send_buff.buff+2);
		send_buff.length=send_buff.buff[1]+2;;
		break;
			   
	case 0x34://FM17550 TYPE B get_uid
		send_buff.buff[0]=TypeB_GetUID(&send_buff.buff[1],send_buff.buff+2);
		send_buff.length=send_buff.buff[1]+2;
		break;

	case 0x35://FM17550 TYPE B wakeup
		Set_Rf(3);   //turn on radio
		Pcd_ConfigISOType(1);
	  	send_buff.buff[0]=TypeB_WUP(&send_buff.buff[1],send_buff.buff+2,&PICC_B);
		send_buff.length=send_buff.buff[1]+2;
		break;
	
	case 0x52://ultralight_read
		send_buff.buff[0]=Ultra_Read(rece_buff.buff[1],send_buff.buff+1);
		send_buff.length=5;
		break;

	case 0x53://ultralight_write
		send_buff.buff[0]=Ultra_Write(rece_buff.buff[1],rece_buff.buff+2);
		send_buff.length=1;
		break;

 	case 0x60:
		Set_Rf(3);   //turn on radio
		Pcd_ConfigISOType(2);
		send_buff.buff[0]=TypeF_Polling(rece_buff.buff[1],&send_buff.buff[1],send_buff.buff+2);
		send_buff.length=send_buff.buff[1]+2;
		break;
*/


}