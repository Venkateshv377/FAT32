#include "header.h"

unsigned char attribute_long;
void long_name(int fd, unsigned int FirstSectorofCluster, unsigned char *attribute, unsigned short int *N)
{
	unsigned char character1, short_name[32], temp = 0;
	unsigned short int count = 0;
	unsigned int i, TempFirstSector = FirstSectorofCluster;
	lseek(fd, FirstSectorofCluster, SEEK_SET);
	read(fd, &character1, 1);
	temp = (character1 << 4);
	temp = temp >> 4;
	count = temp;
	if ((name_long = calloc(count, sizeof(char)*32)) == NULL)
	{
		perror("Error:\n");
	}
	memset(name_long, 0, count*32);
	for (i = count; i > 0; i--)
	{
		lseek(fd, FirstSectorofCluster + (i-1)*32, SEEK_SET);
		read(fd, short_name, 32);
		print(short_name, 32);
		strcat(name_long, name);
	}
	FirstSectorofCluster += count*32;
	lseek(fd, FirstSectorofCluster + 11, SEEK_SET);
	read(fd, attribute, 1);
	lseek(fd, FirstSectorofCluster + 26, SEEK_SET);
	read(fd, N, 2);
}
