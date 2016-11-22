#include "lrs.h"
#include "custom_receive.h"
#include "utils.h"

Action()
{

	int totalAmount = 100;

	long file_handle = NULL;

	char* filename = "..\\newUsers.txt";

			struct user
		{
			char uid[20];
			char secret[20];
			char server[20];
		};

	struct user user1;
	
	int _vuid = atoi("<VUID>");

	char sendStr[512] = "";

	int i = 0;

	short length = 0 ;

	char hex[80] = "";
	char _buff[80] = "\\x00\\x1A"; 



    lr_start_transaction("login");



    if(_vuid%10 < 7 )
	{
		if ((file_handle = fopen(filename, "r")) == NULL)
		{
			lr_output_message ("Unable to create %s", filename);
			return -1;
		}

		fscanf(file_handle,"%s %s %s\n", user1.uid, user1.secret, user1.server); 


		//lr_output_message("%s %s %s",user1.uid,user1.secret,user1.server);


        //sprintf(_buff,"%c",26);
        
		//strcat(hex,_buff);
		strcat(hex,"a,");
		strcat(hex,user1.uid);
		strcat(hex,",");
		strcat(hex,user1.secret);

		//sscanf(hex,"%02X",hex);




		//sscanf("1 2","%d %d %d",buf1, buf2, buf3);


		strToHex(hex,sendStr);
		//strcat(_buff,"\x00\x1A");

		lr_output_message("HEX %s",&_buff);

		strcat(_buff,sendStr);





		lr_output_message("HEX%s",_buff);









	}
	else
	{


		char* filename = "..\\oldUsers.txt";

				if ((file_handle = fopen(filename, "r")) == NULL)
		{
			lr_output_message ("Unable to create %s", filename);
			return -1;
		}

		fscanf(file_handle,"%s %s %s\n", user1.uid, user1.secret, user1.server); 

		//lr_output_message("%s %s %s",user1.uid,user1.secret,user1.server);


	}

	lr_end_transaction("login",LR_AUTO);  


	lr_rendezvous("FinishLogin");



	lrs_create_socket("socket0", "TCP", "LocalHost=0", "RemoteHost=192.168.2.168:400", LrsLastArg);


    lr_start_transaction("test1");




		lrs_set_send_buffer("socket0", _buff , strlen(_buff) );
		lrs_send("socket0", "buf0", LrsLastArg);
		//custom_lrs_receive("socket0", "buf1", LrsLastArg);
		lrs_receive("socket0", "buf1", LrsLastArg);






	for (i = 0 ; i < 0 ; i++) 
	{

		strcpy(sendStr,lr_eval_string("<str>"));  
		lrs_set_send_buffer("socket0", sendStr , strlen(sendStr) );
		lrs_send("socket0", "buf0", LrsLastArg);
		//custom_lrs_receive("socket0", "buf1", LrsLastArg);
		lrs_receive("socket0", "buf1", LrsLastArg);

	}


	lr_end_transaction("test1",LR_AUTO);  


	lrs_close_socket("socket0");


    return 0;
}



