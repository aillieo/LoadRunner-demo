#include "lrs.h"

#include "utils.h"
#include "login.h"

Action()
{

	//随机数 在区分不同行为时使用
	int random = 0;


	login();


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



	logout();


    return 0;

}


