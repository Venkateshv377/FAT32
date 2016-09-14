#include "header.h"

void recursive(int fd, struct BS_BPB *ptr1, unsigned char attr, unsigned short int n, unsigned int FirstDataSector)
{
	unsigned int FirstSectorofCluster;
	unsigned char character1, attribute;

	FirstSectorofCluster = (n-2) * ptr1->BPB_SecPerClus * ptr1->BPB_BytsPerSec + FirstDataSector;
	lseek(fd, FirstSectorofCluster, SEEK_SET);
	read(fd, &character1, 1);
	while(character1 != 0x41 && character1 != 0x00)
	{
		FirstSectorofCluster += 32; 
		if (character1 != 0x41)
		{
			lseek(fd, FirstSectorofCluster, SEEK_SET);
			read(fd, &character1, 1); 
		}
	}
	while(character1 != 0x00)
	{
		if (character1 == 0x41)
		{
			read(fd, &big_name, 31);
			print(big_name);

			lseek(fd, FirstSectorofCluster+32+11, SEEK_SET);        //Setting the file descriptor to read the file attributes
			read(fd, &attribute, 1); 
			lseek(fd, FirstSectorofCluster+32+28, SEEK_SET);
			read(fd, &size, 1);
			if (name[0] == 0x41)
			{
				FirstSectorofCluster += 32;
			}	
			if (attribute == 0x10){
				printf("\033[22;36m%s\033[0m", name);
				dir_count++;
				lseek(fd, FirstSectorofCluster+32+26, SEEK_SET);        //Setting the file descriptor to read the cluster number
				read(fd, &n, 2);
				printf("\n");
				recursive(fd, ptr1, attribute, n, FirstDataSector);
			}
			else if (attribute == 0x20){
				printf("\033[22;32m%s\033[0m", name);
				file_count++;
				printf("\n");
			}
		}
		FirstSectorofCluster += 32;
		lseek(fd, FirstSectorofCluster, SEEK_SET);
		read(fd, &character1, 1);
	}
}
