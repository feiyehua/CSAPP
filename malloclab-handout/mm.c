/*
 * @Author       : FeiYehua
 * @Date         : 2015-04-02 02:12:26
 * @LastEditTime : 2025-08-14 22:54:10
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : mm.c
 *      © 2024 FeiYehua
 */
/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* $begin mallocmacros */
/* Basic constants and macros */
#define WSIZE 4 /* Word and header/footer size (bytes) */            // line:vm:mm:beginconst
#define DSIZE 8                                                      /* Double word size (bytes) */
#define CHUNKSIZE (1 << 12) /* Extend heap by this amount (bytes) */ // line:vm:mm:endconst
#define MIN_BLOCK_SIZE 16                                            // The minimun size of a block

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc)) // line:vm:mm:pack

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))              // line:vm:mm:get
#define PUT(p, val) (*(unsigned int *)(p) = (val)) // line:vm:mm:put

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7) // line:vm:mm:getsize
#define GET_ALLOC(p) (GET(p) & 0x1) // line:vm:mm:getalloc
#define GET_PRE_ALLOC(p) (GET(p) & PRE_ALLOC)
#define GET_MASK(p) (GET(p) & 0x7) // Get the mask of p

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)                      // line:vm:mm:hdrp
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) // line:vm:mm:ftrp

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE))) // line:vm:mm:nextblkp
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) // line:vm:mm:prevblkp

/* Mask for status bits */
#define ALLOC 1
#define NEXT_ALLOC 2 // Actually, we can easily determine whether the next block is allocated, so this bit is unnecessary.
#define PRE_ALLOC 4

/* $end mallocmacros */

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Team",
    /* First member's full name */
    "Yehua Fei",
    /* First member's email address */
    "hello@csapp.it",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""};

/* LIST data structure */
struct LIST
{
    void *next;
    void *prev;
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t))) // Evaluate to 8

/* Function prototypes for internal helper routines */
static void *extend_heap(size_t size);
static void coalesce(void *bp);
static void *find_fit(size_t asize);
static void split_block(void *bp, size_t size, unsigned int block_header_content);
static void update_next_block(void *bp, unsigned int block_header_content);

/* Function prototypes for internal list operations */
static void insert(void *bp);
static void delete(void *bp);

static void *init_block; // The (virtual) payload pointer of the front boundry block
static void *last_block;
static void *first_free_block = NULL;
static int max_free;

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    init_block = mem_sbrk(4);   // Extent the heap size by 4 byte to store the initial block header
    PUT(init_block, 0 | ALLOC); // Store the initial block header
    last_block = mem_sbrk(4);   // Extent the heap size by 4 byte to store the initial block header
    PUT(last_block, 0 | ALLOC | PRE_ALLOC);
    init_block = (void *)((char *)init_block + 4);
    first_free_block = NULL;
    max_free = -1;
    return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    void *free_bp = find_fit(size);
    return (free_bp != NULL) ? free_bp : extend_heap(size);
}

/*
 * add_footer - Copy bp's header's content to it's footer.
 */
void add_footer(void *bp)
{
    void *header = HDRP(bp);
    void *footer = FTRP(bp);
    unsigned int header_content = GET(header);
    PUT(footer, header_content);
}

/*
 * mm_free - Freeing a block does nothing.
 argument ptr is a pointer to the block payload.
 */
void mm_free(void *ptr)
{
    void *header_ptr = HDRP(ptr);
    PUT(header_ptr, GET(header_ptr) & (~ALLOC)); // Modify the header
    add_footer(ptr);
    void *next_block_ptr = NEXT_BLKP(ptr);
    void *next_block_header_ptr = HDRP(next_block_ptr);
    PUT(next_block_header_ptr, GET(next_block_header_ptr) & (~PRE_ALLOC)); // Mark the next block's header: the previous block is a free block
    insert(ptr);                                                           // Add this block to the free list
    if (!(GET(next_block_header_ptr) & ALLOC))                             // The next block is free, we also modify it's footer to keep consistency
    {
        // add_footer(next_block_ptr);// This can be omitted as only the size in footer matters
    }
    coalesce(ptr); // Merge the blocks
}

/*
 * coalesce - Merge the nearby free blocks.
 * argument ptr is a pointer to the operated block payload.
 * Make sure the bp is a free block.
 */
static void coalesce(void *bp)
{
    void *next_block_ptr = NEXT_BLKP(bp);
    void *next_block_header_ptr = HDRP(next_block_ptr);
    size_t next_block_size = GET_SIZE(next_block_header_ptr);
    void *current_block_header_ptr = HDRP(bp);
    size_t current_block_size = GET_SIZE(current_block_header_ptr);
    unsigned int mask = GET_MASK(current_block_header_ptr); // Get the status bits of current block
    if (!(GET(next_block_header_ptr) & ALLOC))              // The next block is free
    {
        int new_block_size = current_block_size + next_block_size;
        max_free = MAX(new_block_size, max_free);
        PUT(current_block_header_ptr, new_block_size | mask);
        add_footer(bp);                 // Add footer for the new block
        delete(next_block_ptr);         // Delete the old next block in the list
        next_block_ptr = NEXT_BLKP(bp); // The new next block's payload address
        next_block_header_ptr = HDRP(next_block_ptr);
        PUT(next_block_header_ptr, GET(next_block_header_ptr) & (~PRE_ALLOC)); // Mark the next block's previous block as free
        if (!(GET(next_block_header_ptr) & ALLOC))                             // The new next block is free
        {
            coalesce(bp);
        }
    }
    if (!(mask & PRE_ALLOC)) // If the previous block is free
    {
        void *previous_block_ptr = PREV_BLKP(bp);
        coalesce(previous_block_ptr); // Coalesce from the previous block's perspective
    }
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    // We have to implement proper alignment. This is the minimun memory space needed, including the header overhead.
    size = MAX(ALIGN(size + sizeof(size_t)), 16);
    void *newptr;
    size_t copySize;
    void *next_block = NEXT_BLKP(ptr);
    void *next_block_header = HDRP(next_block);
    unsigned int next_block_header_content = GET(next_block_header);
    unsigned int alloc = next_block_header_content & ALLOC;
    size_t next_block_size = next_block_header_content & (~0x7);
    void *header = HDRP(ptr);
    unsigned int block_header_content = GET(header);
    size_t current_block_size = GET_SIZE(header);

    if (current_block_size >= size) // This is a memory space shink
    {
        if (current_block_size - size >= MIN_BLOCK_SIZE) // Enough space to create a new block
        {
            split_block(ptr, size, block_header_content);
        }
        else
        {
            // Do nothing, use the space as padding
        }
        return oldptr;
    }
    if (!(alloc) && next_block_size + current_block_size >= size) // This is a memory expand, but we can complete the re-location in place
    {
        unsigned int current_block_prev_alloc = block_header_content & PRE_ALLOC;
        if (next_block_size + current_block_size - size >= MIN_BLOCK_SIZE)
        {
            PUT(header, size | ALLOC | current_block_prev_alloc);
            split_block(next_block, size - current_block_size, next_block_header_content);
        }
        else
        {
            // Edit the header of block
            delete(next_block); // Delete the next block in the free block list
            update_next_block(ptr, (next_block_size + current_block_size) | current_block_prev_alloc);
        }
        return oldptr;
    }
    else
    {
        newptr = mm_malloc(size);
        if (newptr == NULL)
            return NULL;
        copySize = (*(size_t *)((char *)oldptr - sizeof(size_t))) & (~(ALLOC & PRE_ALLOC & NEXT_ALLOC));
        if (size < copySize)
            copySize = size;
        memcpy(newptr, oldptr, copySize);
        mm_free(oldptr);
        return newptr;
    }
}

/*
 * extend_heap - Extent the heap size by argument size (in byte), with proper alignment
 */
void *extend_heap(size_t size)
{
    int newsize = ALIGN(size + sizeof(size_t));
    // For an allocated block, we use the first 4 byte as header. The minimum block size is 8 byte.
    // For an free block, we use the the fisrt 4 byte and the last 4 byte as footer, the minimum block size is 8 byte.
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
        return NULL;
    else
    {
        void *new_last_block = (void *)((char *)p - 4 + newsize); // New location of last block
        // unsigned int last_block_header = GET(last_block); // Old last block's header
        PUT(new_last_block, 0 | ALLOC | PRE_ALLOC); // Modifiy the new last block's header
        p = last_block;                             // The return block's header pointer
        last_block = new_last_block;
        PUT(p, (newsize) | GET_PRE_ALLOC(p) | ALLOC); // We put the total size of the block (including the size of header) to the header so that we can quickly find the next block
        return (void *)((char *)p + sizeof(size_t));
    }
}

/*
 * find_fit - Find the first free block with size size
 */
static void *find_fit(size_t asize)
{
    int newsize = ALIGN(asize + sizeof(size_t));
    if (first_free_block == NULL)
    {
        return first_free_block;
    }
    if (max_free < newsize && max_free != -1)
    {
        return NULL;
    }

    void *current_bp = first_free_block; // The pointer to the first block's payload
    size_t max_size = 0;
    while (current_bp != NULL)
    {
        void *current_block_header_pointer = HDRP(current_bp);
        unsigned int current_block_header_content = GET(current_block_header_pointer);
        unsigned int alloc = 0;
        size_t size = current_block_header_content & (~0x7); // The size of free block
        max_size = MAX(size, max_size);
        if ((!alloc) && size - MIN_BLOCK_SIZE >= newsize) // Freed block have enough space to create a new block
        {
            split_block(current_bp, newsize, current_block_header_content);
            return current_bp;
        }
        else if ((!alloc) && size >= newsize)
        {
            delete(current_bp); // Remove the current block from free block list
            update_next_block(current_bp, current_block_header_content);
            return current_bp;
        }
        current_bp = ((struct LIST *)current_bp)->next;
    }
    max_free = max_size;
    return NULL;
}

/*
 * split_block - Split the given (free) block into half, with the first half having size size.
 * The first block is always allocated.
 */
static void split_block(void *bp, size_t size, unsigned int block_header_content)
{
    delete(bp);
    size_t oldsize = block_header_content & (~0x7);
    unsigned int mask = block_header_content & 0x7;
    PUT(HDRP(bp), size | mask | ALLOC);
    void *next_bp = NEXT_BLKP(bp);
    void *next_block_header_pointer = HDRP(next_bp);
    PUT(next_block_header_pointer, (oldsize - size) | PRE_ALLOC); // Update next block's header
    add_footer(next_bp);
    insert(next_bp);
}

/*
 * update_next_block - When bp is allocated, update the PRE_ALLOC of the next block.
 */
static void update_next_block(void *bp, unsigned int block_header_content)
{
    PUT(HDRP(bp), block_header_content | ALLOC);
    void *next_bp = NEXT_BLKP(bp);
    void *next_block_header_pointer = HDRP(next_bp);
    PUT(next_block_header_pointer, GET(next_block_header_pointer) | PRE_ALLOC); // Update next block's header
    return;
}

/* List implement code */

/* Implement a doubly list to find free blocks faster */
/* In the payload area, the first 4 byte stores the next free block, and the 5-8 byte stores the previous block */
/* The type of block in the previous codes is void *. */

/*
 * insert - Insert a new free block to the list.
 */
static void insert(void *bp)
{
    int size = GET_SIZE(HDRP(bp));
    max_free = MAX(size, max_free);
    struct LIST *old_head = (struct LIST *)first_free_block;
    void *old_head_block = first_free_block;
    struct LIST *new_head = (struct LIST *)bp;
    first_free_block = bp;
    if (old_head == NULL) // There is no free block before
    {
        new_head->next = NULL;
        new_head->prev = NULL;
    }
    else
    {
        old_head->prev = first_free_block;
        new_head->next = old_head_block;
        new_head->prev = NULL;
    }
}

/*
 * delete - Delete a free block in the list.
 */
static void delete(void *bp)
{
    struct LIST *ele = (struct LIST *)bp; // The operated element
    if (ele->prev != NULL)
    {
        struct LIST *pre = (struct LIST *)ele->prev;
        pre->next = ele->next;
    }
    else // This element is the head of list
    {
        first_free_block = ele->next;
    }
    if (ele->next != NULL)
    {
        struct LIST *next = (struct LIST *)ele->next;
        next->prev = ele->prev;
    }
}