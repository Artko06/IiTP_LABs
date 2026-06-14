#include <stdio.h>
#include <stdlib.h>
#include "invert.h"

int main(int argc, char *argv[])
{
    FILE *in = stdin;
    FILE *out = stdout;

    if (argc > 1)
    {
        in = fopen(argv[1], "r");
        if (!in)
        {
            perror("input");
            return 1;
        }
    }

    if (argc > 2)
    {
        out = fopen(argv[2], "w");
        if (!out)
        {
            perror("output");
            return 1;
        }
    }

    invert_chars(in, out);

    if (in != stdin)
        fclose(in);
    if (out != stdout)
        fclose(out);

    return 0;
}

// make run 
// make clean
