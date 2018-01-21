#ifndef HW_H
#define HW_H

#include "const.h"

#define FMC_BufSize 3 //buffer size when we do fractionated morse cipher encryption is 3.
#define FMC_D_BufSize 90 //buffer size when we do fractionated morse cipher decryptionis 3.

void pc_encryption(unsigned short mode);
void pc_decryption(unsigned short mode);
void pc_util_setTable();


void fmc_encryption();
void fmc_decryption();
void fmc_util_setFmKey();
void fmc_util_print(char * buffer);
int fmc_util_flushBuf2(char * buffer, int bIdx);
int fmc_util_flushBuf(char * buffer, int bIdx);

#endif
