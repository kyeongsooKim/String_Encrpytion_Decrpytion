#include <stdbool.h> //bool, true, false
#include <stdlib.h> //NULL



bool my_isNumericChar(char x)
{
        return (x >= '0' && x <= '9')? true: false;
}

int my_strlen(const char * str){
    int i = 0;
    while (*(str +i) != '\0') i++;
    return i;
}

int my_atoi(char *c)
{
    int value =0;
    int sign =1;
    if (*c == '+' || *c == '-')
    {
        if (*c == '-') sign = -1;
        c++;
    }
    while (my_isNumericChar(*c))
    {
        value *= 10;
        value += (int)(*c - '0');
        c++;
    }
    return (value * sign);
}

int my_strcmp(const char * str1, const char * str2){
    int i = 0;
    int result = 0;
    while(1)
    {
        if (*(str1 + i) == *(str2 + i))
        {
            if (*(str1 + i) == '\0')
            {
                result = 0;
                break;
            }
        }
        else
        {
            result = (*(str1 + i) >= *(str2 + i)) ? 1 : -1;
            break;

        }
        i++;
    }
    return result;
}

char * my_strchr(char * pSource, unsigned char ucFind)
{
    char * pFind = NULL;

    while (*pSource != '\0')
    {
        if (*pSource == ucFind)
        {
            pFind = pSource;
            break;
        }
        pSource++;
    }

    return pFind;
}

int my_strchr2(const char * pSource, unsigned char ucFind)
{
    int idx = 0;
    while (*pSource != '\0')
    {
        if (*pSource == ucFind)
            return idx;
        pSource++;
        idx++;
    }

    return -1;
}