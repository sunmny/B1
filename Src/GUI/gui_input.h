#ifndef _GUI_INPUT_H
#define _GUI_INPUT_H
#include "type.h"
	
//��������
extern uint8 user_pwd_nb;
extern uint32 user_pwd_refresh_nb;
extern uint8 user_pwd_input_enter;


	
	
//�������Ա����
void GUI_userpwd_input(void);

//������
uint8 GUI_money_input(uint8* money);

#endif