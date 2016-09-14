#include "header.h"

int main(int argc, char *argv[])
{
	int fd, i = 0;
	struct BS_BPB *ptr;
	unsigned char buff[sizeof(struct BS_BPB)];
	unsigned char character1, attribute;
	unsigned int FirstDataSector, FAT_Start;
	unsigned int FirstSectorofCluster;
	unsigned int SizeofCluster;
	unsigned short int N = 2;

	if (argc < 2)
	{
		printf("Usage: ./<exe_file_name> <file_name>\n");
		return -1;
	}


	if((fd = open(argv[1], O_RDONLY, S_IRWXU)) < 0){
		perror("Error");
		return 0;
	}

	SizeofCluster = ptr->BPB_SecPerClus * 512;
	read(fd, &buff, sizeof(buff));
	ptr = (struct BS_BPB*)buff;

	FirstSectorofCluster = (ptr->BPB_RsvdSecCnt + (ptr->BPB_NumFATs * ptr->BPB_FATSz32)) * ptr->BPB_BytsPerSec; 

	FirstDataSector = (N-2) * ptr->BPB_SecPerClus * ptr->BPB_BytsPerSec + FirstSectorofCluster;

	FAT_Start = ptr->BPB_RsvdSecCnt * ptr->BPB_BytsPerSec;

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

			lseek(fd, FirstSectorofCluster+32+11, SEEK_SET);	//Setting the position to read the file attributes
			read(fd, &attribute, 1);
			lseek(fd, FirstSectorofCluster+32+28, SEEK_SET);
			read(fd, &size, 1); 
			if (name[0] == 0x41)
			{
				FirstSectorofCluster += 32;
			}
			if (attribute == 0x10){
				printf("\033[22;36m%s\033[0m", name); 
				lseek(fd, FirstSectorofCluster+32+26, SEEK_SET);	//Setting the position to read the file attributes
				read(fd, &N, 2);
				dir_count++;
				printf("\n");
				recursive(fd, ptr, attribute, N, FirstDataSector); 
			}

			else {
				printf("\033[22;32m%s\033[0m", name); 
				file_count++;
				printf("\n");
			}	
		}
		FirstSectorofCluster += 32;
		lseek(fd, FirstSectorofCluster, SEEK_SET);
		read(fd, &character1, 1);
	}
	printf("%d files, %d directories\n", file_count, dir_count);
	return 1;
}
