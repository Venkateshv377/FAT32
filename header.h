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

unsigned int file_count, dir_count;
unsigned char *name;
unsigned char big_name[32];
unsigned char *name_long;
unsigned int size;

void recursive(int fd, struct BS_BPB *ptr1, unsigned char attr, unsigned short int n, unsigned int FirstDataSector);

void print(unsigned char *short_name, unsigned int size);

void long_name(int fd, unsigned int FirstSectorofCluster, unsigned char *attribute, unsigned short int *N);
