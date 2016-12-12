#ifndef _UTILS_H

#define _UTILS_H


#include "lrs.h"

#include "utils.h"



//ִ������ �����Ӧ��16������Ϣ���ݴ洢�ڲ���str_parameter�� commandsAmount��ʾ��Ϣ������
int performTransaction(char* transactionName, char* str_parameter, int commandsAmount )
{

	int i = 0;
	char sendStr[512];


	lr_start_transaction(transactionName);


	for (i = 0 ; i < commandsAmount ; i++) 
	{

		strcpy(sendStr,lr_eval_string(str_parameter));  
		lrs_set_send_buffer("socket0", sendStr , strlen(sendStr) );
		lrs_send("socket0", "buf0", LrsLastArg);
		custom_lrs_receive_all("socket0", "buf1", LrsLastArg);

		waitWithHeartBeat(getRandom(2, 10));

	}

	lr_end_transaction(transactionName,LR_AUTO);


	return 0;
}






#endif
