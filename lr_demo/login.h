#ifndef _LOGIN_H

#define _LOGIN_H

#include "utils.h"

int login()
{


	char _buff[80] = "";

	char _sendStr[512] = "";

	int i = 0 ;

	char sendStr[512] = "";


	loginWebForUserInfo();


	lr_start_transaction("login_game");


	sprintf(_buff, "a,%s,%s",lr_eval_string("{uid}"),lr_eval_string("{secret}"));


	strToHex(_buff,_sendStr);


	//�׸���Ϣ���ĳ���Ϊ26 short int
	sprintf(sendStr,"\\x%02X\\x%02X",0,26);


	strcat(sendStr,_sendStr);


	lrs_create_socket("socket0", "TCP", "LocalHost=0", "RemoteHost={server}:400", LrsLastArg);

	lrs_set_recv_timeout(2,0);


	lrs_set_send_buffer("socket0", sendStr , strlen(sendStr) );
	lrs_send("socket0", "buf0", LrsLastArg);
	auto_receive_all("socket0", "buf1", LrsLastArg);
	//lrs_receive("socket0", "buf1", LrsLastArg);





	for (i = 0 ; i < 8 ; i++) 
	{

		autoThink();

		//fscanf(file_handle,"%s\n", sendStr); 
		//lr_output_message("%s",sendStr);
		strcpy(sendStr,lr_eval_string("{str_login}"));  
		//lr_output_message("%s",sendStr);

		lrs_set_send_buffer("socket0", sendStr , strlen(sendStr) );
		lrs_send("socket0", "buf0", LrsLastArg);
		auto_receive_all("socket0", "buf1", LrsLastArg);
		//lrs_receive("socket0", "buf1", LrsLastArg);

	}



	lr_end_transaction("login_game",LR_AUTO);


	//lr_rendezvous("FinishLogin");

	return 0;

}




int loginWebForUserInfo()
{

	lr_start_transaction("web_for_userInfo");

	//�ӻ�ȡ����Ӧ����ȡ���� ���������
	web_reg_save_param("uid","LB=\"uid\":\"","RB=\",",LAST);
	web_reg_save_param("secret","LB=\"secret\":\"","RB=\",",LAST);
	web_reg_save_param("server","LB=\"server\":\"","RB=\",",LAST);


	//web_set_max_html_param_len("262144");
	//�洢��Ӧͷ������ 
	//web_save_header(RESPONSE, "ResponseHeader");
	//�洢��Ӧ�������
	web_reg_save_param("ResponseBody", "LB=", "RB=", "Search=Body",LAST);
	//web_reg_save_param("ResponseBody", "LB=", "RB=", LAST);




	//��������ʹ��POST��ʽ�����ύ����������openidһ���ǴӲ����б��ж�ȡ
	web_submit_data("post1",
					"Action=http://server:8060/login",
					"Method=POST", 
					"RecContentType=text/html", 
					"Mode=HTML", 
					ITEMDATA, 
					"Name=openid","Value={openid}",ENDITEM, 
					"Name=channel","Value=22001",ENDITEM, 
					"Name=subPackage","Value=1",ENDITEM, 
					"Name=deviceType","Value=1",ENDITEM, 
					"Name=bundle","Value=1.0",ENDITEM, 
					LAST);


	//lr_output_message("��Ӧͷ��Ϣ��\n %s", lr_eval_string("{ResponseHeader}"));
	lr_output_message("��Ӧ�����壺\n %s", lr_eval_string("{ResponseBody}"));


	lr_output_message("�õ���uid��\n %s", lr_eval_string("{uid}"));
	lr_output_message("�õ���secret��\n %s", lr_eval_string("{secret}"));
	lr_output_message("�õ���server��\n %s", lr_eval_string("{server}"));



	lr_end_transaction("web_for_userInfo",LR_AUTO);




	return 0;

}









int logout()
{

	lrs_close_socket("socket0");

	return 0;
}








#endif
