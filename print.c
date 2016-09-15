#include "header.h"

void print(unsigned char *short_name, unsigned int size)
{
	int i, j = 0;
	name = calloc(size, sizeof(char));
	memset(name,0,size);
	for(i = 0; i < size-1; i++)
	{
		if(i > 10 && i < 13)
		{
			continue;
		}
		if(isprint(short_name[i]) != 0)
		{
			name[j] = short_name[i];
			j++;
		}
	}
}
