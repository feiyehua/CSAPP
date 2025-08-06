/*
 * @Author       : FeiYehua
 * @Date         : 2013-01-22 23:50:42
 * @LastEditTime : 2025-08-06 09:29:42
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : csim.c
 *      © 2024 FeiYehua
 */
#include "cachelab.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int s, E, b;
bool v;
FILE *fp;

struct line
{
    bool valid;             // always 1
    unsigned long long tag; // tag
    struct line *next;      // The next cache block
    struct line *pre;       // The previous cache block
};

struct set
{
    struct line *head; // The head of line list
    int count;         // Number of cached blocks
};

// print help message
void printHelpMessage()
{
    printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n\n");
    printf("Examples:\n");
    printf("  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

// A command line argument parser
void parseCommendLineArguments(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'h':
                printHelpMessage();
                break;
            case 'v':
                v = 1;
                break;
            case 's':
                i++;
                if (i < argc)
                {
                    s = atoi(argv[i]);
                }
                break;
            case 'E':
                i++;
                if (i < argc)
                {
                    E = atoi(argv[i]);
                }
                break;
            case 'b':
                i++;
                if (i < argc)
                {
                    b = atoi(argv[i]);
                }
                break;
            case 't':
                i++;
                if (i < argc)
                {
                    fp = fopen(argv[i], "r");
                }
                break;

            default:
                printf("%s: invalid option -- '%c'\n", argv[0], argv[i][1]);
                printHelpMessage();
                exit(0);
                break;
            }
        }
    }
    if (s <= 0 || E <= 0 || b <= 0 || fp == NULL)
    {
        printf("%s: Missing required command line argument\n", argv[0]);
        printHelpMessage();
        exit(0);
    }
}

void parseMemoryTrace(char *trace, char *op, long long *address, int *size)
{
    sscanf(trace, " %c %lld,%d", op, address, size);
}
int main(int argc, char **argv)
{
    parseCommendLineArguments(argc, argv);
    char *bufferPointer = malloc(sizeof(char) * 30);
    while (1)
    {
        int cnt = fscanf(fp, "%s", bufferPointer);
        if (cnt < 1)
        {
            break;
        }
        if (bufferPointer[0] == 'I')
        {
            continue;
        }
        
    }

    printSummary(0, 0, 0);
    return 0;
}
