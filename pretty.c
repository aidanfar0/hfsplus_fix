#include "pretty.h"

void promptVer(char *arg) {
    printf("WARNING: This has potential to cause data loss, you should AT LEAST take a backup of the first 2048 bytes of the partition. \n");
        
    printf("LastMountedVersion (4 Chars, Default 'H+Lx'): ");
}

int UIntToStr(int bytes, UInt64 in, char *out) {
    strncpy(out, (char*)&in, bytes);
    out[bytes] = '\0';
}

void buf_print_str(const char *name, char *value) {
    printf("%s=%s\n", name, value);
}

//Value is in big endian form:
void buf_print_u16(const char *name, UInt16 value) {
    printf("%s=%hd\n", name, bswap_16(value));
}
void buf_print_u32(const char *name, UInt32 value) {
    printf("%s=%d\n", name, bswap_32(value));
}

char *getAttrib(int attributes, int bit) {
    return (attributes & (1 << bit)) ? "Y " : "N ";
}

void buf_print_bit(char *name, UInt32 value, int offset) {
    printf("%s%4s\n", getAttrib(value, offset), name);
}

void buf_print_hex(const char *name, UInt32 value) {
    printf("%s=%X\n", name, bswap_32(value));
}

//names is a (pointer to) an array of (pointers to) strings
void buf_print_mask(const char *name, UInt32 value, char *names[], int *offsets) {
    UInt32 real_val = bswap_32(value);
    printf("%s=0x%X\n", name, value);
    
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
	
	buf_print_str("Signature", out2);
	buf_print_u16("Version", info->version);
	
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
    
    buf_print_mask("attributes", info->attributes, names, offsets);
    
    buf_print_str("lastMountedVersion", out4);
    buf_print_hex("journalInfoBlock", info->journalInfoBlock);
    
    buf_print_u32("fileCount", info->fileCount);
    buf_print_u32("folderCount", info->folderCount);
    
    buf_print_u32("blockSize", info->blockSize);
    buf_print_u32("totalBlocks", info->totalBlocks);
    buf_print_u32("freeBlocks", info->freeBlocks);
}
