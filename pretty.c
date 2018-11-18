#include "pretty.h"

int simple = 1;

void promptVer() {
    if(simple) 
        printf("LastMountedVersion (4 Chars, Recommended 'H+Lx'): ");
    else {
	    printf("Enter the 4-Character Last-Moutned version ('\e[1mH+Lx\e[0m' or '\e[1m10.0\e[0m', H+Lx will work with linux read/write mount) ");
	    printf("Ctrl+C To cancel:\n");
	}
}

int UIntToStr(int bytes, UInt64 in, char *out) {
    strncpy(out, (char*)&in, bytes);
    out[bytes] = '\0';
}

#define BIT_SET     "\e[32m☑\e[0m"
#define BIT_UNSET   "\e[31m☐\e[0m"

void buf_print_str(const char *name, char *value, long offset) {
    if(simple)
        printf("%s=%s\n", name, value);
    else
        printf("%-20s[\e[32m+%4ld#%d\e[0m] \e[34m'%s'\e[0m\n",
            name, offset, strlen(name), value);
}

//Value is in big endian form:
void buf_print_u16(const char *name, UInt16 value, long offset) {
    if(simple)
        printf("%s=%hd\n", name, bswap_16(value));
    else
        printf("%-20s[\e[32;3m+%4ld#2\e[0m] \e[33m%hd\e[0m\n",
        name, offset, bswap_16(value));
}
void buf_print_u32(const char *name, UInt32 value, long offset) {
    if(simple)
        printf("%s=%d\n", name, bswap_32(value));
    else
        printf("%-20s[\e[32;3m+%4ld#4\e[0m] \e[33m%d\e[0m\n",
        name, offset, bswap_32(value));
}

char *getAttrib(int attributes, int bit) {
    if(simple)
        return (attributes & (1 << bit)) ? "Y " : "N ";
    else
        return (attributes & (1 << bit)) ? BIT_SET : BIT_UNSET;
}

void buf_print_bit(char *name, UInt32 value, int offset) {
    if(simple)
        printf("%s%4s\n", getAttrib(value, offset), name);
    else
        printf("\t%-33s[\e[32;3m0x%.8X\e[0m] %s\n",
            name, (1 << offset),
            getAttrib(value, offset));
            
}

void buf_print_hex(const char *name, UInt32 value, long offset) {
    if(simple)
        printf("%s=%X\n", name, bswap_32(value));
    else
        printf("%-20s[\e[32;3m+%4ld#4\e[0m] \e[35;3m0x%.8X\e[0m\n",
            name, offset, bswap_32(value));
}

//names is a (pointer to) an array of (pointers to) strings
void buf_print_mask(const char *name, UInt32 value, long offset, char *names[], int *offsets) {
    UInt32 real_val = bswap_32(value);
    if(simple)
        printf("%s=%X\n", name, value);
    else
        printf("%-20s[\e[32;3m+%4ld#4\e[0m] \e[35;3m0x%.8X\e[0m\n",
            name, offset, real_val);
    
    for(int i = 0; names[i] != NULL; i++) {
        buf_print_bit(names[i], real_val, offsets[i]);
    }
}
void printInfo(unsigned char *buffer) {
	HFSPlusVolumeHeader *info = (HFSPlusVolumeHeader*)(buffer+1024);
	
	char out2[3];
	char out4[5];
	UIntToStr(2, (UInt64)info->signature, out2);
	UIntToStr(4, (UInt64)info->lastMountedVersion, out4);
	
	#define offset(member) ((long)&info->member) - ((long)&info->signature)
	
	buf_print_str("Signature", out2, offset(signature));
	buf_print_u16("Version", info->version, offset(version));
	
    UInt32 attrib = bswap_32(info->attributes);
    
    char *names[8] = {
        "kHFSVolumeHardwareLockBit",
        "kHFSVolumeUnmountedBit",
        "kHFSVolumeSparedBlocksBit",
        "kHFSVolumeNoCacheRequiredBit",
        "kHFSBootVolumeInconsistentBit",
        "kHFSCatalogNodeIDsReusedBit",
        "kHFSVolumeJournaledBit",
        NULL
    };
    
    int offsets[7] = {
        kHFSVolumeHardwareLockBit,
        kHFSVolumeUnmountedBit,
        kHFSVolumeSparedBlocksBit,
        kHFSVolumeNoCacheRequiredBit,
        kHFSBootVolumeInconsistentBit,
        kHFSCatalogNodeIDsReusedBit,
        kHFSVolumeJournaledBit,
    };
    
    buf_print_mask("attributes", info->attributes, offset(attributes), names, offsets);
    
    buf_print_str("lastMountedVersion", out4, offset(lastMountedVersion));
    buf_print_hex("journalInfoBlock", info->journalInfoBlock, offset(journalInfoBlock));
    
    buf_print_u32("fileCount", info->fileCount, offset(fileCount));
    buf_print_u32("folderCount", info->folderCount, offset(folderCount));
    
    buf_print_u32("blockSize", info->blockSize, offset(blockSize));
    buf_print_u32("totalBlocks", info->totalBlocks, offset(totalBlocks));
    buf_print_u32("freeBlocks", info->freeBlocks, offset(freeBlocks));
}
