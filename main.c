/*
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <webmaster@flippeh.de> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Lukas Niederbremer.
 * -----------------------------------------------------------------------------
 */

#include "nbt.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

static int opt_dupe = 0;

void dump_nbt(const char *filename);

int main(int argc, char **argv)
{
    int c;
    static int opt_dump = 0;

    //opterr = 0;

    for (;;)
    {
        static struct option long_options[] =
        {
            {"dump",    no_argument, &opt_dump, 1},
            {"version", no_argument, NULL, 'v'},
            {"copy",    no_argument, &opt_dupe, 1},
            {NULL,      no_argument, NULL, 0}
        };

        int option_index = 0;

        c = getopt_long(argc, argv, "dv", long_options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
            case 0:
                if (long_options[option_index].flag != 0)
                    break;

                break;

            case 'v':
                printf("nbttool 1.0 (%s, %s)\n", __DATE__, __TIME__);

                return EXIT_SUCCESS;

            case '?':
                break;
        }
    }

    if (optind < argc)
    {
        /* There is more in argv */

        if (opt_dump)
            dump_nbt(argv[optind]);
    }

    return 0;
}

void dump_nbt(const char *filename)
{
    assert(errno == NBT_OK);

    FILE* f = fopen(filename, "rb");
    nbt_node* root = nbt_parse_file(f);
    fclose(f);

    if(errno != NBT_OK)
    {
        fprintf(stderr, "Parsing error!\n");
        return;
    }

    char* str = nbt_dump_ascii(root);
    nbt_free(root);

    if(str == NULL)
        fprintf(stderr, "Printing error!");

    printf("%s", str);

    free(str);
}
