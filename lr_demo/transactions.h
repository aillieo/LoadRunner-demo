#ifndef _UTILS_H

#define _UTILS_H


#include "lrs.h"

#include "utils.h"



//ִ������ �����Ӧ��16������Ϣ���ݴ洢�ڲ���str_parameter�� commandsAmount��ʾ��Ϣ������
int performTransaction(char* transactionName, char* str_parameter, int commandsAmount )
{

	int i = 0;
	char sendStr[512];
	int ret = 0;
	int retry = 0;


	lr_start_transaction(transactionName);


	for (i = 0 ; i < commandsAmount ; i++) 
	{

		ret = -1;
		retry = 0;

		strcpy(sendStr,lr_eval_string(str_parameter));  
		lr_output_message("%s",sendStr);

		do{
			
			lrs_set_send_buffer("socket0", sendStr , strlen(sendStr) );
			lrs_send("socket0", "buf0", LrsLastArg);


			ret = custom_lrs_receive_all("socket0", "buf1", LrsLastArg);
			//���ճ��������·���

			retry += 1;

			if( ret!=0 && retry >= 3)
			{
				
				lr_error_message("FAIL TRANSACTION %s step=%d vu=%s uid=%s " ,transactionName,i,"0",lr_eval_string("{uid}"));
				lr_end_transaction(transactionName,LR_FAIL);
				return ret;
			}


		}
		while(ret != 0);

		
		//lrs_receive("socket0", "buf1", LrsLastArg);


	}

	lr_end_transaction(transactionName,LR_AUTO);


	return 0;
}






#endif
