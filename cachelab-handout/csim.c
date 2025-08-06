/*
 * @Author       : FeiYehua
 * @Date         : 2013-01-22 23:50:42
 * @LastEditTime : 2025-08-06 12:04:21
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

int numberOfHits, numberOfMisses, numberOfEvications;

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
struct set *sets;

// Perform the line matching.
// *resultPointer =
// address of corresponding cache if the data is already cached.
// Else, the tail of the list.
// Return: status code
// 0 if cache misses
// 1 if cache hit
int lineMatching(struct set *set, unsigned long long tag, struct line **resultPointer)
{
    struct line *head = set->head;
    struct line *cur = head;
    if (cur == NULL)
    {
        *resultPointer = NULL;
        return 0;
    }
    while (cur->tag != tag && cur->next != NULL)
    {
        cur = cur->next;
    }
    *resultPointer = cur;
    return cur->tag == tag;
}

// Update the head of the set
void updateFrequency(struct set *set, struct line *target)
{
    struct line *oldHead = set->head;
    oldHead->pre = target;
    struct line *targetPre = target->pre;
    struct line *targetNext = target->next;
    targetPre->next = targetNext;
    targetNext->pre = targetPre;
    target->next = oldHead;
    target->pre = NULL;
}

// Evict the least frequently used cache
void evictCache(struct line *target)
{
    if (target->next != NULL)
    {
        return;
    }
    struct line *tailPre = target->pre;
    tailPre->next = NULL;
    free(target);
}

// Insert a new cache entry
// We always insert a cache at the head of the list
void insertNewCache(struct set *set, unsigned long long tag)
{
    struct line *oldHead = set->head;
    struct line *newLine = malloc(sizeof(struct line));
    set->head = newLine;
    newLine->tag = tag;
    newLine->valid = 1;
    newLine->next = oldHead;
    if (oldHead != NULL)
        oldHead->pre = newLine;
}

// Perform cache operations
void cache(struct set *operatedSet, unsigned long long tag)
{
    struct line *operatedLine = NULL; // The result of line matching
    int result = lineMatching(operatedSet, tag, &operatedLine);
    if (result == 1)
    {
        // Case of cache hit
        if (v)
            printf("hit ");
        numberOfHits++;
    }
    else
    {
        // Case of cache miss
        if (v)
            printf("miss ");
        numberOfMisses++;
        insertNewCache(operatedSet, tag);
        if (operatedSet->count == E)
        {
            if (v)
                printf("eviction ");
            evictCache(operatedLine);
            numberOfEvications++;
        }
        else
        {
            operatedSet->count++;
        }
    }
}

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

void parseMemoryTrace(char *trace, char *op, unsigned long long *address, int *size)
{
    sscanf(trace, " %c %llx,%d", op, address, size);
}
int main(int argc, char **argv)
{
    parseCommendLineArguments(argc, argv);
    // Allocate memory space for sets
    sets = malloc(sizeof(struct set) * (1ull << s));
    memset(sets, 0, sizeof(struct set) * (1ull << s));
    char *bufferPointer = malloc(sizeof(char) * 30);
    while (1)
    {
        if (fgets(bufferPointer, 29, fp) == NULL)
        {
            break;
        }
        if (bufferPointer[0] == 'I')
        {
            continue;
        }
        else
        {
            if (v)
            {
                int len = strlen(bufferPointer);
                bufferPointer[len - 1] = 0;
                printf("%s ", bufferPointer+1);
            }
            char op;
            unsigned long long address;
            int size;
            parseMemoryTrace(bufferPointer, &op, &address, &size);
            address >>= b;                                          // Remove the block bits
            unsigned long long index = address & ((1ull << s) - 1); // Get set index
            unsigned long long tag = address >> s;                  // Get the tag bits
            struct set *operatedSet = &sets[index];                 // To be operated set
            cache(operatedSet, tag);
            if (op == 'M')
            {
                numberOfHits++;
                if (v)
                    printf("hit ");
            }
            if (v)
                printf("\n");
        }
    }

    printSummary(numberOfHits, numberOfMisses, numberOfEvications);
    return 0;
}
