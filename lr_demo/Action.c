#include "lrs.h"
#include "utils.h"


Action()
{

	long file_handle = NULL;

	char newUsersFile[] = ".\\newUsers.dat";

	//虚拟用户的ID
	int vuid = atoi(lr_eval_string("<VUID>"));

	//随机数 在区分不同行为时使用
	int random = 0;

	char serverStr[128] = "";

	//待发送的缓冲区
	char _buff_h[80] = "";
	char _buff[80] = "";


	struct user
	{
		char uid[20];
		char secret[20];
		char server[20];
	};

	struct user user1;


	int i = 0;




	if ((file_handle = fopen(newUsersFile, "r")) == NULL)
	{
		lr_output_message ("Unable to read %s", newUsersFile);
		return -1;
	}



	//第一行是标题 从第二行开始是第一个用户的数据 对应uvid = 1 以此类推
	for(i = 0 ; i < vuid + 1 ; i++ )
	{
		
		fscanf(file_handle,"%s %s %s\n", user1.uid, user1.secret, user1.server); 
		//lr_output_message("%s %s %s",user1.uid,user1.secret,user1.server);
	}



	//构造服务器字符串
	sprintf(serverStr,"RemoteHost=");
    strcat(serverStr,user1.server);
	strcat(serverStr,":400");


	//根据协议构造登录的消息
	strcat(_buff,"a,");
	strcat(_buff,user1.uid);
	strcat(_buff,",");
	strcat(_buff,user1.secret);

	//数据内容转为16进制
	strToHex(_buff,_buff_h);


	//首个消息传的长度为26 short int
	sprintf(_buff,"\\x%02X\\x%02X",0,26);
	strcat(_buff,_buff_h);



	//登录事务
	lr_start_transaction("login");


	lrs_create_socket("socket0", "TCP", "LocalHost=0", serverStr, LrsLastArg);
	lrs_set_send_buffer("socket0", _buff , strlen(_buff) );
	lrs_send("socket0", "buf0", LrsLastArg);
	custom_lrs_receive("socket0", "buf1", LrsLastArg);


	lr_end_transaction("login",LR_AUTO);



	//开始执行各事务
	performTransaction("t1" , "<str_t1>" , 3 );


	random = atoi(lr_eval_string("<rand>"));
	if(random< 33)
	{
		performTransaction("t2" , "<str_t2>" , 5 );
	}
	else
	{
		performTransaction("t3" , "<str_t3>" , 4 );
	}



	lrs_close_socket("socket0");



    return 0;

}


