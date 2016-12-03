#include "lrs.h"
#include "utils.h"


Action()
{

	long file_handle = NULL;

	char newUsersFile[] = ".\\newUsers.dat";

	//�����û���ID
	int vuid = atoi(lr_eval_string("<VUID>"));

	//����� �����ֲ�ͬ��Ϊʱʹ��
	int random = 0;

	char serverStr[128] = "";

	//�����͵Ļ�����
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



	//��һ���Ǳ��� �ӵڶ��п�ʼ�ǵ�һ���û������� ��Ӧuvid = 1 �Դ�����
	for(i = 0 ; i < vuid + 1 ; i++ )
	{
		
		fscanf(file_handle,"%s %s %s\n", user1.uid, user1.secret, user1.server); 
		//lr_output_message("%s %s %s",user1.uid,user1.secret,user1.server);
	}



	//����������ַ���
	sprintf(serverStr,"RemoteHost=");
    strcat(serverStr,user1.server);
	strcat(serverStr,":400");


	//����Э�鹹���¼����Ϣ
	strcat(_buff,"a,");
	strcat(_buff,user1.uid);
	strcat(_buff,",");
	strcat(_buff,user1.secret);

	//��������תΪ16����
	strToHex(_buff,_buff_h);


	//�׸���Ϣ���ĳ���Ϊ26 short int
	sprintf(_buff,"\\x%02X\\x%02X",0,26);
	strcat(_buff,_buff_h);



	//��¼����
	lr_start_transaction("login");


	lrs_create_socket("socket0", "TCP", "LocalHost=0", serverStr, LrsLastArg);
	lrs_set_send_buffer("socket0", _buff , strlen(_buff) );
	lrs_send("socket0", "buf0", LrsLastArg);
	custom_lrs_receive("socket0", "buf1", LrsLastArg);


	lr_end_transaction("login",LR_AUTO);



	//��ʼִ�и�����
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


