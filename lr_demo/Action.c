#include "lrs.h"

#include "utils.h"
#include "login.h"

Action()
{

	//����� �����ֲ�ͬ��Ϊʱʹ��
	int random = 0;


	login();


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



	logout();


    return 0;

}


