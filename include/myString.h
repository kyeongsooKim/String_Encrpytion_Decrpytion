#ifndef MYSTRING_H
#define MYSTRING_H

#include <stdbool.h>


bool my_isNumericChar(char x);

int my_strlen(const char * str);
int my_atoi(char *c);
int my_strcmp(const char * str1, const char * str2);
char * my_strchr(char * pSource, unsigned char ucFind);
int my_strchr2(const char * pSource, unsigned char ucFind);


#endif
