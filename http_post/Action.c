/*

脚本：
链接web服务器，批量申请和注册账号，并将得到的账号信息存储到文件内，供下一步使用

需要预先存储openid的列表作为参数

使用循环控制，调用时

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


			//从获取的响应中提取数据 并存入参数
			web_reg_save_param("uid","LB=\"uid\":\"","RB=\",\"secret",LAST);
			web_reg_save_param("secret","LB=\"secret\":\"","RB=\",\"server",LAST);
			web_reg_save_param("server","LB=\"server\":\"","RB=\",\"region",LAST);


			//web_set_max_html_param_len("262144");
			//存储响应头的内容 
			//web_save_header(RESPONSE, "ResponseHeader");
			//存储响应体的内容
			//web_reg_save_param("ResponseBody", "LB=", "RB=", "Search=Body",LAST);


			//发送请求，使用POST方式，并提交参数，其中openid一项是从参数列表中读取
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

			//lr_output_message("响应头信息：\n %s", lr_eval_string("{ResponseHeader}"));
			//lr_output_message("响应内容体：\n %s", lr_eval_string("{ResponseBody}"));


			lr_output_message("得到的uid：\n %s", lr_eval_string("{uid}"));
			lr_output_message("得到的secret：\n %s", lr_eval_string("{secret}"));
			lr_output_message("得到的server：\n %s", lr_eval_string("{server}"));

  
			user1.uid = lr_eval_string("{uid}");
			user1.secret = lr_eval_string("{secret}");
			user1.server = lr_eval_string("{server}");

			//将取到的数据写进文件
			fprintf(file_handle,"%s %s %s\n",user1.uid,user1.secret,user1.server);

		}

		fclose(file_handle);

		return 0;


}

