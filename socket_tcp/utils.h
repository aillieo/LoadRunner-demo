#include "lrs.h"

//��stringת��Ϊloadrunner��ʹ�õ�16����ֵ��string
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



