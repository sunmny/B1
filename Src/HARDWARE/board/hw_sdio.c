#include "hw_led.h" 
#include "hw_config.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com							  
////////////////////////////////////////////////////////////////////////////////// 	 

//��ʼ��PA6��PA7Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void hardware_led_init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOAʱ��
	
  //GPIOF9,F10��ʼ������
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����

	GPIO_InitStructure.GPIO_Pin = LED_KEY_R1_PIN | LED_KEY_R2_PIN | LED_CHARGE_R_PIN;//LED0��LED1��ӦIO��
  GPIO_Init(LED_KEY_R1_PORT, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = LED_CHARGE_G_PIN;
	GPIO_Init(LED_CHARGE_G_PORT, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_SetBits(LED_KEY_R1_PORT,LED_KEY_R1_PIN | LED_KEY_R2_PIN | LED_CHARGE_R_PIN);//���øߣ�����
	GPIO_SetBits(LED_CHARGE_G_PORT,LED_CHARGE_G_PIN);//���øߣ�����
	

}

uint8_t hardware_led_get(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
     return GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin);
}


void hardware_led_set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint8_t level)
{
	if(level == ON)
		GPIO_SetBits(GPIOx,GPIO_Pin);
	else
		GPIO_ResetBits(GPIOx,GPIO_Pin);	
}







