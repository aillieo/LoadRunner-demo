#ifndef _UTILS_H

#define _UTILS_H



#include "lrs.h"


//将string转化为loadrunner中使用的16进制值的string
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


//将读取到的字节转化为loadrunner中的16进制形式string
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



//自定义函数 接收可变长度的消息 具体可以根据协议调整
//1、接收消息的前四个字节 并将接收的字节转化为十进制数
//2、根据前四个字节读取到的长度信息 接收消息剩余部分
int custom_lrs_receive(char *sock_desc, char *buf_desc,void *dummy)
{
	int rc;
	
	int buf_len = 4;
	
	char szBytesLength[30], *buf = NULL, *pszError, *pszLastChar;



	/* Get first 4 bytes */


	rc = lrs_receive_ex(sock_desc, buf_desc, "NumberOfBytesToRecv=4", LrsLastArg);	

	if (rc != 0)     //正常情况下函数返回为0，非0表示函数有错误
	{
		//lr_error_message("Receive 4 bytes failed. The error code = %d", rc);

		return -1;
	} /* Receive failed */

	//判断前4个字节是否接受成功
	lrs_get_last_received_buffer(sock_desc, &buf, &buf_len);

	if (buf == NULL || buf_len != 4)
	{
		lr_error_message("receive of %s failed", buf_desc);

		return -1;
	}

	/* Compute buffer length */

	//与官方的demo不同 前四个字节的消息长度即后边消息的长度 不需要减去前4个字节
	sprintf (szBytesLength, "NumberOfBytesToRecv=%d", fiFromHexBinToInt(buf));            //调用另一个自定义函数：计算总长度的函数

	lr_debug_message(LR_MSG_CLASS_FULL_TRACE, "!!!! Bytes length = %s", szBytesLength);

	/* 接受剩下的字节流 */
	rc = lrs_receive_ex(sock_desc, buf_desc, szBytesLength, LrsLastArg);



	if (rc != 0) /* Receive failed */
		return -1;




	return 0;
}


//将接收到的4个字节转化为十进制整数并返回
int fiFromHexBinToInt (char *szBuffer)
{

    int i, j, iIntValue = 0, iExp = 1;

    for( i = 3; i >= 0; i--)
    {
        iExp = 1;

        for (j = 6; j > i*2; j--)
            iExp *= 16;

        iIntValue += (szBuffer[i] & 0x0000000f) * iExp + ((szBuffer[i] & 0x000000f0) >> 4) * iExp * 16;

    }

    return iIntValue;

}


// 接收消息直到没有可接收的内容
int custom_lrs_receive_all(char *sock_desc, char *buf_desc, void *dummy)
{
	int i = 0;
	i = custom_lrs_receive(sock_desc, buf_desc, dummy);

	//判断首条是否接收成功
	if(i == -1)
	{
		lr_error_message("接收消息失败");
		return -1;
	}

	//继续接收 直到没有
	//while(buf_desc + lrs_get_last_received_buffer_size(sock_desc)+1 != NULL )
	while(i != -1)
	{
		i = custom_lrs_receive(sock_desc, buf_desc, dummy);
	}
	return 0;
}


//获得随机数
float getRandom(float min , float max)
{


	//return atoi(lr_eval_string("{rand}"));


	float ret = max;


	if(max < min)
	{
		return 0 ;
	}

	if(max > min)
	{
		//随机数是 0 - 9999
		float rand = (float)(atoi(lr_eval_string("{rand}")));

		ret = min + rand/9999.0 * (max - min) ;
	}

	return ret;

}





int waitWithHeartBeat(double waitTime)
{

	// 心跳包数据
	char* send = "\\x00\\x06\\x04\\x13\\x00\\x00\\x00\\x00";

	while(waitTime > 1)
	{

		waitTime-= 1;
		lr_think_time(1);
		
		lrs_set_send_buffer("socket0", send , strlen(send) );
		lrs_send("socket0", "buf0", LrsLastArg);


	}
	lr_think_time(waitTime);
	return 0;

}




#endif
