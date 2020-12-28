#ifndef __SX1276__H__
#define __SX1276__H__

#define REG_LR_FIFO                                  0x00 
 // Common settings
#define REG_LR_OPMODE                                0x01 
#define REG_LR_BANDSETTING                           0x04
#define REG_LR_FRFMSB                                0x06 
#define REG_LR_FRFMID                                0x07
#define REG_LR_FRFLSB                                0x08 
 // Tx settings
#define REG_LR_PACONFIG                              0x09 
#define REG_LR_PARAMP                                0x0A 
#define REG_LR_OCP                                   0x0B 
 // Rx settings
#define REG_LR_LNA                                   0x0C 
 // LoRa registers
#define REG_LR_FIFOADDRPTR                           0x0D 
#define REG_LR_FIFOTXBASEADDR                        0x0E 
#define REG_LR_FIFORXBASEADDR                        0x0F 
#define REG_LR_FIFORXCURRENTADDR                     0x10 
#define REG_LR_IRQFLAGSMASK                          0x11 
#define REG_LR_IRQFLAGS                              0x12 
#define REG_LR_NBRXBYTES                             0x13 
#define REG_LR_RXHEADERCNTVALUEMSB                   0x14 
#define REG_LR_RXHEADERCNTVALUELSB                   0x15 
#define REG_LR_RXPACKETCNTVALUEMSB                   0x16 
#define REG_LR_RXPACKETCNTVALUELSB                   0x17 
#define REG_LR_MODEMSTAT                             0x18 
#define REG_LR_PKTSNRVALUE                           0x19 
#define REG_LR_PKTRSSIVALUE                          0x1A 
#define REG_LR_RSSIVALUE                             0x1B 
#define REG_LR_HOPCHANNEL                            0x1C 
#define REG_LR_MODEMCONFIG1                          0x1D 
#define REG_LR_MODEMCONFIG2                          0x1E 
#define REG_LR_SYMBTIMEOUTLSB                        0x1F 
#define REG_LR_PREAMBLEMSB                           0x20 
#define REG_LR_PREAMBLELSB                           0x21 
#define REG_LR_PAYLOADLENGTH                         0x22 
#define REG_LR_PAYLOADMAXLENGTH                      0x23 
#define REG_LR_HOPPERIOD                             0x24 
#define REG_LR_FIFORXBYTEADDR                        0x25
#define REG_LR_MODEMCONFIG3                          0x26
 // end of documented register in datasheet
 // I/O settings
#define REG_LR_DIOMAPPING1                           0x40
#define REG_LR_DIOMAPPING2                           0x41
 // Version
#define REG_LR_VERSION                               0x42
 // Additional settings
#define REG_LR_PLLHOP                                0x44
#define REG_LR_TCXO                                  0x4B
#define REG_LR_PADAC                                 0x4D
#define REG_LR_FORMERTEMP                            0x5B
#define REG_LR_BITRATEFRAC                           0x5D
#define REG_LR_AGCREF                                0x61
#define REG_LR_AGCTHRESH1                            0x62
#define REG_LR_AGCTHRESH2                            0x63
#define REG_LR_AGCTHRESH3                            0x64


#define GPIO_VARE_1                                  0X00
#define GPIO_VARE_2                                  0X00
#define RFLR_MODEMCONFIG2_SF_MASK                    0x0f
#define RFLR_MODEMCONFIG1_CODINGRATE_MASK            0xF1 
#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK          0xFB 
#define RFLR_MODEMCONFIG1_BW_MASK                    0x0F 
#define RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK        0xFE 
#define RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK        0xfc
#define RFLR_MODEMCONFIG3_MOBILE_NODE_MASK           0xF7 

#define TIME_OUT_INT                                 0x80 
#define PACKET_RECVER_INT                            0x40 
#define CRC_ERROR_INT                                0x20 
#define RECVER_HEAR_INT                              0x10 
#define FIFO_SEND_OVER                               0x08 
#define RFLR_IRQFLAGS_CAD                            0x04 
#define RFLR_IRQFLAGS_FHSS                           0x02 
#define RFLR_IRQFLAGS_CADD                           0x01 

#define IRQN_TXD_Value                               0xF7
#define IRQN_RXD_Value                               0x9F
#define IRQN_CAD_Value                               0xFA
#define IRQN_SLEEP_Value                             0xFF
#define PACKET_MIAX_Value                            0xff


typedef enum 
{
   Sleep_mode	        = (unsigned char)0x00, 
   Stdby_mode	        = (unsigned char)0x01, 
   TX_mode 	        = (unsigned char)0x02,
   Transmitter_mode	= (unsigned char)0x03,
   RF_mode 		= (unsigned char)0x04,
   Receiver_mode	= (unsigned char)0x05,
   receive_single	= (unsigned char)0x06,
   CAD_mode		= (unsigned char)0x07,
}RFMode_SET;

typedef enum 
{
   FSK_mode             = (unsigned char)0x00, 
   LORA_mode            = (unsigned char)0x80, 
}  Debugging_fsk_ook;

typedef enum{false=0,true=1}BOOL;

typedef enum{enOpen,enClose}cmdEntype_t;

typedef enum{rxOpen,txOpen}cmdpaType_t;

typedef struct{
   void (*lpByteWritefunc)(unsigned char src);
   unsigned char (*lpByteReadfunc)();
   void (*lpSwitchEnStatus)(cmdEntype_t cmd);
   void (*paSwitchCmdfunc)(cmdpaType_t cmd);
   //void (*lpRecvDataTousr)(unsigned char *lpbuf,unsigned short length);
}lpCtrlTypefunc_t;

void register_rf_func(lpCtrlTypefunc_t *func);
void PA_SEELP_OUT(void);
void Delay1s(unsigned int ii);
void SX1276WriteBuffer(unsigned char addr, unsigned char buffer);
unsigned char SX1276ReadBuffer( unsigned char addr);
void SX1276LoRaSetOpMode(RFMode_SET opMode);
void SX1276LoRaFsk(Debugging_fsk_ook opMode);
void SX1276LoRaSetRFFrequency(void);
void SX1276LoRaSetRFPower(unsigned char power);
void SX1276LoRaSetSpreadingFactor(unsigned char factor);
void SX1276LoRaSetNbTrigPeaks(unsigned char value);
void SX1276LoRaSetErrorCoding(unsigned char value);
void SX1276LoRaSetPacketCrcOn(BOOL enable);
void SX1276LoRaSetSignalBandwidth(unsigned char bw);
void SX1276LoRaSetImplicitHeaderOn(BOOL enable);
void SX1276LoRaSetSymbTimeout(unsigned int value);
void SX1276LoRaSetPayloadLength(unsigned char value);
void SX1276LoRaSetPreamLength(unsigned int value);
void SX1276LoRaSetMobileNode(BOOL enable);
void SLEEPLORA_INT(void);
void FUN_RF_SENDPACKET(unsigned char *RF_TRAN_P,unsigned char LEN);
void RF_RECEIVE (void);
void RF_CAD_RECEIVE (void);
void RF_SEELP(void);
void SX1276LORA_INT(void);

/*�����ⲿ���ú���*/
void SX1278_Interupt(void);
void Lora_Init(void);
void Lora_Send(unsigned char *p_send_buf,unsigned char len);
void Lora_RecDataGet(unsigned char *p_recdata,unsigned char len);
unsigned char Lora_GetNumofRecData(void);

#endif



