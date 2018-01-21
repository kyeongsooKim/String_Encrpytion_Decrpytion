#include "hw1.h"
#include <stdlib.h>
#include "myString.h"
#include "const.h" //for using polybius_alphabet, polybius_table
#include <stdio.h>

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the program
 * and will return a unsigned short (2 bytes) that will contain the
 * information necessary for the proper execution of the program.
 *
 * IF -p is given but no (-r) ROWS or (-c) COLUMNS are specified this function
 * MUST set the lower bits to the default value of 10. If one or the other
 * (rows/columns) is specified then you MUST keep that value rather than assigning the default.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return Refer to homework document for the return value of this function.
 */



unsigned short validargs(int argc, char **argv) {

    unsigned short ret = 0x0000; //return value indicates mode bitsj
    argv++;
    int arg_num = argc - 1; //for convenience


    //checking if no flag is speficied.
    if (*argv == NULL)
        return EXIT_FAILURE;


    //check if -h flag is specified before any other flags.
    if (my_strcmp(*argv, "-h") == 0)
    {
        ret |= 0x8000;
        argv++;
        arg_num--;
    }

    if (*argv == NULL)
        return ret;


    //checking first positional flag (-f|-p)
    if (my_strcmp(*argv, "-f")== 0) //check if -f flag is speficied
    {
        ret |= 0x4000;
    }
    else if (my_strcmp(*argv, "-p") != 0) //Invalid ordering of arguments or incorrect arguments
    {
        return EXIT_FAILURE;
    }

    argv++;
    arg_num--;

    if (*argv == NULL) // -d|-e should be speficied after -f|-p
        return EXIT_FAILURE;


    //checking second positional flag (-d|-e)
    if (*argv != NULL && my_strcmp(*argv, "-d")== 0) //check if -d flag is speficied
    {
        ret |= 0x2000;
    }
    else if (*argv != NULL  && my_strcmp(*argv, "-e") != 0) //Invalid ordering of arguments or incorrect arguments
    {
        return EXIT_FAILURE;
    }

    argv++;
    arg_num--;


    // when -f flag is specified
    if(ret & 0x4000)
    {
        if (arg_num == 2 && my_strcmp(*argv, "-k") == 0){
            argv++;
            int i = 0;

            while (**argv != '\0')
            {
                if (my_strchr2(fm_alphabet, **argv) == -1) //key alphabets should exist in polybius_alphabet array.
                    return EXIT_FAILURE;
                else
                {
                    for (int j = 0; j < i ;j++) //repetited alphabets are not allowed.
                        if (*(fm_key + j) == **argv)
                            return EXIT_FAILURE;

                    *(fm_key + i) = **argv;
                }

                i++;
                (*argv)++;
            }

            key = fm_key; // The value of the -k program argument goes here
            argv++;
            arg_num -= 2;
        }
    }
    else //when -p is specified.
    {
        bool r_specified = false;
        bool c_specified = false;
        bool k_specified = false;
        unsigned short mask;
        int col = 10; //default column is 10
        int row = 10; //default row is 10

        // checking optional arguments -k, -r, -c
        while(arg_num == 2 || arg_num == 4 || arg_num == 6){

            if (!r_specified && my_strcmp(*argv, "-r") == 0)
            {
                argv++;
                if (my_atoi(*argv) < 9 || my_atoi(*argv) > 15)
                    return EXIT_FAILURE;

                row = my_atoi(*argv);
                mask = (unsigned short)my_atoi(*argv);
                mask = mask << 4;
                ret |= mask;

                r_specified = true;
                argv++;
                arg_num -= 2;
            }
            else if (!c_specified && my_strcmp(*argv, "-c") == 0)
            {
                argv++;
                if (my_atoi(*argv) < 9 || my_atoi(*argv) > 15)
                    return EXIT_FAILURE;

                col = my_atoi(*argv);
                mask = (unsigned short)my_atoi(*argv);
                ret |= mask;

                c_specified = true;
                argv++;
                arg_num -= 2;
            }
            else if (!k_specified && my_strcmp(*argv, "-k") == 0)
            {

                argv++;
                int i = 0;

                while (**argv != '\0')
                {
                    if (my_strchr(polybius_alphabet, **argv) == NULL) //key alphabets should exist in polybius_alphabet array.
                        return EXIT_FAILURE;
                    else
                    {
                        for (int j = 0; j < i ;j++) //repetited alphabets are not allowed.
                            if (*(polybius_table + j) == **argv)
                                return EXIT_FAILURE;

                        *(polybius_table + i) = **argv;
                    }

                    i++;
                    (*argv)++;
                }

                key = polybius_table; // The value of the -k program argument goes here
                k_specified = true;
                argv++;
                arg_num -= 2;
            }
            else  //invalid flags.
                return EXIT_FAILURE;
        } //while

        //default when -r|-c is not speficied.
        if (!r_specified){
            mask = 10;
            mask = mask << 4;
            ret |= mask;

        }
        if (!c_specified){
            mask = 10;
            ret |= mask;
        }

        // row * col (size of the table) must be larger than the number of polybius_alphabets
        if ( row * col < my_strlen(polybius_alphabet) )
            return EXIT_FAILURE;
    }

    if (*argv != NULL) //unncessary arguments or invalid arguments
        return EXIT_FAILURE;


    return ret;
}


//setting Polybius Cipher Table for both encryption and decryption
void pc_util_setTable(){
    char * p = polybius_alphabet;

    int tableIdx = 0;
    if (key != NULL)
        tableIdx = my_strlen(key);

    while(*p != '\0')
    {

        if(my_strchr(polybius_table, *p) == NULL){
            *(polybius_table + tableIdx) = *p;
            tableIdx++;
            p++;
        }
        else{
            p++;
        }

    }
}

//Polybius Cipher - Encryption
void pc_encryption(unsigned short mode){

    pc_util_setTable(); //set Polybius Cipher table

    unsigned short mask = 0x000F;
    int div = (mode & mask);
    char c, num, row, col;

    c = getchar();
    while (c != EOF) {
        if (c == ' ' || c == '\t' || c == '\n')
            printf("%c",c);
        else
        {
            num = my_strchr(polybius_table, c) - polybius_table;
            row = num / div;
            col = num % div;
            printf("%X%X", row,col);
        }
        c = getchar();
    }
};


//Polybius Cipher - Dncryption
void pc_decryption(unsigned short mode){

    pc_util_setTable(); //set Polybius Cipher table

    unsigned short mask = 0x000F;
    int col = (mode & mask);
    char c, c2;

    c = getchar();
    while (c != EOF) {
        if (c == ' ' || c == '\t' || c == '\n')
            printf("%c",c);
        else
        {
            c2 = getchar();
            int idx = col * (c - '0') + (c2 - '0');

            printf("%c", *(polybius_table + idx));
        }
        c = getchar();
    }


};


//setting fm_key for both encryption and decryption
void fmc_util_setFmKey(){
    const char * p = fm_alphabet;

    int tableIdx = 0;
    if (key != NULL)
        tableIdx = my_strlen(key);

    while(*p != '\0')
    {

        if(my_strchr2(fm_key, *p) == -1){
            *(fm_key + tableIdx) = *p;
            tableIdx++;
            p++;
        }
        else{
            p++;
        }

    }
}

//utility function for Fractionated Morse Cipher Encryption.
void fmc_util_print(char * buffer){
    int idx = 0;
    for (int i = 0; i < 26; i ++){
        if (my_strcmp(*(fractionated_table + i), buffer) == 0 )
            break;
        idx++;
    }

    printf("%c", *(fm_key + idx));
}


void fmc_encryption(){


    fmc_util_setFmKey(); //set fm_key

    char * buffer = polybius_table; //use polybius_table as a buffer.
    int bIdx = 0; //buffer index
    char cPrev;


    char c = getchar();
    cPrev = c; //means nothing. just for the following while loop
    while (c != EOF)
    {


        if ((c == ' ' || c == '\t') && cPrev != ' ' && cPrev != '\t' && cPrev != '\n')
        {
            //at the end of the word, put the one more x (by white space)

            *(buffer + bIdx) = 'x';
            bIdx++;

            if(bIdx == FMC_BufSize){
                fmc_util_print(buffer);
                bIdx = 0;
            }

        }
        else if ((c == ' ' || c == '\t') && (cPrev == ' ' || cPrev == '\t' || cPrev == '\n'))
        {
            //Multiple spaces between words MUST be treated as a single space in translation.
            //do nothing

        }
        else if (c == '\n' && cPrev != ' ' && cPrev != '\t' && cPrev != '\n')
        {
            //at the end of the word, put the one more x (by enter)

            *(buffer + bIdx) = 'x';
            bIdx++;

            if(bIdx == FMC_BufSize){
                fmc_util_print(buffer);
                bIdx = 0;
            }
            printf("%c", c);
            bIdx = 0; //any leftover characters will be truncated off the end and not ciphered.
        }
        else if (c == '\n' && (cPrev == ' ' || cPrev == '\t' || cPrev == '\n'))
        {
            //if user hit the enter just after any other white space.

            printf("%c", c);
            bIdx = 0; //any leftover characters will be truncated off the end and not ciphered.
        }
        else
        {
            int j = my_strchr(polybius_alphabet, c) - polybius_alphabet;
            const char * p = *(morse_table + j);

            if ( *p == '\0') //If ASCII characters do not have a Morse Coding encoding.
                exit(EXIT_FAILURE);

            for (int i =0; i < my_strlen(p); i++){
                *(buffer + bIdx) = *(p + i);
                bIdx++;
                if(bIdx == FMC_BufSize){
                    fmc_util_print(buffer);
                    bIdx = 0;
                }
            }

        }

        //put x whenever read alphabet
        if (c != ' ' && c != '\t' && c != '\n'){
            *(buffer + bIdx) = 'x';
            bIdx++;

            if(bIdx == FMC_BufSize){
                fmc_util_print(buffer);
                bIdx = 0;
             }
        }



        cPrev = c;
        c = getchar();


        //******************* seems this doesn't work for some reason, never figured out yet**
        if (c == EOF && cPrev != ' ' && cPrev != '\t' && cPrev != '\n')
        {

            //just in case when file meets EOF with the last word without white spaces.

            *(buffer + bIdx) = 'x';
            bIdx++;

            if(bIdx == FMC_BufSize)
            {
                fmc_util_print(buffer);
                bIdx = 0;
            }

            break;
        }

    }


};



bool space = false;


//utility function for Fractionated Morse Cipher Decryption to flushing the buffer when it finds x
int fmc_util_flushBuf(char * buffer, int bIdx)
{
    space = false;
    int leftover = -1;

    for(int idx=0;idx< bIdx - 1 ;idx++){

        if (*(buffer + idx) == 'x')
        {
            for (int i = 0 ; *(morse_table + i) != NULL ; i++)
            {
                bool find = false;
                if (my_strlen(*(morse_table + i)) == idx)
                {
                    find = true;
                    for (int j = 0; j < idx; j++)
                    {
                        if  ( *(buffer + j) != *(*(morse_table + i) + j) )
                        {
                            find = false;
                            break;
                        }
                    }
                }

                if (find)
                {
                    printf("%c",*(polybius_alphabet + i));
                }
            }


            if (*(buffer + idx + 1) == 'x')
            {
                space = true;
                leftover = bIdx - (idx + 2);
            }
            else
            {

                leftover = bIdx - (idx + 1);
            }


            for (int i = 0; i <bIdx ; i++)
            {
                if (i < leftover)
                {
                    *(buffer + i) = *(buffer + bIdx - leftover + i);

                }
                else
                {
                    *(buffer + i) = '\0';
                }
            }
            return leftover;
        }
    }

    return leftover;
}


//utility function for Fractionated Morse Cipher Decryption
//it flushes the buffer when it finds x (variation version for program to read EOF or ENTER)
int fmc_util_flushBuf2(char * buffer, int bIdx)
{
    space = false;
    int leftover = -1;

    for(int idx=0;idx< bIdx; idx++){

        if (*(buffer + idx) == 'x')
        {
            for (int i = 0 ; *(morse_table + i) != NULL ; i++)
            {
                bool find = false;
                if (my_strlen(*(morse_table + i)) == idx)
                {
                    find = true;
                    for (int j = 0; j < idx; j++)
                    {
                        if  ( *(buffer + j) != *(*(morse_table + i) + j) )
                        {
                            find = false;
                            break;
                        }
                    }
                }

                if (find)
                {
                    printf("%c",*(polybius_alphabet + i));
                }
            }


            if (idx != bIdx - 1){
                if (*(buffer + idx + 1) == 'x')
                {
                    leftover = bIdx - (idx + 2);
                }
                else
                {
                    leftover = bIdx - (idx + 1);
                }
            }
            else
                leftover = bIdx - (idx + 1);



            for (int i = 0; i <bIdx ; i++)
            {
                if (i < leftover)
                {
                    *(buffer + i) = *(buffer + bIdx - leftover + i);

                }
                else
                {
                    *(buffer + i) = '\0';
                }
            }
            return leftover;
        }
    }

    return leftover;
}




//Fractionated Morse Cipher Encryption
void fmc_decryption(){

    fmc_util_setFmKey(); //set

    char * buffer = polybius_table; //use polybius_table as a buffer.
    int bIdx = 0; //buffer index
    int leftover = 0;

    char c = getchar();
    char cPrev;

    while (c != EOF)
    {
        if (c == '\n' && cPrev != '\n' && bIdx >= 3) // when single x or two x's is truncated off during encryption
        {
            if (*(buffer + bIdx - 1) != 'x')
            {
                *(buffer + bIdx) = 'x';
                bIdx++;
            }

            fmc_util_flushBuf2(buffer, bIdx);
            printf("\n");
            bIdx = 0;
        }
        else if(c == '\n')
        {
            fmc_util_flushBuf2(buffer, bIdx);
            printf("\n");
            bIdx = 0;
        }
        else
        {
            int j = my_strchr2(fm_key,c);

            //read 3 Morse symbols from fractionated_table to buffer e
            for (int i = 0 ; i < my_strlen(*(fractionated_table + j));i++)
            {
                *(buffer + bIdx) = *(*(fractionated_table + j) + i);
                bIdx++;
            }

            leftover = fmc_util_flushBuf(buffer, bIdx);

            //(leftover != -1) means fmc_util_FlushBuf flushed the buffer.
            //fmc_util_flushBuf returns -1 when it doesn't flush the buffer.
            if (leftover != -1)
            {
                bIdx = leftover;
            }

        }

        cPrev = c;
        c = getchar();

        // put space only '\n' is not coming after 'xx'.
        if(c != '\n' && space == true)
        {
            printf(" ");
            space = false;
        }


        if (c == EOF && cPrev != '\n')
        {
            if (*(buffer + bIdx - 1) != 'x')
            {
                *(buffer + bIdx) = 'x';
                bIdx++;
            }

            fmc_util_flushBuf2(buffer, bIdx);
        }
    }

};

