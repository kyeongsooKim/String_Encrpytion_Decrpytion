#include <stdlib.h>

#include "hw1.h"
#include "debug.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

int main(int argc, char **argv)
{
    unsigned short mode;

    mode = validargs(argc, argv);

    debug("Mode: 0x%X", mode);

    if(mode & 0x8000) // -h flag is specified
    {
        USAGE(*argv, EXIT_SUCCESS);
    }
    else if(mode == EXIT_FAILURE)
    {
        USAGE(*argv, EXIT_FAILURE);
    }
    else if( (mode & 0x6000) == 0x6000) //Fractionated Morse Cipher - Decryption
        fmc_decryption();
    else if( (mode & 0x4000) == 0x4000) //Fractionated Morse Cipher - Encryption
        fmc_encryption();
    else if( (mode & 0x2000) == 0x2000) //Polybius Cipher - Decryption
        pc_decryption(mode);
    else                             //Polybius Cipher - Encryption
        pc_encryption(mode);


    return EXIT_SUCCESS;
}

/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */