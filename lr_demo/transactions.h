#ifndef _UTILS_H

#define _UTILS_H


#include "lrs.h"

#include "utils.h"



//执行事务 事务对应的16进制消息内容存储在参数str_parameter中 commandsAmount表示消息的条数
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
