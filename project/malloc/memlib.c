#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "memlib.h"

#define MAX_HEAP (100*(1<<20)) //100MB


/* Private global variables */
static char *mem_heap; // points to first byte of heap
static char *mem_brk; // points to last byte of heap plus 1
static char *mem_max_addr; // Max legal heap addr plus 1

// mem_init - Initialize the memory system model
void mem_init(){
    mem_heap = (char *)Malloc(MAX_HEAP);
    mem_brk = (char *)mem_heap;
    mem_max_addr = (char *)(mem_heap + MAX_HEAP);
}

// mem_sbrk - heap의 영역을 확장시켜주는 함수. 확장 후 변경 된 주소를 보내준다.
void *mem_sbrk(int incr){
    char *old_brk = mem_brk; // return을 위해 원래 mem_brk를 저장해준다.

    if ( (incr < 0) || ((mem_brk + incr) > mem_max_addr)) {
        errno = ENOMEM;
        fprintf(stderr, "ERROR: mem_sbrk failed. Ran out of memory...\n");
        return (void *)-1;
    }
    mem_brk += incr;
    return (void *)old_brk;
}