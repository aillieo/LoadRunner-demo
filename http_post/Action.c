/*

�ű���
����web�����������������ע���˺ţ������õ����˺���Ϣ�洢���ļ��ڣ�����һ��ʹ��

��ҪԤ�ȴ洢openid���б���Ϊ����

ʹ��ѭ�����ƣ�����ʱ

*/

Action()
{

	    char* filename = "..\\newUsers.txt";

		int idAmount = 10;

		struct user
		{
			char* uid;
			char* secret;
			char* server;
		};

		struct user user1;

		long file_handle = NULL;

		int i = 0;


		if ((file_handle = fopen(filename, "w+")) == NULL)
		{
			lr_output_message ("Unable to create %s", filename);
			return -1;
		}


		for(i = 0 ; i < idAmount ; i ++)
		{


			//�ӻ�ȡ����Ӧ����ȡ���� ���������
			web_reg_save_param("uid","LB=\"uid\":\"","RB=\",\"secret",LAST);
			web_reg_save_param("secret","LB=\"secret\":\"","RB=\",\"server",LAST);
			web_reg_save_param("server","LB=\"server\":\"","RB=\",\"region",LAST);


			//web_set_max_html_param_len("262144");
			//�洢��Ӧͷ������ 
			//web_save_header(RESPONSE, "ResponseHeader");
			//�洢��Ӧ�������
			//web_reg_save_param("ResponseBody", "LB=", "RB=", "Search=Body",LAST);


			//��������ʹ��POST��ʽ�����ύ����������openidһ���ǴӲ����б��ж�ȡ
			web_submit_data("post1",
							"Action=http://192.168.2.168:8060/mix/login",
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
			//lr_output_message("��Ӧ�����壺\n %s", lr_eval_string("{ResponseBody}"));


			lr_output_message("�õ���uid��\n %s", lr_eval_string("{uid}"));
			lr_output_message("�õ���secret��\n %s", lr_eval_string("{secret}"));
			lr_output_message("�õ���server��\n %s", lr_eval_string("{server}"));

  
			user1.uid = lr_eval_string("{uid}");
			user1.secret = lr_eval_string("{secret}");
			user1.server = lr_eval_string("{server}");

			//��ȡ��������д���ļ�
			fprintf(file_handle,"%s %s %s\n",user1.uid,user1.secret,user1.server);

		}

		fclose(file_handle);

		return 0;


}

