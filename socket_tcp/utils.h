#include "lrs.h"

//将string转化为loadrunner中使用的16进制值的string
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



//自定义函数 接收可变长度的消息 具体可以根据协议调整
//1、接收消息的前四个字节 并将接收的字节转化为十进制数
//2、根据前四个字节读取到的长度信息 接收消息剩余部分
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

	//判断前4个字节是否接受成功
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


//将接收到的4个字节转化为十进制整数并返回
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
