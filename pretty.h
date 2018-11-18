#ifndef HFSFIX_PRETTY
#define HFSFIX_PRETTY
#include <string.h>
#include <stdio.h>
#include <byteswap.h> //bswap_32
#include "types.h"
void printInfo(unsigned char *buffer);
void promptVer();
#endif
