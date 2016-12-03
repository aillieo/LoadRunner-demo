#include "lrs.h"

//��stringת��Ϊloadrunner��ʹ�õ�16����ֵ��string
int strToHex(char* src, char* des)
{
	int index = 0 ;
	for(index = 0; index < strlen(src) ; index++ )
	{
		char hex[5];
		sprintf(hex,"\\x%02X",src[index]);
		strcat(des + index , hex);
	}

	return 0;

}


//����ȡ�����ֽ�ת��Ϊloadrunner�е�16������ʽstring
int dataToHex(char* src, char* des, int length )
{
	int i = 0 ;
	for(i = 0; i < length ; i++ )
	{
		unsigned char hex[5];
		sprintf(hex,"\\x%02X",src[i]);
		memcpy(des + i*4 , hex,4);
	}

	return 0;


}



//�Զ��庯�� ���տɱ䳤�ȵ���Ϣ ������Ը���Э�����
//1��������Ϣ��ǰ�ĸ��ֽ� �������յ��ֽ�ת��Ϊʮ������
//2������ǰ�ĸ��ֽڶ�ȡ���ĳ�����Ϣ ������Ϣʣ�ಿ��
int auto_receive(char *sock_desc, char *buf_desc,void *dummy)
{
	int rc;
	
	int buf_len = 4;
	
	char szBytesLength[30], *buf = NULL, *pszError, *pszLastChar;

	rc = lrs_receive_ex(sock_desc, buf_desc, "NumberOfBytesToRecv=4", LrsLastArg);	

	if (rc != 0) 
	{
		lr_error_message("Receive 4 bytes failed. The error code = %d", rc);

		return -1;
	} 

	//�ж�ǰ4���ֽ��Ƿ���ܳɹ�
	lrs_get_last_received_buffer(sock_desc, &buf, &buf_len);

	if (buf == NULL || buf_len != 4)
	{
		lr_error_message("receive of %s failed", buf_desc);

		return -1;
	}

	sprintf (szBytesLength, "NumberOfBytesToRecv=%d", fiFromHexBinToInt(buf)); 

	lr_debug_message(LR_MSG_CLASS_FULL_TRACE, "!!!! Bytes length = %s", szBytesLength);

	rc = lrs_receive_ex(sock_desc, buf_desc, szBytesLength, LrsLastArg);

	if (rc != 0)
		return -1;

	return 0;
}


//�����յ���4���ֽ�ת��Ϊʮ��������������
int fiFromHexBinToInt(char *szBuffer)
{

	int i, j, iIntValue = 0, iExp = 1; 


	for(i = 0 ; i < 4 ; i ++)
	{
		iExp = 1;
		for(j = i ; j <3 ; j ++ )
		{
			iExp*= 256;
		}
		iIntValue += (szBuffer[i] & 0x000000ff)  * iExp;
	}

	return iIntValue;

}





//��������
float getRandom(float min , float max)
{
	float ret = max;

	if(max < min)
	{
		return 0 ;
	}

	if(max > min)
	{
		//������� 0 - 99
		float rand = (float)(atoi(lr_eval_string("<rand>")));
		ret = min + rand/99.0 * (max - min) ;
	}

	return ret;

}




//�ȴ��漴min~max��ʱ��
float randomThinkTime(float min , float max)
{

	float ret = getRandom(min,max);
	lr_think_time(ret);

	return ret;

}



//ִ������ �����Ӧ��16������Ϣ���ݴ洢�ڲ���str_parameter�� commandsAmount��ʾ��Ϣ������
int performTransaction(char* transactionName, char* str_parameter, int commandsAmount )
{

	int i = 0;
	char sendStr[512];


	lr_start_transaction(transactionName);
	lr_output_message("vuid %s start transation %s",lr_eval_string("<VUID>") , transactionName);


	for (i = 0 ; i < commandsAmount ; i++) 
	{

		strcpy(sendStr,lr_eval_string(str_parameter));  
		lrs_set_send_buffer("socket0", sendStr , strlen(sendStr) );
		lrs_send("socket0", "buf0", LrsLastArg);
		auto_receive("socket0", "buf1", LrsLastArg);

		randomThinkTime(2, 10);

	}

	lr_end_transaction(transactionName,LR_AUTO);

	lr_output_message("vuid %s finish transation %s",lr_eval_string("<VUID>") , transactionName);

	return 0;
}



//�ȴ�һ��ʱ���ִ��xx����
int performTransactionAfterWait(char* transactionName, char* str_parameter, int commandsAmount ,float waitTime)
{

	lr_think_time(waitTime);

	return performTransaction(transactionName, str_parameter, commandsAmount );

}


