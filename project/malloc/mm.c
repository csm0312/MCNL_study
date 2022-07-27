#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

#define WSZIE 4 // 1word size - 4 byte
#define DSIZE 8 // double word size - 8 byte
#define CHUNKSIZE (1<<12) // heap 확장을 위한 size

// x, y중 큰 값을 반환
#define MAX(x, y) ((x) > (y) ? (x) : (y))

// 블록의 size와 할당여부를 알려주는 alloc bit를 합쳐 header와 footer에 담을 수 있도록 반환
#define PACK(size, alloc)    ((size) | (alloc))

// p의 값을 반환 - p는 (*void)형 이므로 형변환을 통해서 반환 한다.
#define GET(p)               (*(unsigned int *)(p))

// p의 값을 val로 변환 - p는 (*void)형 이므로 형변환을 통해서 변환 한다.
#define PUT(p, val)          (*(unsigned int *)(p) = (val))

// p의 size만 갖고 온다 - header나 footer size 반환시 이용
#define GET_SIZE(p)          (GET(p) & ~(0x7))

// p의 정체가 무엇인지 확인하고 있다 - header 나 footer가 free인지 alloc인지 확인한다.
#define GET_ALLOC(p)         (GET(p) & 0x1)

//bp를 word size 만큼 뒤로 가서 해당 header를 반환한다
#define HDRP(bp) ((char *)(bp) - WSIZE)

//bp끝으로 이동하여 footer를 반환한다
//header를 통해서 해당 size를 갖고 오고 해당 size만큼 이동 후 footer 위치로 이동하기 위해 8byte를 뺀다
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

//bp 다음 블럭으로 이동한다.
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))

//bp 이전 블럭으로 이동한다.
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))


static char* heap_listp;

// mm_init - 초기 가용 블럭을 만들어 주는 함수 - 가용리스트를 만들고 size를 확장해준다.
int mm_init(void){   
    // 4word는 필요하기에 그것보다 작으면 안되기 때문에 해당 size보다 작은 경우에 error로 간주함
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
        return -1;

    PUT(heap_listp, 0); // alignment padding
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); // prologue header
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); // prologue footer
    PUT(heap_listp + (3*WSIZE), PACK(0, 1)); // epliogue header
    heap_listp += (2*WSIZE); 
    
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;
    return 0;
}

//
static void *extend_heap(size_t words){
    char *bp;
    size_t size;
    // 가용 리스트의 크기를 8의 배수로 맞추기 위한 작업
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    // mem_sbrk함수를 이용하여 늘렸을 때, 늘어날 수 없다면 return NULL
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;
    // header와 footer 업데이트
    PUT(HDRP(bp), PACK(size, 0)); // header에 size와 할당비트 업데이트
    PUT(FTRP(bp), PACK(size, 0)); // footer에 size와 할당비트 업데이트
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); // 새로운 epilogue header생성

    return coalesce(bp); // 확장된 영역을 합쳐 준다
}

// mm_free - alloc된 블럭을 free 해준다
void mm_free(void *bp) {
    size_t size = GET_SIZE(HDRP(bp)); //해당 블록 size를 반환
    PUT(HDRP(bp), PACK(size, 0));  //header alloc bit를 0으로 변환 (size | 0)
    PUT(FTRP(bp), PACK(size, 0));  //footer alloc bit를 0으로 변환 (size | 0)
    coalesce(bp); 
}

// *coalesce - free된 영역들을 합쳐준다
static void *coalesce(void *bp){
    //상태 확인을 위해 이전 alloc 여부와 다음 alloc 여부를 확인한다.
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    // 앞 뒤 블록 둘 다 allocated일 경우 - 원래대로
    if (prev_alloc && next_alloc) {
        return bp;
    }
    
    // 뒤 블록만 free일 경우 - 현재 블록과 뒷 블록과 결합
    else if (prev_alloc && !next_alloc) {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }

    //앞 블록만 free일 경우 - 이전 블록과 현재 블록과 결합
    else if (!prev_alloc && next_alloc) {
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    // 앞 뒤 블록 둘 다 free일 경우 - 이전 블록부터 다음 블록까지 결합
    else {
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    return bp;
}

//
void *mm_malloc(size_t size){

    size_t asize;
    size_t extendsize;
    char *bp;
    if (size == 0)
        return NULL;
    if (size <= DSIZE)
        asize = 2*DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);
    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
        return NULL;
    place(bp, asize);
    return bp;

}