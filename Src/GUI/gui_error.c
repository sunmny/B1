//#include "includes.h"
//#include "gui.h"

/*
void gui_err(uint8 err)
{
	 Event_Type evt;
	
   switch(err)
	 {
		 case ERR_USER_CHECK:
          while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	 
	     	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	    	 lcd_showstring(64-MESSAGE_FONT_SIZE*4, 25, MESSAGE_FONT_SIZE, "�������,����������");
		     lcd_showstring(104, 52, FONT12, "ȷ��");
         lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
	    	 break;
		 case ERR_CARD_PWD:
         while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*4, 25, MESSAGE_FONT_SIZE, "���������,����������");
		     lcd_showstring(104, 52, FONT12, "ȷ��");
         lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
		     break;
		 case ERR_GSM_CONNECT:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*3, 25, MESSAGE_FONT_SIZE, "�������Ӵ���");
         lcd_showstring(104, 52, FONT12, "ȷ��");
		     lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
		  case ERR_GSM_SENDDATA:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*4, 25, MESSAGE_FONT_SIZE, "�������");
         lcd_showstring(104, 52, FONT12, "ȷ��");
			   lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
			 case ERR_GSM_CARDNO:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*3, 25, MESSAGE_FONT_SIZE, "����δע��");
         lcd_showstring(104, 52, FONT12, "ȷ��");
			   lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
			  case ERR_GSM_SQL:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*4, 25, MESSAGE_FONT_SIZE, "���������ݿ����");
         lcd_showstring(104, 52, FONT12, "ȷ��");
				 lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
				case ERR_GSM_MONEYLOW:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*2, 25, MESSAGE_FONT_SIZE, "����");
         lcd_showstring(104, 52, FONT12, "ȷ��");
				 lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
		case ERR_GSM_TIMEOUT:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*2, 25, MESSAGE_FONT_SIZE, "���ӳ�ʱ");
         lcd_showstring(104, 52, FONT12, "ȷ��");
		     lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
		case ERR_GSM_RECEIVE:
			    while(xQueueReceive(g_queue_lcd, &evt, portMAX_DELAY) != pdPASS);	  
	    	 taskENTER_CRITICAL();
  	     lcd_clean_nostateinfo();
	       lcd_showstring(64-MESSAGE_FONT_SIZE*3, 25, MESSAGE_FONT_SIZE, "Ӧ�����ݴ���");
         lcd_showstring(104, 52, FONT12, "ȷ��");
		     lcd_refresh();
	    	 taskEXIT_CRITICAL();
         xQueueSend(g_queue_lcd, &evt, NULL);
			   break;
		
		
		
		 default:
	     break;
	}
}
*/
