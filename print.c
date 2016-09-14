#include "header.h"

void print(unsigned char big_name[])
{
	int i, j = 0;
	memset(name,0,32);
	for(i = 0; i < 32; i++)
	{
		if(i > 11 && i < 13)
		{
			continue;
		}
		if(isprint(big_name[i]) != 0)
		{
			name[j] = big_name[i];
			j++;
		}
	}
}
