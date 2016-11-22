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



