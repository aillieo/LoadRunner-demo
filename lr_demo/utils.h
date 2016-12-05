#ifndef _UTILS_H

#define _UTILS_H



#include "lrs.h"


//��stringת��Ϊloadrunner��ʹ�õ�16����ֵ��string
int strToHex(char* src, char* des)
{
	int i = 0 ;
	for(i = 0; i < strlen(src) ; i++ )
	{
		char hex[5];
		sprintf(hex,"\\x%02X",(unsigned char)src[i]);
		strcat(des + i , hex);
	}

	return 0;


}


//����ȡ�����ֽ�ת��Ϊloadrunner�е�16������ʽstring
int dataToHex(char* src, char* des, int length )
{
	int i = 0 ;
	for(i = 0; i < length ; i++ )
	{
		char hex[5];
		sprintf(hex,"\\x%02X",(unsigned char)src[i]);
		memcpy(des + i*4 , hex,4);
	}
	memcpy(des + length * 4 , "\0",4);

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



	/* 
	* Get package header  0-6��bytes�� [5..6] bytes  is package length 
	*/


	rc = lrs_receive_ex(sock_desc, buf_desc, "NumberOfBytesToRecv=4", LrsLastArg);	

	if (rc != 0)     //��������º�������Ϊ0����0��ʾ�����д���
	{
		//lr_error_message("Receive 4 bytes failed. The error code = %d", rc);

		return -1;
	} /* Receive failed */

	//�ж�ǰ4���ֽ��Ƿ���ܳɹ�
	lrs_get_last_received_buffer(sock_desc, &buf, &buf_len);

	if (buf == NULL || buf_len != 4)
	{
		lr_error_message("receive of %s failed", buf_desc);

		return -1;
	}

	/* Compute buffer length */

	sprintf (szBytesLength, "NumberOfBytesToRecv=%d", fiFromHexBinToInt(buf));            //������һ���Զ��庯���������ܳ��ȵĺ���

	lr_debug_message(LR_MSG_CLASS_FULL_TRACE, "!!!! Bytes length = %s", szBytesLength);

	/* ����ʣ�µ��ֽ��� */
	rc = lrs_receive_ex(sock_desc, buf_desc, szBytesLength, LrsLastArg);



	if (rc != 0) /* Receive failed */
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


// ������Ϣֱ��û�пɽ��յ�����
int custom_lrs_receive_all(char *sock_desc, char *buf_desc, void *dummy)
{
	int i = 0;
	i = custom_lrs_receive(sock_desc, buf_desc, dummy);

	//�ж������Ƿ���ճɹ�
	if(i == -1)
	{
		lr_error_message("������Ϣʧ��");
		return -1;
	}

	//�������� ֱ��û��
	//while(buf_desc + lrs_get_last_received_buffer_size(sock_desc)+1 != NULL )
	while(i != -1)
	{
		i = custom_lrs_receive(sock_desc, buf_desc, dummy);
	}
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







#endif
