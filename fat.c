#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

struct BS_BPB {
	uint8_t BS_jmpBoot[3]; //0-2
	uint8_t BS_OEMName[8]; //3-10
	uint16_t BPB_BytsPerSec;//11-12
	uint8_t BPB_SecPerClus; //13 
	uint16_t BPB_RsvdSecCnt; //14-15
	uint8_t BPB_NumFATs; //16 
	uint16_t BPB_RootEntCnt; //17-18
	uint16_t BPB_TotSec16; //19-20
	uint8_t BPB_Media; //21 
	uint16_t BPB_FATSz16; //22-23
	uint16_t BPB_SecPerTrk; //24-25
	uint16_t BPB_NumHeads; //26-27
	uint32_t BPB_HiddSec; //28-31
	uint32_t BPB_TotSec32; //32-35
	uint32_t BPB_FATSz32 ; //36-39
	uint16_t BPB_ExtFlags ; //40-41
	uint16_t BPB_FSVer ; //42-43
	uint32_t BPB_RootClus ;//44-47
	uint16_t BPB_FSInfo ; //48-49
	uint16_t BPB_BkBootSec ;//50-51
	uint8_t BPB_Reserved[12]; //52-63
	uint8_t BS_DrvNum ;//64
	uint8_t BS_Reserved1 ;//65
	uint8_t BS_BootSig ;//66
	uint32_t BS_VolID ;//67-70
	uint8_t BS_VolLab[11]; //71-81
	uint8_t BS_FilSysType[8]; //82-88

} __attribute__((packed));

unsigned int file_count = 0, dir_count = 0;
unsigned char name[11] = {0};

void recursive(int fd, struct BS_BPB *ptr1, unsigned char attr, unsigned short int n, unsigned int FirstDataSector);
int main(int argc, char *argv[])
{
	int fd, i = 0;
	struct BS_BPB fat;
	struct BS_BPB *ptr;
	unsigned char buff[sizeof(fat)];
	unsigned char character, attribute;
	unsigned int FirstDataSector, FAT_Start;
	unsigned int FirstSectorofCluster;
	unsigned int DataSec, SizeofCluster;
	unsigned short int N = 2;

	if (argc < 2)
	{
		printf("Usage: ./<exe_file_name> <file_name>\n");
		return -1;
	}

	SizeofCluster = fat.BPB_SecPerClus * 512;

	if((fd = open(argv[1], O_RDONLY, S_IRWXU)) < 0){
		perror("Error");
		return 0;
	}

	read(fd, &buff, sizeof(fat));
	ptr = (struct BS_BPB*)buff;
	//	memcpy(&fat, buff, sizeof(buff));
	printf("Bytes: %d\n", ptr->BPB_BytsPerSec);

	FirstSectorofCluster = (ptr->BPB_RsvdSecCnt + (ptr->BPB_NumFATs * ptr->BPB_FATSz32)) * ptr->BPB_BytsPerSec; 

	FirstDataSector = (N-2) * ptr->BPB_SecPerClus * ptr->BPB_BytsPerSec + FirstSectorofCluster;

	FAT_Start = ptr->BPB_RsvdSecCnt * ptr->BPB_BytsPerSec;

	lseek(fd, FirstSectorofCluster, SEEK_SET);
	//	read(fd, &buff, sizeof(fat));
	//	printf("FirstSector1: %x\n", FirstSectorofCluster);
	read(fd, &character, 1);
	//	ptr = (struct BS_BPB*)buff;

	printf("%x\t%x\t%x\t%x\t%x\t%x\t%x\n", ptr->BPB_RsvdSecCnt, ptr->BPB_NumFATs, ptr->BPB_FATSz32, ptr->BPB_SecPerClus, FirstDataSector, FirstSectorofCluster, ptr->BPB_TotSec32);

	while(character != 0x41)
	{
		FirstSectorofCluster += 32;
		if (character != 0x41)
		{
			lseek(fd, FirstSectorofCluster, SEEK_SET);
			read(fd, &character, 1);
		}
	}

	while(character != 0x00)
	{
		if (character == 0x41)
		{
			lseek(fd, FirstSectorofCluster+32, SEEK_SET);
			read(fd, &name, 8);
			lseek(fd, FirstSectorofCluster+32+11, SEEK_SET);	/*Setting the position to read the file attributes*/
			read(fd, &attribute, 1);
			lseek(fd, FirstSectorofCluster+32+26, SEEK_SET);	/*Setting the position to read the file attributes*/
			read(fd, &N, 2);
			printf("name is: %s\t ", name);
			if (attribute == 0x10){
				dir_count++;
				printf("Directory");
				recursive(fd, ptr, attribute, N, FirstDataSector); 
			}
			else if (attribute == 0x20){
				file_count++;
				printf("File");
			}
			printf("\n");
		}
		FirstSectorofCluster += 32;
		lseek(fd, FirstSectorofCluster, SEEK_SET);
		read(fd, &character, 1);
	}
	printf("Number of files: %d\n", file_count);
	printf("Number of directory: %d\n", dir_count);
	return 1;
}

void recursive(int fd, struct BS_BPB *ptr1, unsigned char attr, unsigned short int n, unsigned int FirstDataSector)
{
	unsigned int FirstSectorofCluster;
	unsigned char character, attribute;
	//	printf("FirstDataSector: %x\n", FirstDataSector);
	FirstDataSector = (n-2) * ptr1->BPB_SecPerClus * ptr1->BPB_BytsPerSec + FirstDataSector;
	FirstSectorofCluster = FirstDataSector;
	lseek(fd, FirstSectorofCluster, SEEK_SET);
	read(fd, &character, 1);
	printf("\ncharacter: %x\n", character);
	while(character != 0x41)
	{
		FirstSectorofCluster += 32;
		if (character != 0x41)
		{
			lseek(fd, FirstSectorofCluster, SEEK_SET);
			read(fd, &character, 1);
		}
	}

	printf("\ncharacter: %x\n", character);
	printf("FirstSectorofCluster: %x\n", FirstSectorofCluster);
	while(character != 0x00)
	{
		if (character == 0x41)
		{
			lseek(fd, FirstSectorofCluster+32, SEEK_SET);
			read(fd, &name, 8); 
			lseek(fd, FirstSectorofCluster+32+11, SEEK_SET);        /*Setting the position to read the file attributes*/
			read(fd, &attribute, 1); 
			lseek(fd, FirstSectorofCluster+32+26, SEEK_SET);        /*Setting the position to read the file attributes*/
			read(fd, &n, 2); 
			printf("\nname is: %s\t ", name); 
			if (attribute == 0x10){
				dir_count++;
				printf("Directory");
				recursive(fd, ptr1, attribute, n, FirstDataSector); 
			}
			else if (attribute == 0x20){
				file_count++;
				printf("File");
			}
		}
		FirstSectorofCluster += 32; 
		lseek(fd, FirstSectorofCluster, SEEK_SET);
		read(fd, &character, 1); 
	}
}
