#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h> //PROT_READ, PROT_WRITE, MAP_SHARED
#include <fcntl.h> //O_RDWR
#include <string.h>

#include <byteswap.h> //bswap_32
#include "pretty.h"
#include "types.h"

//#include "CoreFoundation/CoreFoundation.h" //CFSwapInt32HostToBig


int main(int argc, char *argv[])
{
	int fd = open(argv[1], O_RDWR);
	if(fd < 0) {
		perror("open");
		return -1;
	}
	
	unsigned char *buffer = (unsigned char *)mmap(NULL, 2048, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(buffer == (unsigned char*)0xffffffff) {
		perror("mmap");
		return -1;
	}
	
	printInfo(buffer);
	
	if((buffer[1024] != 'H') && (buffer[1025] != '+')) {
		fprintf(stderr, "%s: HFS+ signature not found -- aborting.\n", argv[0]);
		return -1;
	}
	
	unsigned long attributes = *(unsigned long *)(&buffer[1028]);
	attributes = bswap_32(attributes);
	
	printf("\n\n");
	
	if(!(attributes &  0x00002000)) {
		printf("Journal is already disabled\n");//kHFSVolumeJournaledBit not currently set in the volume attributes field.\n");
	} else {
	    printf("Journal is enabled, will disable\n");
	}
	
	if(!(attributes & 0x00000100)) {
		printf("Volume not set to unmounted, will 'unmount'\n");//kHFSVolumeUnmountedBit not currently set in the volume attributes field.\n");
	} else {
	    printf("Volume already unmounted cleanly\n");
	}
	
	promptVer();
	
	char custom_sig[256];
	if(scanf("%s", custom_sig) != 1) {
	    printf("error: No signature read\n");
	    return 1;
	}
	if(strlen(custom_sig) > 4) {
	    printf("error: Signature too long\n");
	    return 1;
	}
	custom_sig[4] = '\0';
	if(strlen(custom_sig) != 4) {
	    printf("error: Signature too short\n");
	    return 1;
	}
	
	printf("Proceeding\n");
	
	attributes &= 0xffffdfff;
	printf("Journal disabled.\n");
	
	attributes |= 0x00000100;
	printf("Unmounted.\n");
	attributes = bswap_32(attributes);
	*(unsigned long *)(&buffer[1028]) = attributes;
	
	//LastMountedVersion
	buffer[1032] = custom_sig[0];//'1';
	buffer[1033] = custom_sig[1];//'0';
	buffer[1034] = custom_sig[2];//'.';
	buffer[1035] = custom_sig[3];//'0';
	
	printf("Set signature.\n");
	
	//journalInfoBlock
	buffer[1036] = 0;
	buffer[1037] = 0;
	buffer[1038] = 0;
	buffer[1039] = 0;
	
	printf("Zerod journal.\n");
	
	printf("\n\nNew info:\n");
	
	printInfo(buffer);
	return 0;
}
