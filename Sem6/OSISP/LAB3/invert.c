#include <stdio.h>
#include <string.h>
#include "invert.h"

#define MAX_LEN 1024

void invert_chars(FILE *in, FILE *out)
{
    char buf[MAX_LEN];

    while (fgets(buf, MAX_LEN, in))
    {
        size_t len = strlen(buf);

        /* убираем перевод строки */
        if (len > 0 && buf[len - 1] == '\n')
        {
            buf[len - 1] = '\0';
            len--;
        }

        for (int i = len - 1; i >= 0; i--)
        {
            fputc(buf[i], out);
        }
        fputc('\n', out);
    }
}
